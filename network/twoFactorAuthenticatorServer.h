#pragma once

#include <QString>
#include <QDateTime>
#include <QSqlDatabase>

class MainServer;

class TwoFactorAuthenticatorServer
{
public:
    /**
     * @brief The TotpEnrollment struct a pending 2 factor authentication setup, confirmed once the user enters a valid totp code
     */
    struct TotpEnrollment
    {
        QString username;
        QString base32Secret;
        QDateTime created;
    };
    /**
     * @brief The PasswordResetSession struct a pending totp based password reset of one client
     */
    struct PasswordResetSession
    {
        QString username;
        QDateTime created;
        qint32 attempts{0};
    };

    TwoFactorAuthenticatorServer(MainServer & mainServer);
    virtual ~TwoFactorAuthenticatorServer() = default;
    void disconnectClient(qint64 socketId);
    /**
     * @brief start2faSetup starts the optional 2fa enrollment for the logged in account of the given socket
     * @param socketId
     * @param objData
     */
    void start2faSetup(qint64 socketId, const QJsonObject & objData);
    /**
     * @brief confirm2faSetup confirms a pending 2fa enrollment with a totp code of the user's app
     * @param socketId
     * @param objData
     */
    void confirm2faSetup(qint64 socketId, const QJsonObject & objData);
    /**
     * @brief cancel2fa cancels a pending 2fa enrollment and a pending password reset of the socket
     * @param socketId
     * @param objData
     */
    void cancel2fa(qint64 socketId, const QJsonObject & objData);
    /**
     * @brief startPasswordReset starts the password reset workflow, totp based for accounts with 2fa, mail based else
     * @param socketId
     * @param objData
     */
    void startPasswordReset(qint64 socketId, const QJsonObject & objData);
    /**
     * @brief submitPasswordReset2faCode checks the entered totp code of a pending password reset and resets the password on success
     * @param socketId
     * @param objData
     */
    void submitPasswordReset2faCode(qint64 socketId, const QJsonObject & objData);
    /**
     * @brief send2faResponse sends a json response for a 2fa command
     * @param socketId
     * @param command response command name
     * @param result error code of the operation
     * @param additionalData optional additional json values (e.g. secret, url, new password)
     */
    void send2faResponse(qint64 socketId, const QString & command, GameEnums::LoginError result, const QJsonObject & additionalData = QJsonObject());
    /**
     * @brief cleanUpExpired2faSessions removes expired 2fa enrollments and password reset sessions and informs affected clients
     */
    void cleanUpExpired2faSessions();
    /**
     * @brief migrateAddTotpDatabase adds the totp_secret column to the players table if it does not exist yet
     * @param database
     */
    static void migrateAddTotpDatabase(QSqlDatabase &database);
    /**
     * @brief hasTotpSecret checks if a totp secret for 2 factor authentication is stored for the account
     * @param database
     * @param username
     * @return true if a non-empty totp secret exists
     */
    static bool hasTotpSecret(QSqlDatabase & database, const QString & username);
    /**
     * @brief storeTotpSecret stores the base32 encoded totp secret for the account
     * @param database
     * @param username
     * @param base32Secret base32 encoded totp secret
     * @return true if the secret was stored
     */
    static bool storeTotpSecret(QSqlDatabase & database, const QString & username, const QString & base32Secret);
    /**
     * @brief clearTotpSecret removes the totp secret of the account
     * @param database
     * @param username
     * @return true if the secret was removed
     */
    static bool clearTotpSecret(QSqlDatabase & database, const QString & username);
    /**
     * @brief isPasswordResetAllowed checks if a password reset is allowed for the account (not locked due to too many failed attempts)
     */
    static bool isPasswordResetAllowed(QSqlDatabase &database, QSqlQuery &accountInfo);
    /**
     * @brief resetLoginAttempts resets the login attempts for the account
     */
    static void resetLoginAttempts(QSqlDatabase &database, QSqlQuery &accountInfo);
    /**
     * @brief handleLoginFailed handles a failed login attempt for the account, increasing the fail count and locking the account if necessary
     */
    static void handleLoginFailed(QSqlDatabase &database, QSqlQuery &accountInfo);
private:
    /**
     * @brief m_pending2faSetups pending 2fa enrollments per client socket
     */
    QHash<quint64, TotpEnrollment> m_pending2faSetups;
    /**
     * @brief m_passwordResetSessions pending totp password resets per client socket
     */
    QHash<quint64, PasswordResetSession> m_passwordResetSessions;
    /**
     * @brief m_mainServer reference to the main server for database access and sending responses
     */
    MainServer & m_mainServer;
};