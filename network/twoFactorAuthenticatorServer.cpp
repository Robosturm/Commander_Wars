#include "network/twoFactorAuthenticatorServer.h"
#include "network/mainserver.h"
#include "multiplayer/totp.h"
#include <QSqlQuery>
#include <QSqlError>

TwoFactorAuthenticatorServer::TwoFactorAuthenticatorServer(MainServer & mainServer)
    : m_mainServer(mainServer)
{
    Totp::selfTest();    
}

void TwoFactorAuthenticatorServer::send2faResponse(qint64 socketId, const QString & command, GameEnums::LoginError result, const QJsonObject & additionalData)
{
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
    QJsonObject outData = additionalData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    QJsonDocument outDoc(outData);
    emit m_mainServer.getGameServer()->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void TwoFactorAuthenticatorServer::start2faSetup(qint64 socketId, const QJsonObject &objData)
{
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Starting 2fa setup for username " + username, GameConsole::eDEBUG);
    bool success = false;
    QSqlQuery query = MainServer::getAccountInfo(m_mainServer.getDatabase(), username, success);
    if (!query.first() || !success)
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSSETUP2FA, GameEnums::LoginError_AccountDoesntExist);
        return;
    }
    if (!query.value(MainServer::SQL_TOTPSECRET).toString().isEmpty())
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSSETUP2FA, GameEnums::LoginError_2faAlreadyConfigured);
        return;
    }
    TotpEnrollment enrollment;
    enrollment.username = username;
    enrollment.base32Secret = Totp::base32Encode(Totp::generateSecret());
    enrollment.created = QDateTime::currentDateTimeUtc();
    m_pending2faSetups.insert(socketId, enrollment);
    QJsonObject additionalData;
    additionalData.insert(JsonKeys::JSONKEY_TOTPSECRET, enrollment.base32Secret);
    additionalData.insert(JsonKeys::JSONKEY_TOTPURL, Totp::buildOtpAuthUrl(QStringLiteral("CommanderWars"), username, enrollment.base32Secret));
    send2faResponse(socketId, NetworkCommands::SERVERRESPONSSETUP2FA, GameEnums::LoginError_None, additionalData);
}

void TwoFactorAuthenticatorServer::confirm2faSetup(qint64 socketId, const QJsonObject &objData)
{
    QString code = objData.value(JsonKeys::JSONKEY_TOTPCODE).toString();
    auto enrollment = m_pending2faSetups.find(socketId);
    if (enrollment == m_pending2faSetups.end())
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSCONFIRM2FA, GameEnums::LoginError_2faSetupExpired);
        return;
    }
    if (enrollment->created.msecsTo(QDateTime::currentDateTimeUtc()) > Settings::getInstance()->getTotpSetupTimeoutMs())
    {
        m_pending2faSetups.erase(enrollment);
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSCONFIRM2FA, GameEnums::LoginError_2faSetupExpired);
        return;
    }
    const QByteArray secret = Totp::base32Decode(enrollment->base32Secret);
    if (Totp::validateCode(secret, code))
    {
        const QString username = enrollment->username;
        const QString base32Secret = enrollment->base32Secret;
        m_pending2faSetups.erase(enrollment);
        if (storeTotpSecret(m_mainServer.getDatabase(), username, base32Secret))
        {
            CONSOLE_PRINT("2fa activated for username " + username, GameConsole::eDEBUG);
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSCONFIRM2FA, GameEnums::LoginError_None);
        }
        else
        {
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSCONFIRM2FA, GameEnums::LoginError_DatabaseNotAccesible);
        }
    }
    else
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSCONFIRM2FA, GameEnums::LoginError_Invalid2faCode);
    }
}

void TwoFactorAuthenticatorServer::cancel2fa(qint64 socketId, const QJsonObject &objData)
{
    Q_UNUSED(objData);
    CONSOLE_PRINT("Canceling 2fa workflows of client " + QString::number(socketId), GameConsole::eDEBUG);
    m_pending2faSetups.remove(socketId);
    m_passwordResetSessions.remove(socketId);
}

void TwoFactorAuthenticatorServer::startPasswordReset(qint64 socketId, const QJsonObject &objData)
{
    QString mailAdress = objData.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Password reset requested for username " + username, GameConsole::eDEBUG);
    bool success = false;
    QSqlQuery query = MainServer::getAccountInfo(m_mainServer.getDatabase(), username, success);
    if (!query.first() || !success)
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_AccountDoesntExist);
    }
    else
    {
        
        const QString totpSecret = query.value(MainServer::SQL_TOTPSECRET).toString();
        if (!totpSecret.isEmpty())
        {
            if (isPasswordResetAllowed(m_mainServer.getDatabase(), query))
            {
                // totp based reset: ask the client for the current code of the user's app
                PasswordResetSession session;
                session.username = username;
                session.created = QDateTime::currentDateTimeUtc();
                m_passwordResetSessions.insert(socketId, session);
                send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_None);
            }
            else
            {
                send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_2faLockedDueToTooManyFailedAttempts);
            }
        }
        else
        {
            // no 2fa configured and no mail server available: the account can't be reset
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_No2faConfigured);
        }
    }
}

bool TwoFactorAuthenticatorServer::isPasswordResetAllowed(QSqlDatabase &database, QSqlQuery &accountInfo)
{
    bool allowed = false;
    auto loginFailCount = accountInfo.value(MainServer::SQL_TOTPSECRETFAILCOUNT).toInt();
    if (loginFailCount < Settings::getInstance()->getPasswordResetMaxAttempts())
    {
        allowed = true;
    }
    else
    {
        auto firstFailTime = accountInfo.value(MainServer::SQL_TOTPSECRETFIRSTFAILTIME).toLongLong();
        qint64 currentTime = QDateTime::currentSecsSinceEpoch();
        if (currentTime - firstFailTime > Settings::getInstance()->getPasswordFailTimeoutS())
        {
            resetLoginAttempts(database, accountInfo);
            allowed = true;
        }
    }
    return allowed;
}

void TwoFactorAuthenticatorServer::resetLoginAttempts(QSqlDatabase &database, QSqlQuery &accountInfo)
{
    // reset login fail count and first fail time
    QSqlQuery resetQuery(database);
    resetQuery.prepare(QString("UPDATE ") + MainServer::SQL_TABLE_PLAYERS + " SET " +
                       MainServer::SQL_TOTPSECRETFAILCOUNT + " = 0, " +
                       MainServer::SQL_TOTPSECRETFIRSTFAILTIME + " = 0 WHERE " +
                       MainServer::SQL_USERNAME + " = ?;");
    resetQuery.addBindValue(accountInfo.value(MainServer::SQL_USERNAME).toString());
    resetQuery.exec();
}

void TwoFactorAuthenticatorServer::submitPasswordReset2faCode(qint64 socketId, const QJsonObject &objData)
{
    QString code = objData.value(JsonKeys::JSONKEY_TOTPCODE).toString();
    auto session = m_passwordResetSessions.find(socketId);
    if (session == m_passwordResetSessions.end())
    {
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_2faResetTimeout);
    }
    else if (session->created.msecsTo(QDateTime::currentDateTimeUtc()) > Settings::getInstance()->getPasswordResetTimeoutMs())
    {
        m_passwordResetSessions.erase(session);
        send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_2faResetTimeout);
    }
    else
    {
        bool success = false;
        QSqlQuery query = MainServer::getAccountInfo(m_mainServer.getDatabase(), session->username, success);
        const QString totpSecret = (query.first() && success) ? query.value(MainServer::SQL_TOTPSECRET).toString() : QString();
        if (totpSecret.isEmpty())
        {
            m_passwordResetSessions.erase(session);
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_No2faConfigured);
        }
        else if (isPasswordResetAllowed(m_mainServer.getDatabase(), query))
        {
            if (Totp::validateCode(Totp::base32Decode(totpSecret), code))
            {
                resetLoginAttempts(m_mainServer.getDatabase(), query);
                const QString username = session->username;
                m_passwordResetSessions.erase(session);
                QString newPassword = m_mainServer.createRandomPassword();
                Password password;
                password.setPassword(newPassword);
                QSqlQuery changeQuery(m_mainServer.getDatabase());
                changeQuery.prepare(QString("UPDATE ") + MainServer::SQL_TABLE_PLAYERS + " SET " +
                                    MainServer::SQL_PASSWORD + " = ?, " +
                                    MainServer::SQL_VALIDPASSWORD + " = 0 WHERE " +
                                    MainServer::SQL_USERNAME + " = ?;");
                auto hash = password.getHash().toHex();
                changeQuery.addBindValue(hash);
                changeQuery.addBindValue(username);
                changeQuery.exec();
                if (!MainServer::sqlQueryFailed(changeQuery))
                {
                    CONSOLE_PRINT("Password reset by 2fa for username " + username, GameConsole::eDEBUG);
                    QJsonObject additionalData;
                    additionalData.insert(JsonKeys::JSONKEY_NEWPASSWORD, newPassword);
                    send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_None, additionalData);
                }
                else
                {
                    send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_DatabaseNotAccesible);
                }
            }
            else
            {
                handleLoginFailed(m_mainServer.getDatabase(), query);
                ++session->attempts;
                if (session->attempts >= Settings::getInstance()->getPasswordResetMaxAttempts())
                {
                    CONSOLE_PRINT("Too many wrong 2fa codes for password reset of client " + QString::number(socketId), GameConsole::eDEBUG);
                    m_passwordResetSessions.erase(session);
                    send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_TooMany2faAttempts);
                }
                else
                {
                    send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_Invalid2faCode);
                }
            }
        }
        else
        {
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_2faLockedDueToTooManyFailedAttempts);
        }
    }
}

void TwoFactorAuthenticatorServer::handleLoginFailed(QSqlDatabase &database, QSqlQuery &accountInfo)
{
    auto loginFailCount = accountInfo.value(MainServer::SQL_TOTPSECRETFAILCOUNT).toInt();
    loginFailCount++;
    qint64 firstFailTime = 0;
    if (loginFailCount >= Settings::getInstance()->getPasswordResetMaxAttempts())
    {
        firstFailTime = QDateTime::currentSecsSinceEpoch();
    }    
    // lock account
    QSqlQuery lockQuery(database);
    lockQuery.prepare(QString("UPDATE ") + MainServer::SQL_TABLE_PLAYERS + " SET " +
                      MainServer::SQL_TOTPSECRETFAILCOUNT + " = ?, " +
                      MainServer::SQL_TOTPSECRETFIRSTFAILTIME + " = ? WHERE " +
                      MainServer::SQL_USERNAME + " = ?;");
    lockQuery.addBindValue(loginFailCount);
    lockQuery.addBindValue(firstFailTime);
    lockQuery.addBindValue(accountInfo.value(MainServer::SQL_USERNAME).toString());
    lockQuery.exec();
}

void TwoFactorAuthenticatorServer::cleanUpExpired2faSessions()
{
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for (auto it = m_pending2faSetups.begin(); it != m_pending2faSetups.end();)
    {
        if (it->created.msecsTo(now) > Settings::getInstance()->getTotpSetupTimeoutMs())
        {
            CONSOLE_PRINT("Dropping expired 2fa setup of client " + QString::number(it.key()), GameConsole::eDEBUG);
            it = m_pending2faSetups.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = m_passwordResetSessions.begin(); it != m_passwordResetSessions.end();)
    {
        if (it->created.msecsTo(now) > Settings::getInstance()->getPasswordResetTimeoutMs())
        {
            CONSOLE_PRINT("Password reset of client " + QString::number(it.key()) + " timed out", GameConsole::eDEBUG);
            const quint64 socketId = it.key();
            it = m_passwordResetSessions.erase(it);
            // inform the client so the cancellation is shown to the user
            send2faResponse(socketId, NetworkCommands::SERVERRESPONSRESETPASSWORD2FA, GameEnums::LoginError_2faResetTimeout);
        }
        else
        {
            ++it;
        }
    }
}

void TwoFactorAuthenticatorServer::disconnectClient(qint64 socketId)
{
    // drop pending 2fa states of the disconnected client
    m_pending2faSetups.remove(socketId);
    m_passwordResetSessions.remove(socketId);
}

void TwoFactorAuthenticatorServer::migrateAddTotpDatabase(QSqlDatabase &database)
{
    // migrate older databases: add the totp secret column if missing
    QSqlQuery pragmaQuery(database);
    pragmaQuery.exec(QString("PRAGMA table_info(") + MainServer::SQL_TABLE_PLAYERS + ")");
    bool hasTotpColumn = false;
    while (pragmaQuery.next())
    {
        if (pragmaQuery.value("name").toString() == QLatin1String(MainServer::SQL_TOTPSECRET))
        {
            hasTotpColumn = true;
            break;
        }
    }
    if (!hasTotpColumn)
    {
        QSqlQuery alterQuery(database);
        alterQuery.exec(QString("ALTER TABLE ") + MainServer::SQL_TABLE_PLAYERS + " ADD COLUMN " + MainServer::SQL_TOTPSECRET + " TEXT DEFAULT ''");
        if (MainServer::sqlQueryFailed(alterQuery))
        {
            CONSOLE_PRINT("Unable to add totp column to player table: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Added totp secret column to player table", GameConsole::eDEBUG);
        }
    }
    // migrate older databases: add the totp secret fail count column if missing
    bool hasTotpFailCountColumn = false;
    pragmaQuery.exec(QString("PRAGMA table_info(") + MainServer::SQL_TABLE_PLAYERS + ")");
    while (pragmaQuery.next())
    {
        if (pragmaQuery.value("name").toString() == QLatin1String(MainServer::SQL_TOTPSECRETFAILCOUNT))
        {
            hasTotpFailCountColumn = true;
            break;
        }
    }
    if (!hasTotpFailCountColumn)
    {
        QSqlQuery alterQuery(database);
        alterQuery.exec(QString("ALTER TABLE ") + MainServer::SQL_TABLE_PLAYERS + " ADD COLUMN " + MainServer::SQL_TOTPSECRETFAILCOUNT + " INTEGER DEFAULT 0");
        if (MainServer::sqlQueryFailed(alterQuery))
        {
            CONSOLE_PRINT("Unable to add totp secret fail count column to player table: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Added totp secret fail count column to player table", GameConsole::eDEBUG);
        }
        alterQuery.exec(QString("ALTER TABLE ") + MainServer::SQL_TABLE_PLAYERS + " ADD COLUMN " + MainServer::SQL_TOTPSECRETFIRSTFAILTIME + " INTEGER DEFAULT 0");
        if (MainServer::sqlQueryFailed(alterQuery))
        {
            CONSOLE_PRINT("Unable to add totp secret first fail time column to player table: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Added totp secret first fail time column to player table", GameConsole::eDEBUG);
        }
        alterQuery.exec(QString("ALTER TABLE ") + MainServer::SQL_TABLE_PLAYERS + " ADD COLUMN " + MainServer::SQL_LOGINFAILCOUNT + " INTEGER DEFAULT 0");
        if (MainServer::sqlQueryFailed(alterQuery))
        {
            CONSOLE_PRINT("Unable to add login fail count column to player table: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Added login fail count column to player table", GameConsole::eDEBUG);
        }
        alterQuery.exec(QString("ALTER TABLE ") + MainServer::SQL_TABLE_PLAYERS + " ADD COLUMN " + MainServer::SQL_LOGINFIRSTFAILTIME + " INTEGER DEFAULT 0");
        if (MainServer::sqlQueryFailed(alterQuery))
        {
            CONSOLE_PRINT("Unable to add login first fail time column to player table: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        }
        else
        {
            CONSOLE_PRINT("Added login first fail time column to player table", GameConsole::eDEBUG);
        }
    }
}

bool TwoFactorAuthenticatorServer::hasTotpSecret(QSqlDatabase &database, const QString &username)
{
    bool success = false;
    QSqlQuery query = MainServer::getAccountInfo(database, username, success);
    if (query.first() && success)
    {
        auto value = query.value(MainServer::SQL_TOTPSECRET);
        if (value.isNull())
        {
            CONSOLE_PRINT("Totp secret column is null for user " + username, GameConsole::eDEBUG);
        }
        else
        {
            return !value.toString().isEmpty();
        }
    }
    return false;
}

bool TwoFactorAuthenticatorServer::storeTotpSecret(QSqlDatabase &database, const QString &username, const QString &base32Secret)
{
    QSqlQuery updateQuery(database);
    updateQuery.prepare(QString("UPDATE ") + MainServer::SQL_TABLE_PLAYERS + " SET " +
                        MainServer::SQL_TOTPSECRET + " = ? WHERE " +
                        MainServer::SQL_USERNAME + " = ?;");
    updateQuery.addBindValue(base32Secret);
    updateQuery.addBindValue(username);
    updateQuery.exec();
    if (MainServer::sqlQueryFailed(updateQuery))
    {
        CONSOLE_PRINT("Unable to store totp secret for user " + username + ". Error: " + database.lastError().nativeErrorCode(), GameConsole::eERROR);
        return false;
    }
    return true;
}

bool TwoFactorAuthenticatorServer::clearTotpSecret(QSqlDatabase &database, const QString &username)
{
    return storeTotpSecret(database, username, "");
}