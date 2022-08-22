#include <QBuffer>
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/commandlineparser.h"

#include "game/gamemap.h"

const char* const DRIVER = "QSQLITE";

const char* const SQL_TABLE_PLAYERS = "players";
const char* const SQL_USERNAME = "username";
const char* const SQL_PASSWORD = "password";
const char* const SQL_MAILADRESS = "mailAdress";
const char* const SQL_MMR = "mmr";
const char* const SQL_VALIDPASSWORD = "validPassword";
const char* const SQL_LASTLOGIN = "lastLogin";

spMainServer MainServer::m_pInstance;

MainServer* MainServer::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = spMainServer::create();
    }
    return m_pInstance.get();
}

bool MainServer::exists()
{
    return m_pInstance.get() != nullptr;
}

void MainServer::release()
{
    m_pInstance = nullptr;
}

MainServer::MainServer()
    : QObject(),
      m_updateTimer(this),
      m_pGameServer(spTCPServer::create(this)),
      m_pSlaveServer(spTCPServer::create(this))
{
    CONSOLE_PRINT("Game server launched", Console::eDEBUG);
    Interpreter::setCppOwnerShip(this);
    m_updateTimer.setSingleShot(true);
    m_updateTimer.start(5000);
    moveToThread(Mainapp::getGameServerThread());
    // publish server to js environment for ai training
    QString javascriptName = "mainServer";
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(javascriptName, pInterpreter->newQObject(this));
    // connect signals for tcp server events
    connect(m_pGameServer.get(), &TCPServer::recieveData, this, &MainServer::recieveData, Qt::QueuedConnection);
    connect(m_pGameServer.get(), &TCPServer::sigConnected, this, &MainServer::playerJoined, Qt::QueuedConnection);
    connect(m_pGameServer.get(), &TCPServer::sigDisconnected, this, &MainServer::disconnected, Qt::QueuedConnection);
    // connect signals for tcp slave events
    connect(m_pSlaveServer.get(), &TCPServer::recieveData, this, &MainServer::receivedSlaveData, Qt::QueuedConnection);
    // internal updates
    connect(this, &MainServer::sigStartRemoteGame, this, &MainServer::slotStartRemoteGame, Qt::QueuedConnection);
    connect(&m_updateTimer, &QTimer::timeout, this, &MainServer::sendGameDataUpdate, Qt::QueuedConnection);
    parseSlaveAddressOptions();

    bool dataBaseLaunched = true;
    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        m_serverData = QSqlDatabase::addDatabase(DRIVER);
        QString path = Settings::getUserPath() + "/commanderWars.db";
        if (Settings::getUserPath().isEmpty())
        {
            path = QCoreApplication::applicationDirPath() + "/commanderWars.db";
        }
        m_serverData.setDatabaseName(path);
        if (!m_serverData.open())
        {
            CONSOLE_PRINT("Unable to open player error: " + m_serverData.lastError().text(), Console::eERROR);
            CONSOLE_PRINT("Unable to open player native error: " + m_serverData.lastError().nativeErrorCode(), Console::eERROR);
            dataBaseLaunched = false;
        }
        if (dataBaseLaunched)
        {
            QSqlQuery query = m_serverData.exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_PLAYERS + " (" +
                                                SQL_USERNAME + " TEXT PRIMARY KEY, " +
                                                SQL_PASSWORD + " TEXT, " +
                                                SQL_MAILADRESS + " TEXT, " +
                                                SQL_MMR + " INTEGER, " +
                                                SQL_VALIDPASSWORD + " INTEGER, " +
                                                SQL_LASTLOGIN + " TEXT)");
            if (sqlQueryFailed(query))
            {
                CONSOLE_PRINT("Unable to create player table " + m_serverData.lastError().nativeErrorCode(), Console::eERROR);
            }
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to detect sql driver. Server won't be started.", Console::eERROR);
        dataBaseLaunched = false;
    }
    if (dataBaseLaunched)
    {
        m_mailSender.moveToThread(&m_mailSenderThread);
        m_mailSenderThread.start();
        emit m_mailSender.sigConnectToServer();

        CONSOLE_PRINT("Starting tcp server and listening to new clients.", Console::eDEBUG);
        emit m_pGameServer->sig_connect(Settings::getServerListenAdress(), Settings::getServerPort(), Settings::getServerSecondaryListenAdress());
        emit m_pSlaveServer->sig_connect(Settings::getSlaveListenAdress(), Settings::getSlaveServerPort(), "");
    }
}

MainServer::~MainServer()
{
    if (m_mailSenderThread.isRunning())
    {
        emit m_mailSender.sigDisconnectFromServer();
        m_mailSenderThread.quit();
        m_mailSenderThread.wait();
        m_mailSender.moveToThread(QThread::currentThread());
    }
    m_pGameServer->disconnectTCP();
    // clean up server and client games.
    for (auto & game : m_games)
    {
        game->process->kill();
    }
    m_games.clear();
}

bool MainServer::sqlQueryFailed(const QSqlQuery & query)
{
    auto error = query.lastError();
    auto type = error.type();
    bool failed = !query.isActive() && type != QSqlError::NoError;
    if (failed)
    {
       CONSOLE_PRINT("Sql query failed with " + error.text(), Console::eERROR);
    }
    return failed;
}

void MainServer::parseSlaveAddressOptions()
{
    QStringList addressOptions = Settings::getSlaveHostOptions().split(";");
    if (addressOptions.size() == 0)
    {
        CONSOLE_PRINT("Slave host option: " + Settings::getSlaveHostOptions() + " contains no valid slave spawn options. Server will be unusable.", Console::eWARNING);
    }
    else
    {
        for (auto & option : addressOptions)
        {
            QStringList values = option.split("&");
            if (values.size() == 4)
            {
                bool ok = false;
                AddressInfo info;
                info.address = values[0];
                info.secondaryAddress = values[1];
                info.minPort = values[2].toUInt(&ok);
                if (info.minPort == 0)
                {
                    info.minPort = 2;
                }
                info.maxPort = values[3].toUInt(&ok);
                m_slaveAddressOptions.append(info);
                if (m_slaveAddressOptions.size() == 1)
                {
                    m_lastUsedPort = info.minPort -1;
                }
                CONSOLE_PRINT("Slave host option: " + info.address + " from port " + QString::number(info.minPort) + " to " + QString::number(info.maxPort), Console::eDEBUG);
            }
            else
            {
                CONSOLE_PRINT("Slave host option: " + option + " couldn't be parsed and will be ignored", Console::eWARNING);
            }
        }
    }
}

void MainServer::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Network json hosting client server command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::SERVERJOINGAME)
        {
            joinSlaveGame(socketID, objData);
        }
        else if (messageType == NetworkCommands::REQUESTPUBLICKEY)
        {
            QJsonObject objData = doc.object();
            auto & cypher = Mainapp::getInstance()->getCypher();
            auto action = static_cast<NetworkCommands::PublicKeyActions>(objData.value(JsonKeys::JSONKEY_RECEIVEACTION).toInt());
            emit m_pGameServer->sig_sendData(socketID, cypher.getPublicKeyMessage(action), NetworkInterface::NetworkSerives::ServerHostingJson, false);
        }
        else if (messageType == NetworkCommands::CRYPTEDMESSAGE)
        {
            handleCryptedMessage(socketID, doc);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTUSERGAMES)
        {
            onRequestUsergames(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTGAMES)
        {
            onRequestGameData(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Network hosting client server command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID, data);
        }        
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
}

void MainServer::receivedSlaveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHostingJson)
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject objData = doc.object();
        QString messageType = objData.value(JsonKeys::JSONKEY_COMMAND).toString();
        CONSOLE_PRINT("Network json hosting slave server command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::SLAVEREADY)
        {
            onSlaveReady(socketID, objData);
        }
        else  if (messageType == NetworkCommands::GAMERUNNINGONSERVER)
        {
            onGameRunningOnServer(socketID, objData);
        }
        else  if (messageType == NetworkCommands::SERVEROPENPLAYERCOUNT)
        {
            onOpenPlayerCount(socketID, objData);
        }
        else if (messageType == NetworkCommands::SLAVEGAMESTARTED)
        {
            onSlaveGameStarted(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
}

void MainServer::onSlaveReady(quint64 socketID, const QJsonObject & objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto iter = m_games.find(slaveName);
    if (iter != m_games.end())
    {
        auto & internGame = iter.value();
        internGame->game->onConnectToLocalServer(socketID, m_pSlaveServer);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), Console::eDEBUG);
    }
}

void MainServer::onGameRunningOnServer(quint64 socketID, const QJsonObject & objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto iter = m_games.find(slaveName);
    if (iter != m_games.end())
    {
        auto & internGame = iter.value();
        internGame->game->slaveRunning(objData, m_pGameServer);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), Console::eDEBUG);
    }
}

void MainServer::onSlaveGameStarted(quint64 socketID, const QJsonObject & objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto iter = m_games.find(slaveName);
    if (iter != m_games.end())
    {
        auto & internGame = iter.value();
        internGame->game->slaveGameStarted(objData);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), Console::eDEBUG);
    }
}

void MainServer::onRequestUsergames(quint64 socketId, const QJsonObject & objData)
{
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QString command = QString(NetworkCommands::SERVERUSERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonObject games;
    qint32 i = 0;
    for (const auto & game : qAsConst(m_games))
    {
        if (game->game.get() != nullptr &&
            game->game->getSlaveRunning())
        {
            auto & data = game->game->getData();
            if (data.getPlayerNames().contains(username))
            {
                if (data.getLaunched())
                {
                    QJsonObject obj = game->game->getData().toJson();
                    games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                    ++i;
                }
                else
                {
                    CONSOLE_PRINT("Found game which isn't started for username " + username, Console::eDEBUG);
                }
            }
        }
    }
    data.insert(JsonKeys::JSONKEY_GAMES, games);
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onRequestGameData(quint64 socketId, const QJsonObject & objData)
{
    sendGameDataToClient(socketId);
}

void MainServer::onOpenPlayerCount(quint64 socketID, const QJsonObject & objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    qint32 openPlayerCount = objData.value(JsonKeys::JSONKEY_OPENPLAYERCOUNT).toInteger();
    auto iter = m_games.find(slaveName);
    if (iter != m_games.end())
    {
        auto & internGame = iter.value();
        auto & data = internGame->game->getData();
        data.setPlayers(data.getMaxPlayers() - openPlayerCount);
        m_updateGameData = true;
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), Console::eDEBUG);
    }
}

void MainServer::joinSlaveGame(quint64 socketID, const QJsonObject & objData)
{
    bool found = false;
    QString slave = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    CONSOLE_PRINT("Searching for game " + slave + " for socket " + QString::number(socketID) + " to join game.", Console::eDEBUG);

    auto iter = m_games.find(slave);
    if (iter != m_games.end())
    {
        const auto & game = iter.value();
        // only send valid game data to clients
        if (game->game.get() != nullptr &&
            game->game->getServerName() == slave &&
            game->game->getSlaveRunning() &&
            game->game->getData().getPlayers() > 0)
        {
            // send data
            QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
            CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_ADDRESS, game->game->getData().getSlaveAddress());
            data.insert(JsonKeys::JSONKEY_SECONDARYADDRESS, game->game->getData().getSlaveSecondaryAddress());
            data.insert(JsonKeys::JSONKEY_PORT, static_cast<qint64>(game->game->getData().getSlavePort()));
            QJsonDocument doc(data);
            emit m_pGameServer->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
            found = true;
        }
    }
    if (!found)
    {
        CONSOLE_PRINT("Failed to find game " + slave + " for socket " + QString::number(socketID) + " to join game. Forcing a disconnection.", Console::eDEBUG);
        QString command = QString(NetworkCommands::SERVERGAMENOLONGERAVAILABLE);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void MainServer::startRemoteGame(const QString & initScript, const QString & id)
{
    emit sigStartRemoteGame(initScript, id);
}

void MainServer::slotStartRemoteGame(QString initScript, QString id)
{
    QByteArray sendData;
    spawnSlave(initScript, Settings::getMods(), id, 0, sendData);
}

void MainServer::disconnected(qint64 socketId)
{
    // check if we have a slave in start up state associated with this socket
    for (auto & game : qAsConst(m_games))
    {
        if (game->game->getData().getPlayers() == 0 &&
            game->game->getHostingSocket() == socketId)
        {
            game->game->startCloseTimer();
            break;
        }
    }
}

void MainServer::spawnSlaveGame(QDataStream & stream, quint64 socketID, QByteArray& data, QString initScript, QString id)
{
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    if (validHostRequest(mods))
    {
        spawnSlave(initScript, mods, id, socketID, data);
    }
    else
    {
        CONSOLE_PRINT("Requested invalid mod configuration.", Console::eDEBUG);
        QString command = QString(NetworkCommands::SERVERINVALIDMODCONFIG);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void MainServer::spawnSlave(const QString & initScript, const QStringList & mods, QString id, quint64 socketID, QByteArray& data)
{
    m_slaveGameIterator++;

    QString slaveAddress;
    QString slaveSecondaryAddress;
    quint16 slavePort;
    if (getNextFreeSlaveAddress(slaveAddress, slavePort, slaveSecondaryAddress))
    {
        bool createLogs = Mainapp::getInstance()->getCreateSlaveLogs();
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
        CONSOLE_PRINT("Launching slave game " + slaveName + " creating logs: " + (createLogs ? "true" : "false"), Console::eDEBUG);
        auto game = spInternNetworkGame::create();
        QString program = QCoreApplication::applicationFilePath();
        game->process = std::make_shared<QProcess>();
        game->process->setObjectName(slaveName + "Slaveprocess");
        const char* const prefix = "--";
        QStringList args({QString(prefix) + CommandLineParser::ARG_SLAVE,
                          QString(prefix) + CommandLineParser::ARG_SLAVENAME,
                          slaveName,
                          QString(prefix) + CommandLineParser::ARG_NOUI, // comment out for debugging
                          QString(prefix) + CommandLineParser::ARG_NOAUDIO,
                          QString(prefix) + CommandLineParser::ARG_MODS,
                          Settings::getConfigString(mods),
                          QString(prefix) + CommandLineParser::ARG_SLAVEADDRESS,
                          slaveAddress,
                          QString(prefix) + CommandLineParser::ARG_SLAVESECONDARYADDRESS,
                          slaveSecondaryAddress,
                          QString(prefix) + CommandLineParser::ARG_SLAVEPORT,
                          QString::number(slavePort),
                          QString(prefix) + CommandLineParser::ARG_MASTERADDRESS,
                          Settings::getSlaveListenAdress(),
                          QString(prefix) + CommandLineParser::ARG_MASTERPORT,
                          QString::number(Settings::getSlaveServerPort()),
                          QString(prefix) + CommandLineParser::ARG_INITSCRIPT,
                          initScript});
        if (createLogs)
        {
            args << QString(prefix) + CommandLineParser::ARG_CREATESLAVELOGS;
        }
        game->game = spNetworkGame::create(nullptr);
        game->game->setDataBuffer(data);
        game->game->setServerName(slaveName);
        game->game->setHostingSocket(socketID);
        game->game->getData().setSlaveAddress(slaveAddress);
        game->game->getData().setSlaveSecondaryAddress(slaveSecondaryAddress);
        game->game->getData().setSlavePort(slavePort);
        connect(game->process.get(), QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), game->game.get(), &NetworkGame::processFinished, Qt::QueuedConnection);
        connect(game->game.get(), &NetworkGame::sigDataChanged, this, &MainServer::updateGameData, Qt::QueuedConnection);
        connect(game->game.get(), &NetworkGame::sigClose, this, &MainServer::closeGame, Qt::QueuedConnection);
        game->process->start(program, args);
        game->game->setId(id);
        m_games.insert(slaveName, game);
    }
    else
    {
        CONSOLE_PRINT("No free slots available.", Console::eDEBUG);
        QString command = QString(NetworkCommands::SERVERNOGAMESLOTSAVAILABLE);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

bool MainServer::validHostRequest(QStringList mods)
{
    // make sure the server has the requested mods installed.
    for (auto & mod : mods)
    {
        QFile file(mod + "/mod.txt");
        if (!file.exists())
        {
            return false;
        }
    }
    return true;
}

void MainServer::updateGameData()
{
    m_updateGameData = true;
}

void MainServer::sendGameDataUpdate()
{
    if (m_updateGameData)
    {
        m_updateGameData = false;
        sendGameDataToClient(0);
    }
    m_updateTimer.start(5000);
}

void MainServer::playerJoined(qint64 socketId)
{
    CONSOLE_PRINT("Player joined the server " + QString::number(socketId), Console::eDEBUG);
    emit m_pGameServer->sigSetIsActive(socketId, false);
}

void MainServer::sendGameDataToClient(qint64 socketId)
{
    QString command = QString(NetworkCommands::SERVERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonObject games;
    qint32 i = 0;
    for (const auto & game : qAsConst(m_games))
    {
        if (game->game.get() != nullptr &&
            game->game->getSlaveRunning() &&
            !game->game->getData().getLaunched())
        {
            QJsonObject obj = game->game->getData().toJson();
            games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
            ++i;
        }
    }
    data.insert(JsonKeys::JSONKEY_GAMES, games);
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::closeGame(NetworkGame* pGame)
{
    CONSOLE_PRINT("Despawning games", Console::eDEBUG);
    QString slaveName = pGame->getServerName();
    auto iter = m_games.find(slaveName);
    if (iter != m_games.end())
    {
        const auto & game = iter.value();
        SlaveAddress freeAddress;
        freeAddress.address = game->game->getData().getSlaveAddress();
        freeAddress.secondaryAddress = game->game->getData().getSlaveSecondaryAddress();
        freeAddress.port = game->game->getData().getSlavePort();
        m_freeAddresses.append(freeAddress);
        game->process->kill();
        game->game = nullptr;
        m_games.remove(slaveName);
        m_updateGameData = true;
    }
}

bool MainServer::getNextFreeSlaveAddress(QString & address, quint16 & port, QString & secondaryAddress)
{
    bool success = false;
    if (m_freeAddresses.size() > 0)
    {
        auto & newAddress = m_freeAddresses.constLast();
        address = newAddress.address;
        secondaryAddress = newAddress.secondaryAddress;
        port = newAddress.port;
        m_freeAddresses.removeLast();
        success = true;
    }
    else
    {
        if (m_lastUsedAddressIndex < m_slaveAddressOptions.size())
        {
            auto & options = m_slaveAddressOptions[m_lastUsedAddressIndex];
            if (m_lastUsedPort < options.maxPort)
            {
                address = options.address;
                secondaryAddress = options.secondaryAddress;
                ++m_lastUsedPort;
                port = m_lastUsedPort;
                success = true;
            }
            else
            {
                m_lastUsedAddressIndex++;
                if (m_lastUsedAddressIndex < m_slaveAddressOptions.size())
                {
                    m_lastUsedPort = m_slaveAddressOptions[m_lastUsedAddressIndex].minPort - 1;
                    success = getNextFreeSlaveAddress(address, port, secondaryAddress);
                }
            }
        }
    }
    return success;
}

void MainServer::handleCryptedMessage(qint64 socketId, const QJsonDocument & doc)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QJsonObject objData = doc.object();
    QJsonDocument decryptedDoc = QJsonDocument::fromJson(cypher.getDecryptedMessage(doc));
    auto action = static_cast<NetworkCommands::PublicKeyActions>(objData.value(JsonKeys::JSONKEY_RECEIVEACTION).toInt());
    CONSOLE_PRINT("Handling crypted message action " + QString::number(static_cast<qint32>(action)) + " received", Console::eDEBUG);
    switch (action)
    {
        case NetworkCommands::PublicKeyActions::LoginAccount:
        {
            loginToAccount(socketId, decryptedDoc, action);
            break;
        }
        case NetworkCommands::PublicKeyActions::CreateAccount:
        {
            createAccount(socketId, decryptedDoc, action);
            break;
        }
        case NetworkCommands::PublicKeyActions::ResetPassword:
        {
            resetAccountPassword(socketId, decryptedDoc, action);
            break;
        }
        case NetworkCommands::PublicKeyActions::ChangePassword:
        {
            changeAccountPassword(socketId, decryptedDoc, action);
            break;
        }
        default:
        {
            CONSOLE_PRINT("Unknown crypted message action " + QString::number(static_cast<qint32>(action)) + " received", Console::eDEBUG);
            break;
        }
    }
}

void MainServer::createAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QJsonObject data = doc.object();
    QByteArray password = cypher.toByteArray(data.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    QString mailAdress = data.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = data.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Creating account with username " + username + " and email adress " + mailAdress, Console::eDEBUG);
    bool success = false;
    QSqlQuery query = getAccountInfo(m_serverData, username, success);
    GameEnums::LoginError result = GameEnums::LoginError_None;
    if (!query.first() && success)
    {
        auto hexPassword = password.toHex();
        QString dateTime = QDateTime::currentDateTimeUtc().toString();
        QString command = QString("INSERT INTO ") + SQL_TABLE_PLAYERS + "(" +
                          SQL_USERNAME + ", " +
                          SQL_PASSWORD + ", " +
                          SQL_MAILADRESS + ", " +
                          SQL_MMR + ", " +
                          SQL_VALIDPASSWORD + ", " +
                          SQL_LASTLOGIN +
                          ") VALUES(" +
                          "'" + username + "'," +
                          "'" + hexPassword + "'," +
                          "'" + mailAdress + "'," +
                          "750," +
                          "1," +
                          "'" + dateTime + "'" +
                          ")";
        query = m_serverData.exec(command);
        if (sqlQueryFailed(query))
        {
            result = GameEnums::LoginError_AccountExists;
        }
    }
    else
    {
        CONSOLE_PRINT("Username is already existing." , Console::eDEBUG);
        result = GameEnums::LoginError_AccountExists;
    }
    QString command = QString(NetworkCommands::SERVERACCOUNTMESSAGE);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), Console::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    outData.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::loginToAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QJsonObject data = doc.object();
    QByteArray password = cypher.toByteArray(data.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    QString username = data.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Login to account with username " + username, Console::eDEBUG);
    auto result = checkPassword(m_serverData, username, password);
    if (result == GameEnums::LoginError_None)
    {
        // mark client as logged in
        emit m_pGameServer->sigSetIsActive(socketId, true);
    }
    QString command = QString(NetworkCommands::SERVERACCOUNTMESSAGE);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), Console::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    outData.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    if (result == GameEnums::LoginError_None)
    {
        sendGameDataToClient(socketId);
    }
}

GameEnums::LoginError MainServer::checkPassword(QSqlDatabase & database, const QString & username, const QByteArray & password)
{
    bool success = false;
    QSqlQuery query = getAccountInfo(database, username, success);
    GameEnums::LoginError result = GameEnums::LoginError_None;
    if (query.first() && success)
    {
        CONSOLE_PRINT("Checking password for : " + username, Console::eDEBUG);
        auto dbPassword = query.value(SQL_PASSWORD);
        auto outdatedPassword = query.value(SQL_VALIDPASSWORD).toInt();
        if (dbPassword.toByteArray() != password.toHex())
        {
            result = GameEnums::LoginError_WrongPassword;
        }
        if (outdatedPassword == 0)
        {
            CONSOLE_PRINT("Account info for : " + username + " is out dated.", Console::eDEBUG);
            result = GameEnums::LoginError_PasswordOutdated;
        }
    }
    else
    {
        CONSOLE_PRINT("Account info for : " + username + " not found.", Console::eDEBUG);
        result = GameEnums::LoginError_AccountDoesntExist;
    }
    return result;
}

GameEnums::LoginError MainServer::verifyLoginData(const QString & username, const QByteArray & password)
{
    GameEnums::LoginError valid = GameEnums::LoginError_DatabaseNotAccesible;
    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        QSqlDatabase serverData = QSqlDatabase::addDatabase(DRIVER);
        QString path = Settings::getUserPath() + "/commanderWars.db";
        if (Settings::getUserPath().isEmpty())
        {
            path = QCoreApplication::applicationDirPath() + "/commanderWars.db";
        }
        serverData.setDatabaseName(path);
        if (!serverData.open())
        {
            CONSOLE_PRINT("Unable to open player error: " + serverData.lastError().text(), Console::eERROR);
            CONSOLE_PRINT("Unable to open player native error: " + serverData.lastError().nativeErrorCode(), Console::eERROR);
        }
        else
        {
            valid = checkPassword(serverData, username, password);
        }
    }
    return valid;
}

void MainServer::resetAccountPassword(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action)
{
    QJsonObject data = doc.object();
    QString mailAdress = data.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = data.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Resetting account with username " + username + " and email adress " + mailAdress, Console::eDEBUG);
    bool success = false;
    QSqlQuery query = getAccountInfo(m_serverData, username, success);
    GameEnums::LoginError result = GameEnums::LoginError_None;
    if (query.first() && success)
    {
        auto dbMailAdress = query.value(SQL_MAILADRESS);
        if (dbMailAdress.toString() == mailAdress)
        {
            QString newPassword = createRandomPassword();
            Password password;
            password.setPassword(newPassword);
            QString message = "Hello " + username + "\n\n" +
                              "You're password has been reset. \n" +
                              "You're new password is: " + newPassword + "\n\n" +
                              "You have to change your password \n" +
                              "once you logged into the server again\n\n" +
                              "Kind regards\nThe server crew";
            auto changeQuery = m_serverData.exec(QString("UPDATE ") + SQL_TABLE_PLAYERS + " SET " +
                                            SQL_PASSWORD + " = " + "'" + password.getHash().toHex() + "', " +
                                            SQL_VALIDPASSWORD + " = 0 WHERE " +
                                            SQL_USERNAME + " = '" + username + "';");
            if (!sqlQueryFailed(changeQuery))
            {
                emit m_mailSender.sigSendMail(0, "Commander Wars password reset", message, mailAdress, username);
            }
            else
            {
                result = GameEnums::LoginError_InvalidPasswordReset;
            }
        }
        else
        {
            result = GameEnums::LoginError_InvalidPasswordReset;
        }
    }
    else
    {
        result = GameEnums::LoginError_AccountDoesntExist;
    }
    QString command = QString(NetworkCommands::SERVERACCOUNTMESSAGE);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), Console::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    outData.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::changeAccountPassword(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action)
{
    auto & cypher = Mainapp::getInstance()->getCypher();
    QJsonObject data = doc.object();
    QByteArray oldPassword = cypher.toByteArray(data.value(JsonKeys::JSONKEY_OLDPASSWORD).toArray());
    QByteArray password = cypher.toByteArray(data.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    bool success = false;
    QString username = data.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Login to account with username " + username, Console::eDEBUG);
    QSqlQuery query = getAccountInfo(m_serverData, username, success);
    GameEnums::LoginError result = GameEnums::LoginError_None;
    if (query.first() && success)
    {
        auto dbPassword = query.value(SQL_PASSWORD);
        auto outdatedPassword = query.value(SQL_VALIDPASSWORD).toInt();
        if (dbPassword.toByteArray() != oldPassword.toHex())
        {
            result = GameEnums::LoginError_WrongPassword;
        }
        else
        {
            auto changeQuery = m_serverData.exec(QString("UPDATE ") + SQL_TABLE_PLAYERS + " SET " +
                                            SQL_PASSWORD + " = " + "'" + password.toHex() + "', " +
                                            SQL_VALIDPASSWORD + " = 1 WHERE " +
                                            SQL_USERNAME + " = '" + username + "';");
            if (sqlQueryFailed(changeQuery))
            {
                result = GameEnums::LoginError_InvalidPasswordReset;
            }
            else
            {
                if (outdatedPassword == 0)
                {
                    // mark client as logged in
                    emit m_pGameServer->sigSetIsActive(socketId, true);
                    sendGameDataToClient(socketId);
                }
            }
        }
    }
    else
    {
        result = GameEnums::LoginError_AccountDoesntExist;
    }
    QString command = QString(NetworkCommands::SERVERACCOUNTMESSAGE);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), Console::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    outData.insert(JsonKeys::JSONKEY_RECEIVEACTION, static_cast<qint32>(action));
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

QString MainServer::createRandomPassword() const
{
    QString password;
    QVector<char> specialChars = {'#', '?', '!', '@', '$', '%', '^', '&', '*', '-'};
    auto specialCharPos = GlobalUtils::randInt(0, 7);
    auto numberCharPos = GlobalUtils::randInt(0, 7);
    while (numberCharPos == specialCharPos)
    {
        numberCharPos = GlobalUtils::randInt(0, 7);
    }
    bool smallLetter = false;
    bool capitalLetter = false;
    for (qint32 i = 0; i < 8; ++i)
    {
        if (i == specialCharPos)
        {
            password += specialChars[GlobalUtils::randInt(0, specialChars.length() - 1)];
        }
        else if (i == numberCharPos)
        {
            password += static_cast<char>(GlobalUtils::randInt('0', '9'));
        }
        else if (GlobalUtils::randInt(0, 1) == 1)
        {
            password += static_cast<char>(GlobalUtils::randInt('A', 'Z'));
            capitalLetter = true;
        }
        else
        {
            password += static_cast<char>(GlobalUtils::randInt('a', 'z'));
            smallLetter = true;
        }
    }
    if (!smallLetter)
    {
        password += static_cast<char>(GlobalUtils::randInt('a', 'z'));
    }
    if (!capitalLetter)
    {
        password += static_cast<char>(GlobalUtils::randInt('A', 'Z'));
    }
    return password;
}

QSqlQuery MainServer::getAccountInfo(QSqlDatabase & database, const QString & username, bool & success)
{
    QSqlQuery query = database.exec(QString("SELECT ") +
                                    SQL_USERNAME + ", " +
                                    SQL_PASSWORD + ", " +
                                    SQL_MAILADRESS + ", " +
                                    SQL_MMR + ", " +
                                    SQL_VALIDPASSWORD + ", " +
                                    SQL_LASTLOGIN +
                                    " from " + SQL_TABLE_PLAYERS +
                                    " WHERE " + SQL_USERNAME +
                                    " = '" + username + "';");
    success = !sqlQueryFailed(query);
    return query;
}

