#include <QBuffer>
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
#include "coreengine/globalutils.h"
#include "coreengine/workerthread.h"
#include "multiplayer/password.h"

#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

const char *const DRIVER = "QSQLITE";

const char *const MainServer::SQL_TABLE_PLAYERS = "players";
const char *const MainServer::SQL_USERNAME = "username";
const char *const MainServer::SQL_PASSWORD = "password";
const char *const MainServer::SQL_MAILADRESS = "mailAdress";
const char *const MainServer::SQL_VALIDPASSWORD = "validPassword";
const char *const MainServer::SQL_LASTLOGIN = "lastLogin";

const char *const MainServer::SQL_TABLE_PLAYERDATA = "playerData";
const char *const MainServer::SQL_COID = "coid";
const char *const MainServer::SQL_GAMESMADE = "gamesMade";
const char *const MainServer::SQL_GAMESLOST = "gamesLost";
const char *const MainServer::SQL_GAMESWON = "gamesWon";
const char *const MainServer::SQL_GAMESDRAW = "gamesDraw";
const char *const MainServer::SQL_METADATA = "metaData";

const char *const MainServer::SQL_TABLE_MATCH_DATA = "matchData";
const char *const MainServer::SQL_MMR = "mmr";
const char *const MainServer::SQL_MINGAMES = "minGames";
const char *const MainServer::SQL_MAXGAMES = "maxGames";
const char *const MainServer::SQL_RUNNINGGAMES = "runningGames";
const char *const MainServer::SQL_MATCHHISTORY = "matchHistory";
const char *const MainServer::SQL_SIGNEDUP = "signedUp";

const char *const MainServer::SQL_TABLE_DOWNLOADMAPINFO = "downloadMapInfo";
const char *const MainServer::SQL_MAPAUTHOR = "mapAuthor";
const char *const MainServer::SQL_MAPNAME = "mapName";
const char *const MainServer::SQL_MAPPATH = "mapPath";
const char *const MainServer::SQL_MAPIMAGEPATH = "mapImagePath";
const char *const MainServer::SQL_MAPPLAYERS = "mapPlayers";
const char *const MainServer::SQL_MAPWIDTH = "mapWidth";
const char *const MainServer::SQL_MAPHEIGHT = "mapHeight";
const char *const MainServer::SQL_MAPFLAGS = "mapFlags";
const char *const MainServer::SQL_MAPUPLOADER = "mapUploader";
const char *const MainServer::SQL_MAPDOWNLOADCOUNT = "mapDownloadCount";
const char *const MainServer::SQL_MAPUPLOADDATE = "mapUploadDate";
const char *const MainServer::SQL_MAPLASTDOWNLOADDATE = "mapLastDownloadDate";

const char *const MainServer::SQL_TABLE_REPLAYINFO = "replayInfo";
const char *const MainServer::SQL_REPLAYPATH = "replayPath";
const char *const MainServer::SQL_REPLAYCREATIONTIME = "replayCreationTime";

spMainServer MainServer::m_pInstance{nullptr};
QSqlDatabase *MainServer::m_serverData{nullptr};

MainServer *MainServer::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = MemoryManagement::create<MainServer>();
    }
    return m_pInstance.get();
}

bool MainServer::exists()
{
    return m_pInstance.get() != nullptr;
}

void MainServer::initDatabase()
{
    if (QSqlDatabase::isDriverAvailable(DRIVER) && m_serverData == nullptr)
    {
        static QSqlDatabase database = QSqlDatabase::addDatabase(DRIVER);
        m_serverData = &database;
        QString path = Settings::getInstance()->getUserPath() + "/commanderWars.db";
        if (Settings::getInstance()->getUserPath().isEmpty())
        {
            path = QCoreApplication::applicationDirPath() + "/commanderWars.db";
        }
        m_serverData->setDatabaseName(path);
        if (!m_serverData->open())
        {
            CONSOLE_PRINT("Unable to open player error: " + m_serverData->lastError().text(), GameConsole::eERROR);
            CONSOLE_PRINT("Unable to open player native error: " + m_serverData->lastError().nativeErrorCode(), GameConsole::eERROR);
            m_serverData = nullptr;
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to detect sql driver. Server won't be started.", GameConsole::eERROR);
    }
}

void MainServer::release()
{
    m_pInstance.reset();
}

MainServer::MainServer()
    : QObject(),
    m_periodicExecutionTimer(this),
    m_pGameServer(MemoryManagement::create<TCPServer>(this)),
    m_pSlaveServer(MemoryManagement::create<TCPServer>(this)),
    m_matchMakingCoordinator(this),
    m_mapFileServer(this),
    m_replayRecordFileserver(this),
    m_mailSenderThread(this)
{
    CONSOLE_PRINT("Game server launched", GameConsole::eDEBUG);
#ifdef GRAPHICSUPPORT
    setObjectName("MainServer");
#endif
    m_mailSenderThread.setObjectName("Mailserverthread");
    Interpreter::setCppOwnerShip(this);
    m_periodicExecutionTimer.setSingleShot(false);
    m_periodicExecutionTimer.start(30 * 1000);
    // publish server to js environment for ai training
    QString javascriptName = "mainServer";
    Interpreter *pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(javascriptName, pInterpreter->newQObject(this));
    Mainapp *pApp = Mainapp::getInstance();
    // connect signals for tcp server events
    connect(m_pGameServer.get(), &TCPServer::recieveData, this, &MainServer::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
    connect(m_pGameServer.get(), &TCPServer::sigConnected, this, &MainServer::playerJoined, Qt::QueuedConnection);
    connect(m_pGameServer.get(), &TCPServer::sigDisconnected, this, &MainServer::disconnected, Qt::QueuedConnection);
    // connect signals for tcp slave events
    connect(m_pSlaveServer.get(), &TCPServer::recieveData, this, &MainServer::receivedSlaveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
    // internal updates
    connect(this, &MainServer::sigStartRemoteGame, this, &MainServer::slotStartRemoteGame, Qt::QueuedConnection);
    connect(&m_periodicExecutionTimer, &QTimer::timeout, this, &MainServer::periodicTasks, Qt::QueuedConnection);
    parseSlaveAddressOptions();

    initDatabase();

    if (m_serverData != nullptr)
    {
        startDatabase();
        m_mailSender.moveToThread(&m_mailSenderThread);
        m_mailSenderThread.start(QThread::Priority::NormalPriority);
        connect(&m_mailSender, &SmtpMailSender::sigMailResult, this, &MainServer::onMailSendResult, Qt::QueuedConnection);
        restoreServer();
        CONSOLE_PRINT("Starting tcp server and listening to clients.", GameConsole::eDEBUG);
        emit m_pGameServer->sig_connect(Settings::getInstance()->getServerListenAdress(), Settings::getInstance()->getServerPort(), Settings::getInstance()->getServerSecondaryListenAdress());
        emit m_pSlaveServer->sig_connect(Settings::getInstance()->getSlaveListenAdress(), Settings::getInstance()->getSlaveServerPort(), "");
    }
    moveToThread(Mainapp::getWorkerthread());
}

MainServer::~MainServer()
{
    if (m_mailSenderThread.isRunning())
    {
        emit m_mailSender.sigMoveToThread(QThread::currentThread());
        m_mailSenderThread.quit();
        while (!m_mailSenderThread.wait(1))
        {
            QCoreApplication::processEvents();
        }
    }
    m_pGameServer->disconnectTCP();
    // clean up server and client games.
    for (auto &game : m_games)
    {
        game->process->kill();
    }
    m_games.clear();
}

void MainServer::startDatabase()
{
    // create primary table for user data
    QSqlQuery query = m_serverData->exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_PLAYERS + " (" +
                                         SQL_USERNAME + " TEXT PRIMARY KEY, " +
                                         SQL_PASSWORD + " TEXT, " +
                                         SQL_MAILADRESS + " TEXT, " +
                                         SQL_MMR + " INTEGER, " +
                                         SQL_VALIDPASSWORD + " INTEGER, " +
                                         SQL_LASTLOGIN + " TEXT)");
    if (sqlQueryFailed(query))
    {
        CONSOLE_PRINT("Unable to create player table error: " + m_serverData->lastError().nativeErrorCode(), GameConsole::eERROR);
    }
    // create table for map file server
    query = m_serverData->exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_DOWNLOADMAPINFO + " (" +
                               SQL_MAPPATH + " TEXT PRIMARY KEY, " +
                               SQL_MAPAUTHOR + " TEXT, " +
                               SQL_MAPNAME + " TEXT, " +
                               SQL_MAPIMAGEPATH + " TEXT, " +
                               SQL_MAPPLAYERS + " INTEGER, " +
                               SQL_MAPWIDTH + " INTEGER, " +
                               SQL_MAPHEIGHT + " INTEGER, " +
                               SQL_MAPFLAGS + " BIGINT," +
                               SQL_MAPUPLOADER + " TEXT, " +
                               SQL_MAPDOWNLOADCOUNT + " INTEGER, " +
                               SQL_MAPUPLOADDATE + " TEXT, " +
                               SQL_METADATA + " TEXT, " +
                               SQL_MAPLASTDOWNLOADDATE + " TEXT)");    
    // create table for record file server
    query = m_serverData->exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_REPLAYINFO + " (" +
                               SQL_REPLAYPATH + " TEXT PRIMARY KEY, " +
                               SQL_REPLAYCREATIONTIME + " BIGINT" +
                               SQL_METADATA + " TEXT, " +
                               SQL_MAPPLAYERS + " INTEGER, " +
                               SQL_MAPWIDTH + " INTEGER, " +
                               SQL_MAPHEIGHT + " INTEGER, " +
                               SQL_MAPFLAGS + " BIGINT," +
                               SQL_MAPAUTHOR + " TEXT" +
                               + ")");
    if (sqlQueryFailed(query))
    {
        CONSOLE_PRINT("Unable to create map table: " + m_serverData->lastError().nativeErrorCode(), GameConsole::eERROR);
    }
    bool success = false;
    query = getAllUsers(*m_serverData, success);
    if (success)
    {
        if (query.first())
        {
            auto username = query.value(SQL_USERNAME);
            createUserTable(username.toString());
            while (query.next())
            {
                auto username = query.value(SQL_USERNAME);
                createUserTable(username.toString());
            }
        }
    }
}

void MainServer::restoreServer()
{
    auto savefile = Mainapp::getInstance()->getParser().getServerSaveFile();
    if (QFile::exists(savefile))
    {
        QFile file(savefile);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        file.open(QIODevice::ReadOnly);
        deserializeObject(stream);
    }
}

bool MainServer::sqlQueryFailed(const QSqlQuery &query)
{
    auto error = query.lastError();
    auto type = error.type();
    bool failed = !query.isActive() && type != QSqlError::NoError;
    if (failed)
    {
        CONSOLE_PRINT("Sql query failed with " + error.text() + " Query: " + query.lastQuery(), GameConsole::eERROR);
    }
    return failed;
}

void MainServer::parseSlaveAddressOptions()
{
    QStringList addressOptions = Settings::getInstance()->getSlaveHostOptions().split(";");
    if (addressOptions.size() == 0)
    {
        CONSOLE_PRINT("Slave host option: " + Settings::getInstance()->getSlaveHostOptions() + " contains no valid slave spawn options. Server will be unusable.", GameConsole::eWARNING);
    }
    else
    {
        for (auto &option : addressOptions)
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
                    m_lastUsedPort = info.minPort - 1;
                }
                CONSOLE_PRINT("Slave host option: " + info.address + " from port " + QString::number(info.minPort) + " to " + QString::number(info.maxPort), GameConsole::eDEBUG);
            }
            else
            {
                CONSOLE_PRINT("Slave host option: " + option + " couldn't be parsed and will be ignored", GameConsole::eWARNING);
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
        CONSOLE_PRINT("Network json hosting client server command received: " + messageType, GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SERVERJOINGAME)
        {
            joinSlaveGame(socketID, objData);
        }
        else if (messageType == NetworkCommands::LOGINACCOUNT)
        {
            loginToAccount(socketID, objData);
        }
        else if (messageType == NetworkCommands::CREATEACCOUNT)
        {
            createAccount(socketID, objData);
        }
        else if (messageType == NetworkCommands::RESETPASSWORD)
        {
            resetAccountPassword(socketID, objData);
        }
        else if (messageType == NetworkCommands::CHANGEPASSWORD)
        {
            changeAccountPassword(socketID, objData);
        }
        else if (messageType == NetworkCommands::DELETEACCOUNT)
        {
            deleteAccount(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTUSERGAMES)
        {
            onRequestUsergames(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTOBSERVEGAMES)
        {
            onRequestObservegames(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTGAMES)
        {
            onRequestGameData(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTVERSION)
        {
            onRequestServerVersion(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTAUTOMATCHINFO)
        {
            onRequestServerAutoMatchInfo(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTUSERNAMES)
        {
            onRequestPlayersFromServer(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERREQUESTPLAYERSTATS)
        {
            onRequestPlayerStats(socketID, objData);
        }
        else if (messageType == NetworkCommands::MAPUPLOAD)
        {
            m_mapFileServer.onMapUpload(socketID, objData);
        }
        else if (messageType == NetworkCommands::MAPFILEDOWNLOADREQUEST)
        {
            m_mapFileServer.onRequestDownloadMap(socketID, objData);
        }
        else if (messageType == NetworkCommands::REQUESTAVAILABLEMAPS)
        {
            m_mapFileServer.onRequestFilteredMaps(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVERDELETEMAPFILE)
        {
            m_mapFileServer.onRequestDeleteMap(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command in MainServer::recieveData " + messageType + " received", GameConsole::eDEBUG);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Network hosting client server command received: " + messageType, GameConsole::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID, data);
        }
        else
        {
            CONSOLE_PRINT("Unknown command in MainServer::recieveData " + messageType + " received", GameConsole::eDEBUG);
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
        CONSOLE_PRINT("Network json hosting slave server command received: " + messageType, GameConsole::eDEBUG);
        if (messageType == NetworkCommands::SLAVEREADY)
        {
            onSlaveReady(socketID, objData);
        }
        else if (messageType == NetworkCommands::GAMERUNNINGONSERVER)
        {
            onGameRunningOnServer(socketID, objData);
        }
        else if (messageType == NetworkCommands::SERVEROPENPLAYERCOUNT)
        {
            onOpenPlayerCount(socketID, objData);
        }
        else if (messageType == NetworkCommands::SLAVEGAMESTARTED)
        {
            onSlaveGameStarted(socketID, objData);
        }
        else if (messageType == NetworkCommands::SLAVERELAUNCHED)
        {
            onSlaveRelaunched(socketID, objData);
        }
        else if (messageType == NetworkCommands::SLAVEINFODESPAWNING)
        {
            onSlaveInfoDespawning(socketID, objData);
        }
        else if (messageType == NetworkCommands::SLAVEMULTIPLAYERGAMERESULT)
        {
            m_matchMakingCoordinator.onSlaveInfoGameResult(socketID, objData);
            m_replayRecordFileserver.onSlaveInfoGameResult(socketID, objData);
        }
        else
        {
            CONSOLE_PRINT("Unknown command in MainServer::receivedSlaveData " + messageType + " received", GameConsole::eDEBUG);
        }
    }
}

void MainServer::exit()
{
    CONSOLE_PRINT("Closing server", GameConsole::eDEBUG);
    QCoreApplication::exit(0);
}

AutoMatchMaker *MainServer::getAutoMatchMaker(const QString matchMaker)
{
    return m_matchMakingCoordinator.getAutoMatchMaker(matchMaker);
}

void MainServer::onRequestPlayerStats(quint64 socketID, const QJsonObject &objData)
{
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QString command = QString(NetworkCommands::SERVERPLAYERSTATS);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QSqlQuery query = m_serverData->exec(QString("SELECT ") +
                                         SQL_COID + ", " +
                                         SQL_GAMESMADE + ", " +
                                         SQL_GAMESLOST + ", " +
                                         SQL_GAMESWON + ", " +
                                         SQL_GAMESDRAW +
                                         " from " + SQL_TABLE_PLAYERDATA + username +
                                         " WHERE " + SQL_COID +
                                         " LIKE '%';");
    bool success = !sqlQueryFailed(query);
    QJsonObject coStats;
    if (success && query.first())
    {
        do
        {
            QString coId = query.value(SQL_COID).toString();
            if (!coId.isEmpty())
            {
                QJsonObject obj;
                obj.insert(JsonKeys::JSONKEY_COSTATSDRAW, query.value(SQL_GAMESDRAW).toInt());
                obj.insert(JsonKeys::JSONKEY_COSTATSWON, query.value(SQL_GAMESWON).toInt());
                obj.insert(JsonKeys::JSONKEY_COSTATSLOST, query.value(SQL_GAMESLOST).toInt());
                obj.insert(JsonKeys::JSONKEY_COSTATSMADE, query.value(SQL_GAMESMADE).toInt());
                coStats.insert(coId, obj);
            }
        } while (query.next());
    }
    data.insert(JsonKeys::JSONKEY_COSTATS, coStats);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onRequestPlayersFromServer(quint64 socketID, const QJsonObject &objData)
{
    QString command = QString(NetworkCommands::SERVERUSERNAMES);
    QString searchFilter = objData.value(JsonKeys::JSONKEY_FILTER).toString();
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QSqlQuery query = m_serverData->exec(QString("SELECT ") +
                                         SQL_USERNAME +
                                         " from " + SQL_TABLE_PLAYERS +
                                         " WHERE " + SQL_USERNAME +
                                         " LIKE '" + searchFilter + "%';");
    bool success = !sqlQueryFailed(query);
    QJsonArray usernames;
    if (success && query.first())
    {
        do
        {
            usernames.append(query.value(SQL_USERNAME).toString());
        } while (query.next());
    }
    data.insert(JsonKeys::JSONKEY_USERNAMES, usernames);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onSlaveRelaunched(quint64 socketID, const QJsonObject &objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto *game = getInternGame(slaveName);
    if (game != nullptr)
    {
        // send data
        QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
        CONSOLE_PRINT("onSlaveRelaunched sending command " + command +
                          " with address " + game->game->getData().getSlaveAddress() +
                          " secondary address " + game->game->getData().getSlaveSecondaryAddress() +
                          " and port " + QString::number(game->game->getData().getSlavePort()),
                      GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_ADDRESS, game->game->getData().getSlaveAddress());
        data.insert(JsonKeys::JSONKEY_SECONDARYADDRESS, game->game->getData().getSlaveSecondaryAddress());
        data.insert(JsonKeys::JSONKEY_PORT, static_cast<qint64>(game->game->getData().getSlavePort()));
        QJsonDocument doc(data);
        if (!informClientsAboutRelaunch(m_runningSlaves, slaveName, doc))
        {
            if (informClientsAboutRelaunch(m_runningLobbies, slaveName, doc))
            {
                game->game->onSlaveRelaunched();
            }
        }
        else
        {
            game->game->onSlaveRelaunched();
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), GameConsole::eDEBUG);
    }
}

bool MainServer::informClientsAboutRelaunch(QVector<SuspendedSlaveInfo> &games, const QString &slaveName, const QJsonDocument &doc)
{
    for (qint32 i = 0; i < games.size(); ++i)
    {
        auto &game = games[i];
        if (game.game.getSlaveName() == slaveName)
        {
            CONSOLE_PRINT("Found slave game " + slaveName, GameConsole::eDEBUG);
            for (auto &socket : game.pendingSockets)
            {
                CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(socket), GameConsole::eDEBUG);
                emit m_pGameServer->sig_sendData(socket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
            }
            games.removeAt(i);
            return true;
        }
    }
    return false;
}

void MainServer::onSlaveInfoDespawning(quint64 socketID, const QJsonObject &objData)
{
    bool runningGame = objData.value(JsonKeys::JSONKEY_RUNNINGGAME).toBool();
    if (runningGame)
    {
        SuspendedSlaveInfo slaveInfo;
        slaveInfo.savefile = objData.value(JsonKeys::JSONKEY_SAVEFILE).toString();
        slaveInfo.game.fromJson(objData);
        slaveInfo.runningGame = runningGame;
        slaveInfo.despawnTime.start();
        setUuidForGame(slaveInfo.game);
        m_runningSlaves.append(slaveInfo);
    }
    else
    {
        SuspendedSlaveInfo slaveInfo;
        slaveInfo.savefile = objData.value(JsonKeys::JSONKEY_SAVEFILE).toString();
        slaveInfo.game.fromJson(objData);
        slaveInfo.runningGame = runningGame;
        slaveInfo.despawnTime.start();
        setUuidForGame(slaveInfo.game);
        m_runningLobbies.append(slaveInfo);
    }
    despawnSlave(socketID);
}

void MainServer::onRequestServerAutoMatchInfo(quint64 socketId, const QJsonObject &objData)
{
    QString command = QString(NetworkCommands::SERVERSENDAUTOMATCHINFO);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    m_matchMakingCoordinator.getMatchMakingData(objData.value(JsonKeys::JSONKEY_PLAYERID).toString(), data);
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::despawnSlave(quint64 socketID)
{
    QString command = NetworkCommands::DESPAWNSLAVE;
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending command " + command + " to slave to socket " + QString::number(socketID), GameConsole::eDEBUG);
    emit m_pSlaveServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::setUuidForGame(NetworkGameData &game)
{
    game.setUuid(m_uuidGameCounter);
    ++m_uuidGameCounter;
    if (m_uuidGameCounter == 0)
    {
        ++m_uuidGameCounter;
    }
}

MainServer::InternNetworkGame *MainServer::getInternGame(const QString &slaveName, qint32 *index)
{
    for (qint32 i = 0; i < m_games.size(); ++i)
    {
        auto &game = m_games[i];
        if (game->slaveName == slaveName)
        {
            if (index != nullptr)
            {
                *index = i;
            }
            return game.get();
        }
    }
    if (index != nullptr)
    {
        *index = -1;
    }
    return nullptr;
}

MapFileServer* MainServer::getMapFileServer()
{
    return &m_mapFileServer;
}

void MainServer::onSlaveReady(quint64 socketID, const QJsonObject &objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto *internGame = getInternGame(slaveName);
    if (internGame != nullptr)
    {
        internGame->game->onConnectToLocalServer(socketID, m_pSlaveServer);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), GameConsole::eDEBUG);
    }
}

void MainServer::onGameRunningOnServer(quint64 socketID, const QJsonObject &objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto *internGame = getInternGame(slaveName);
    if (internGame != nullptr)
    {
        internGame->game->slaveRunning(objData, m_pGameServer);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), GameConsole::eDEBUG);
    }
}

void MainServer::onSlaveGameStarted(quint64 socketID, const QJsonObject &objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    auto *internGame = getInternGame(slaveName);
    if (internGame != nullptr)
    {
        internGame->game->slaveGameStarted(objData);
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), GameConsole::eDEBUG);
    }
}

void MainServer::onRequestServerVersion(quint64 socketId, const QJsonObject &objData)
{
    QString command = QString(NetworkCommands::SERVERVERSION);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_VERSION, Mainapp::getGameVersion());
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onRequestUsergames(quint64 socketId, const QJsonObject &objData)
{
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    QString command = QString(NetworkCommands::SERVERUSERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonObject games;
    qint32 i = 0;
    qint32 start = objData.value(JsonKeys::JSONKEY_MATCHSTARTINDEX).toInt();
    qint32 count = objData.value(JsonKeys::JSONKEY_MATCHCOUNT).toInt();
    qint32 totalCount = 0;
    for (qint32 index = start; index < m_games.size(); ++index)
    {
        auto &game = m_games[index];
        if (game->game.get() != nullptr)
        {
            auto &data = game->game->getData();
            if (data.getPlayerNames().contains(username))
            {
                if (i < count)
                {
                    QJsonObject obj = game->game->getData().toJson();
                    games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                    ++i;
                }
                ++totalCount;
            }
        }
    }
    for (qint32 index = start; index < m_runningSlaves.size(); ++index)
    {
        auto &game = m_runningSlaves[index];
        if (game.game.getPlayerNames().contains(username))
        {
            if (i < count)
            {
                QJsonObject obj = game.game.toJson();
                games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                ++i;
            }
            ++totalCount;
        }
    }
    for (qint32 index = start; index < m_runningLobbies.size(); ++index)
    {
        auto &game = m_runningLobbies[index];
        if (game.game.getPlayerNames().contains(username))
        {
            if (i < count)
            {
                QJsonObject obj = game.game.toJson();
                games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                ++i;
            }
            ++totalCount;
        }
    }
    data.insert(JsonKeys::JSONKEY_GAMES, games);
    data.insert(JsonKeys::JSONKEY_MATCHCOUNT, totalCount);
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onRequestObservegames(quint64 socketId, const QJsonObject &objData)
{
    QString command = QString(NetworkCommands::SERVERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonObject games;
    qint32 i = 0;
    qint32 start = objData.value(JsonKeys::JSONKEY_MATCHSTARTINDEX).toInt();
    qint32 count = objData.value(JsonKeys::JSONKEY_MATCHCOUNT).toInt();
    qint32 totalCount = 0;
    for (qint32 index = start; index < m_games.size(); ++index)
    {
        auto &game = m_games[index];
        if (game->game.get() != nullptr &&
            game->game->getData().getObservers() < game->game->getData().getMaxObservers())
        {
            if (i < count)
            {
                QJsonObject obj = game->game->getData().toJson();
                games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                ++i;
            }
            ++totalCount;
        }
    }
    for (qint32 index = start; index < m_runningLobbies.size(); ++index)
    {
        auto &game = m_runningLobbies[index];
        if (game.game.getObservers() < game.game.getMaxObservers())
        {
            if (i < count)
            {
                QJsonObject obj = game.game.toJson();
                games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                ++i;
            }
            ++totalCount;
        }
    }
    data.insert(JsonKeys::JSONKEY_GAMES, games);
    data.insert(JsonKeys::JSONKEY_MATCHCOUNT, totalCount);
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onRequestGameData(quint64 socketId, const QJsonObject &objData)
{
    QString command = QString(NetworkCommands::SERVERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    QJsonObject games;
    qint32 i = 0;
    qint32 start = objData.value(JsonKeys::JSONKEY_MATCHSTARTINDEX).toInt();
    qint32 count = objData.value(JsonKeys::JSONKEY_MATCHCOUNT).toInt();
    qint32 totalCount = 0;
    for (qint32 index = start; index < m_games.size(); ++index)
    {
        auto &game = m_games[index];
        if (game->game.get() != nullptr &&
            game->game->getSlaveRunning() &&
            !game->game->getData().getLaunched())
        {
            if (i < count)
            {
                QJsonObject obj = game->game->getData().toJson();
                games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
                ++i;
            }
            ++totalCount;
        }
    }
    for (qint32 index = start; index < m_runningLobbies.size(); ++index)
    {
        if (i < count)
        {
            auto &game = m_runningLobbies[index];
            QJsonObject obj = game.game.toJson();
            games.insert(JsonKeys::JSONKEY_GAMEDATA + QString::number(i), obj);
            ++i;
        }
        ++totalCount;
    }
    data.insert(JsonKeys::JSONKEY_GAMES, games);
    data.insert(JsonKeys::JSONKEY_MATCHCOUNT, totalCount);
    // send server data to all connected clients
    QJsonDocument doc(data);
    emit m_pGameServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::onOpenPlayerCount(quint64 socketID, const QJsonObject &objData)
{
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    qint32 openPlayerCount = objData.value(JsonKeys::JSONKEY_OPENPLAYERCOUNT).toInteger();
    auto *internGame = getInternGame(slaveName);
    if (internGame != nullptr)
    {
        auto &data = internGame->game->getData();
        data.setPlayers(data.getMaxPlayers() - openPlayerCount);
        if (objData.contains(JsonKeys::JSONKEY_CURRENTPLAYER))
        {
            data.setCurrentPlayer(objData.value(JsonKeys::JSONKEY_CURRENTPLAYER).toString());
        }
        if (objData.contains(JsonKeys::JSONKEY_RUNNINGGAME))
        {
            data.setRunningGame(objData.value(JsonKeys::JSONKEY_RUNNINGGAME).toBool());
        }
        if (objData.contains(JsonKeys::JSONKEY_USERNAMES))
        {
            internGame->game->updatePlayers(objData);
        }
        if (objData.contains(JsonKeys::JSONKEY_ONLINEINFO))
        {
            internGame->game->updateOnlineInfo(objData);
        }
        if (objData.contains(JsonKeys::JSONKEY_MATCHOBSERVERCOUNT))
        {
            data.setObservers(objData.value(JsonKeys::JSONKEY_MATCHOBSERVERCOUNT).toInt());
        }
        if (objData.contains(JsonKeys::JSONKEY_MATCHMAXOBSERVERCOUNT))
        {
            data.setMaxObservers(objData.value(JsonKeys::JSONKEY_MATCHMAXOBSERVERCOUNT).toInt());
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to find slave game: " + slaveName + " running games: " + QString::number(m_games.size()), GameConsole::eDEBUG);
    }
}

void MainServer::joinSlaveGame(quint64 socketID, const QJsonObject &objData)
{
    bool found = false;
    QString slaveName = objData.value(JsonKeys::JSONKEY_SLAVENAME).toString();
    CONSOLE_PRINT("Searching for game " + slaveName + " for socket " + QString::number(socketID) + " to join game.", GameConsole::eDEBUG);
    auto *internGame = getInternGame(slaveName);
    if (internGame != nullptr)
    {
        // only send valid game data to clients
        if (internGame->game.get() != nullptr &&
            internGame->game->getServerName() == slaveName &&
            internGame->game->getSlaveRunning())
        {
            // send data
            QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
            CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_ADDRESS, internGame->game->getData().getSlaveAddress());
            data.insert(JsonKeys::JSONKEY_SECONDARYADDRESS, internGame->game->getData().getSlaveSecondaryAddress());
            data.insert(JsonKeys::JSONKEY_PORT, static_cast<qint64>(internGame->game->getData().getSlavePort()));
            QJsonDocument doc(data);
            emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
            found = true;
        }
    }
    if (!found)
    {
        found = tryJoinSuspendedGame(socketID, slaveName, m_runningSlaves);
        if (!found)
        {
            found = tryJoinSuspendedGame(socketID, slaveName, m_runningLobbies);
        }
    }
    if (!found)
    {
        CONSOLE_PRINT("Failed to find game " + slaveName + " for socket " + QString::number(socketID) + " to join game. Forcing a disconnection.", GameConsole::eDEBUG);
        QString command = QString(NetworkCommands::SERVERGAMENOLONGERAVAILABLE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

bool MainServer::tryJoinSuspendedGame(quint64 socketID, const QString &slave, QVector<SuspendedSlaveInfo> &games)
{
    bool found = false;
    for (auto &game : games)
    {
        if (game.game.getSlaveName() == slave)
        {
            game.pendingSockets.append(socketID);
            if (!game.relaunched)
            {
                spawnSlave(socketID, game);
            }
            found = true;
            break;
        }
    }
    return found;
}

void MainServer::startRemoteGame(const QString initScript, const QString id)
{
    emit sigStartRemoteGame(initScript, id);
}

void MainServer::slotStartRemoteGame(QString initScript, QString id)
{
    QByteArray sendData;
    QByteArray minimapData;
    spawnSlave(initScript, Settings::getInstance()->getMods(), id, 0, sendData, minimapData, true);
}

void MainServer::disconnected(qint64 socketId)
{
    // nothing to do.
}

void MainServer::spawnSlaveGame(QDataStream &stream, quint64 socketID, QByteArray &data, QString initScript, QString id)
{
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    if (validHostRequest(mods))
    {
        QByteArray minimapData = Filesupport::readByteArray(stream);
        spawnSlave(initScript, mods, id, socketID, data, minimapData);
    }
    else
    {
        CONSOLE_PRINT("Requested invalid mod configuration.", GameConsole::eDEBUG);
        QString command = QString(NetworkCommands::SERVERINVALIDMODCONFIG);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void MainServer::spawnSlave(quint64 socketID, SuspendedSlaveInfo &slaveInfo)
{

    QString slaveAddress;
    QString slaveSecondaryAddress;
    quint16 slavePort;
    if (getNextFreeSlaveAddress(slaveAddress, slavePort, slaveSecondaryAddress))
    {
        m_slaveGameIterator++;
        bool createLogs = Mainapp::getInstance()->getCreateSlaveLogs();
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
        CONSOLE_PRINT("Launching suspended slave game " + slaveName + " creating logs: " + (createLogs ? "true" : "false"), GameConsole::eDEBUG);
        auto game = MemoryManagement::create<InternNetworkGame>();
        const QString program = QCoreApplication::applicationFilePath();
        game->process = MemoryManagement::create<QProcess>(this);
        game->process->setObjectName(slaveName + "Slaveprocess");
        const char *const prefix = "--";
        QStringList args({QString(prefix) + CommandLineParser::ARG_SLAVE,
                          QString(prefix) + CommandLineParser::ARG_SLAVENAME,
                          slaveName,
                          QString(prefix) + CommandLineParser::ARG_NOUI, // comment out for debugging
                          QString(prefix) + CommandLineParser::ARG_NOAUDIO,
                          QString(prefix) + CommandLineParser::ARG_SLAVEADDRESS,
                          slaveAddress,
                          QString(prefix) + CommandLineParser::ARG_SLAVEPORT,
                          QString::number(slavePort),
                          QString(prefix) + CommandLineParser::ARG_MASTERADDRESS,
                          Settings::getInstance()->getSlaveListenAdress(),
                          QString(prefix) + CommandLineParser::ARG_MASTERPORT,
                          QString::number(Settings::getInstance()->getSlaveServerPort()),
                          QString(prefix) + CommandLineParser::ARG_SPAWNAIPROCESS,
                          "0"});
        if (!slaveSecondaryAddress.isEmpty())
        {
            args << QString(prefix) + CommandLineParser::ARG_SLAVESECONDARYADDRESS << slaveSecondaryAddress;
        }
        if (slaveInfo.game.getMods().length() > 0)
        {
            args << QString(prefix) + CommandLineParser::ARG_MODS << Settings::getInstance()->getConfigString(slaveInfo.game.getMods());
        }
        if (createLogs)
        {
            args << QString(prefix) + CommandLineParser::ARG_CREATESLAVELOGS;
        }
        game->game = MemoryManagement::create<NetworkGame>(this, slaveName);
        game->game->setSlaveRespawnFile(slaveInfo.savefile);
        game->game->setHostingSocket(socketID);
        game->game->getData().setSlaveAddress(slaveAddress);
        game->game->getData().setSlaveSecondaryAddress(slaveSecondaryAddress);
        game->game->getData().setSlavePort(slavePort);
        game->game->getData().setSlaveName(slaveName);
        setUuidForGame(game->game->getData());
        // copy over suspended slave data
        game->game->getData().setPlayerNames(slaveInfo.game.getPlayerNames());
        game->game->getData().setMapName(slaveInfo.game.getMapName());
        game->game->getData().setDescription(slaveInfo.game.getDescription());
        game->game->getData().setMods(slaveInfo.game.getMods());
        game->game->getData().setMaxPlayers(slaveInfo.game.getMaxPlayers());
        game->game->getData().setLocked(slaveInfo.game.getLocked());
        game->game->setRunningGame(slaveInfo.runningGame);
        // connect signals and spawn process
        connect(game->process.get(), &QProcess::finished, game->game.get(), &NetworkGame::processFinished, Qt::QueuedConnection);
        connect(game->process.get(), &QProcess::errorOccurred, game->game.get(), &NetworkGame::errorOccurred, Qt::QueuedConnection);
        connect(game->game.get(), &NetworkGame::sigClose, this, &MainServer::closeGame, Qt::QueuedConnection);
        CONSOLE_PRINT("Starting suspended slave process: " + program + " " + args.join(" "), GameConsole::eDEBUG);
        game->process->start(program, args);
        game->slaveName = slaveName;
        m_games.append(game);
        slaveInfo.game.setSlaveName(slaveName);
        slaveInfo.relaunched = true;
    }
    else
    {
        slaveInfo.pendingSockets.clear();
        CONSOLE_PRINT("No free slots available.", GameConsole::eDEBUG);
        QString command = QString(NetworkCommands::SERVERNOGAMESLOTSAVAILABLE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void MainServer::spawnSlave(const QString &initScript, const QStringList &mods, QString id, quint64 socketID, QByteArray &data, QByteArray &minimapData, bool trainingSession)
{

    QString slaveAddress;
    QString slaveSecondaryAddress;
    quint16 slavePort;
    if (getNextFreeSlaveAddress(slaveAddress, slavePort, slaveSecondaryAddress))
    {
        m_slaveGameIterator++;
        bool createLogs = Mainapp::getInstance()->getCreateSlaveLogs();
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
        CONSOLE_PRINT("Launching slave game " + slaveName + " creating logs: " + (createLogs ? "true" : "false"), GameConsole::eDEBUG);
        auto game = MemoryManagement::create<InternNetworkGame>();
        QString program = QCoreApplication::applicationFilePath();
        game->process = MemoryManagement::create<QProcess>(this);
        game->process->setObjectName(slaveName + "Slaveprocess");
        const char *const prefix = "--";
        QStringList args({QString(prefix) + CommandLineParser::ARG_SLAVE,
                          QString(prefix) + CommandLineParser::ARG_SLAVENAME,
                          slaveName,
                          QString(prefix) + CommandLineParser::ARG_NOUI, // comment out for debugging
                          QString(prefix) + CommandLineParser::ARG_NOAUDIO,
                          QString(prefix) + CommandLineParser::ARG_SLAVEADDRESS,
                          slaveAddress,
                          QString(prefix) + CommandLineParser::ARG_SLAVEPORT,
                          QString::number(slavePort),
                          QString(prefix) + CommandLineParser::ARG_MASTERADDRESS,
                          Settings::getInstance()->getSlaveListenAdress(),
                          QString(prefix) + CommandLineParser::ARG_MASTERPORT,
                          QString::number(Settings::getInstance()->getSlaveServerPort()),
                          QString(prefix) + CommandLineParser::ARG_SPAWNAIPROCESS,
                          "0"});
        if (!slaveSecondaryAddress.isEmpty())
        {
            args << QString(prefix) + CommandLineParser::ARG_SLAVESECONDARYADDRESS << slaveSecondaryAddress;
        }
        if (mods.length() > 0)
        {
            args << QString(prefix) + CommandLineParser::ARG_MODS << Settings::getInstance()->getConfigString(mods);
        }
        if (!initScript.isEmpty())
        {
            args << QString(prefix) + CommandLineParser::ARG_INITSCRIPT << initScript;
        }
        if (createLogs)
        {
            args << QString(prefix) + CommandLineParser::ARG_CREATESLAVELOGS;
        }
        if (trainingSession)
        {
            args << QString(prefix) + CommandLineParser::ARG_SLAVETRAINING;
        }
        game->game = MemoryManagement::create<NetworkGame>(this, slaveName);
        game->game->setDataBuffer(data);
        game->game->setHostingSocket(socketID);
        game->game->getData().setSlaveAddress(slaveAddress);
        game->game->getData().setSlaveSecondaryAddress(slaveSecondaryAddress);
        game->game->getData().setSlavePort(slavePort);
        game->game->getData().setMods(mods);
        game->game->getData().setMinimapData(minimapData);
        setUuidForGame(game->game->getData());
        connect(game->process.get(), &QProcess::finished, game->game.get(), &NetworkGame::processFinished, Qt::QueuedConnection);
        connect(game->process.get(), &QProcess::errorOccurred, game->game.get(), &NetworkGame::errorOccurred, Qt::QueuedConnection);
        connect(game->game.get(), &NetworkGame::sigClose, this, &MainServer::closeGame, Qt::QueuedConnection);
        game->game->setId(id);
        CONSOLE_PRINT("Starting slave process: " + program + " " + args.join(" "), GameConsole::eDEBUG);
        game->process->start(program, args);
        game->slaveName = slaveName;
        m_games.append(game);
    }
    else
    {
        CONSOLE_PRINT("No free slots available.", GameConsole::eDEBUG);
        QString command = QString(NetworkCommands::SERVERNOGAMESLOTSAVAILABLE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit m_pGameServer->sig_sendData(socketID, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

bool MainServer::validHostRequest(QStringList mods)
{
    // make sure the server has the requested mods installed.
    for (auto &mod : mods)
    {
        if (!QFile::exists(mod + "/mod.txt") &&
            !QFile::exists(Settings::getInstance()->getUserPath() + mod + "/mod.txt") &&
            !QFile::exists(oxygine::Resource::RCC_PREFIX_PATH + mod + "/mod.txt"))
        {
            return false;
        }
    }
    return true;
}

void MainServer::playerJoined(qint64 socketId)
{
    CONSOLE_PRINT("Player joined the server " + QString::number(socketId), GameConsole::eDEBUG);
    emit m_pGameServer->sigSetIsActive(socketId, false);
}

void MainServer::periodicTasks()
{
    // CONSOLE_PRINT("MainServer::periodicTasks", GameConsole::eDEBUG);
    cleanUpSuspendedGames(m_runningSlaves);
    cleanUpSuspendedGames(m_runningLobbies);
    executeScript();
    m_matchMakingCoordinator.periodicTasks();
}

void MainServer::cleanUpSuspendedGames(QVector<SuspendedSlaveInfo> &games)
{
    qint32 i = 0;
    std::chrono::milliseconds ms = Settings::getInstance()->getSuspendedDespawnTime();
    while (i < games.size())
    {
        auto &game = games[i];
        if (game.despawnTime.hasExpired(ms.count()))
        {
            CONSOLE_PRINT("Removing game with savefile " + game.savefile + " from suspended game list.", GameConsole::eDEBUG);
            QFile::remove(game.savefile);
            games.removeAt(i);
        }
        else
        {
            ++i;
        }
    }
}

void MainServer::executeScript()
{
    const char *const SCRIPTFILE = "serverScript.js";
    if (QFile::exists(SCRIPTFILE))
    {
        CONSOLE_PRINT("WorkerThread::executeServerScript loading server script" + QString(SCRIPTFILE), GameConsole::eDEBUG);
        Interpreter *pInterpreter = Interpreter::getInstance();
        if (pInterpreter->openScript(SCRIPTFILE, false))
        {
            QFile::remove(SCRIPTFILE);
            CONSOLE_PRINT("Executing server script", GameConsole::eDEBUG);
            pInterpreter->doFunction("serverScript");
        }
    }
}

void MainServer::closeGame(NetworkGame *pGame)
{
    CONSOLE_PRINT("Despawning  game", GameConsole::eDEBUG);
    QString slaveName = pGame->getServerName();
    qint32 index = -1;
    auto *game = getInternGame(slaveName, &index);
    if (game != nullptr)
    {
        SlaveAddress freeAddress;
        freeAddress.address = game->game->getData().getSlaveAddress();
        freeAddress.secondaryAddress = game->game->getData().getSlaveSecondaryAddress();
        freeAddress.port = game->game->getData().getSlavePort();
        CONSOLE_PRINT("Freeing address " + game->game->getData().getSlaveAddress() +
                          " secondary address " + game->game->getData().getSlaveSecondaryAddress() +
                          " and port " + QString::number(game->game->getData().getSlavePort()),
                      GameConsole::eDEBUG);
        m_freeAddresses.append(freeAddress);
        game->process->kill();
        game->game.reset();
        m_games.removeAt(index);
    }
}

bool MainServer::getNextFreeSlaveAddress(QString &address, quint16 &port, QString &secondaryAddress)
{
    bool success = false;
    if (!m_despawning)
    {
        address = "";
        secondaryAddress = "";
        port = 0;
        if (m_freeAddresses.size() > 0)
        {
            auto &newAddress = m_freeAddresses.constLast();
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
                auto &options = m_slaveAddressOptions[m_lastUsedAddressIndex];
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
        CONSOLE_PRINT("getNextFreeSlaveAddress using address " + address +
                          " secondary address " + secondaryAddress +
                          " and port " + QString::number(port),
                      GameConsole::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("Denied spawning of slave game cause server is despawning", GameConsole::eDEBUG);
    }
    return success;
}

void MainServer::createAccount(qint64 socketId, const QJsonObject &objData)
{
    QByteArray password = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    QString mailAdress = objData.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Creating account with username " + username + " and email adress " + mailAdress, GameConsole::eDEBUG);
    bool success = false;
    QSqlQuery query = getAccountInfo(*m_serverData, username, success);
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
        query = m_serverData->exec(command);
        createUserTable(username);
        if (sqlQueryFailed(query))
        {
            result = GameEnums::LoginError_AccountExists;
        }
    }
    else
    {
        CONSOLE_PRINT("Username is already existing.", GameConsole::eDEBUG);
        result = GameEnums::LoginError_AccountExists;
    }
    QString command = QString(NetworkCommands::SERVERRESPONSCREATEACCOUNT);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::deleteAccount(qint64 socketId, const QJsonObject &objData)
{
    QByteArray password = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    QString mailAdress = objData.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Deleting account with username " + username + " and email adress " + mailAdress, GameConsole::eDEBUG);
    bool success = false;
    GameEnums::LoginError result = GameEnums::LoginError_None;
    QSqlQuery query = getAccountInfo(*m_serverData, username, success);
    if (query.first() && success)
    {
        auto dbMailAdress = query.value(SQL_MAILADRESS);
        if (dbMailAdress.toString() == mailAdress)
        {
            result = checkPassword(*m_serverData, username, password);
            if (result == GameEnums::LoginError_None)
            {
                QString command = QString("DELETE FROM ") + SQL_TABLE_PLAYERS + " WHERE " +
                                  SQL_USERNAME + " = '" + username + "';";
                query = m_serverData->exec(command);
                if (sqlQueryFailed(query))
                {
                    result = GameEnums::LoginError_DatabaseNotAccesible;
                }
            }
        }
        else
        {
            result = GameEnums::LoginError_WrongEmailAdress;
        }
    }
    else
    {
        result = GameEnums::LoginError_AccountDoesntExist;
    }

    QString command = QString(NetworkCommands::SERVERRESPONSDELETEACCOUNT);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::loginToAccount(qint64 socketId, const QJsonObject &objData)
{
    QByteArray password = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Login to account with username " + username, GameConsole::eDEBUG);
    auto result = checkPassword(*m_serverData, username, password);
    if (result == GameEnums::LoginError_None)
    {
        // mark client as logged in
        emit m_pGameServer->sigSetIsActive(socketId, true);
    }
    QString command = QString(NetworkCommands::SERVERRESPONSLOGINACCOUNT);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

void MainServer::createUserTable(const QString &username)
{
    QSqlQuery query = m_serverData->exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_PLAYERDATA + username + " (" +
                                         SQL_COID + " TEXT PRIMARY KEY, " +
                                         SQL_GAMESMADE + " INTEGER, " +
                                         SQL_GAMESLOST + " INTEGER, " +
                                         SQL_GAMESWON + " INTEGER, " +
                                         SQL_GAMESDRAW + " INTEGER," +
                                         SQL_METADATA + " TEXT)");
    if (sqlQueryFailed(query))
    {
        CONSOLE_PRINT("Unable to create user table for user " + username + ". Error: " + m_serverData->lastError().nativeErrorCode(), GameConsole::eERROR);
    }
}

void MainServer::createMatchData(const QString &match)
{
    QSqlQuery query = m_serverData->exec(QString("CREATE TABLE if not exists ") + SQL_TABLE_MATCH_DATA + match + " (" +
                                         SQL_USERNAME + " TEXT PRIMARY KEY, " +
                                         SQL_MMR + " INTEGER, " +
                                         SQL_MINGAMES + " INTEGER, " +
                                         SQL_MAXGAMES + " INTEGER, " +
                                         SQL_RUNNINGGAMES + " INTEGER," +
                                         SQL_METADATA + " TEXT," +
                                         SQL_SIGNEDUP + " BOOL," +
                                         SQL_MATCHHISTORY + " TEXT)");
    if (sqlQueryFailed(query))
    {
        CONSOLE_PRINT("Unable to create match table for match " + match + ". Error: " + m_serverData->lastError().nativeErrorCode(), GameConsole::eERROR);
    }
}

GameEnums::LoginError MainServer::checkPassword(QSqlDatabase &database, const QString &username, const QByteArray &password)
{
    bool success = false;
    QSqlQuery query = getAccountInfo(database, username, success);
    GameEnums::LoginError result = GameEnums::LoginError_None;
    if (query.first() && success)
    {
        CONSOLE_PRINT("Checking password for : " + username, GameConsole::eDEBUG);
        auto dbPassword = query.value(SQL_PASSWORD);
        auto outdatedPassword = query.value(SQL_VALIDPASSWORD).toInt();
        if (dbPassword.toByteArray() != password.toHex())
        {
            result = GameEnums::LoginError_WrongPassword;
        }
        if (outdatedPassword == 0)
        {
            CONSOLE_PRINT("Account info for : " + username + " is out dated.", GameConsole::eDEBUG);
            result = GameEnums::LoginError_PasswordOutdated;
        }
    }
    else
    {
        CONSOLE_PRINT("Account info for : " + username + " not found.", GameConsole::eDEBUG);
        result = GameEnums::LoginError_AccountDoesntExist;
    }
    return result;
}

GameEnums::LoginError MainServer::verifyLoginData(const QString &username, const QByteArray &password)
{
    GameEnums::LoginError valid = GameEnums::LoginError_DatabaseNotAccesible;
    if (m_serverData != nullptr)
    {
        valid = checkPassword(*m_serverData, username, password);
    }
    return valid;
}

void MainServer::resetAccountPassword(qint64 socketId, const QJsonObject &objData)
{
    QString mailAdress = objData.value(JsonKeys::JSONKEY_EMAILADRESS).toString();
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Resetting account with username " + username + " and email adress " + mailAdress, GameConsole::eDEBUG);
    bool success = false;
    QSqlQuery query = getAccountInfo(*m_serverData, username, success);
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
            auto changeQuery = m_serverData->exec(QString("UPDATE ") + SQL_TABLE_PLAYERS + " SET " +
                                                  SQL_PASSWORD + " = " + "'" + password.getHash().toHex() + "', " +
                                                  SQL_VALIDPASSWORD + " = 0 WHERE " +
                                                  SQL_USERNAME + " = '" + username + "';");
            if (!sqlQueryFailed(changeQuery))
            {
                CONSOLE_PRINT("Try sending reset password mail", GameConsole::eDEBUG);
                emit m_mailSender.sigSendMail(socketId, "Commander Wars password reset", message, mailAdress, username);
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
    if (result != GameEnums::LoginError_None)
    {
        QString command = QString(NetworkCommands::SERVERRESPONSRESETPASSWORD);
        CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
        QJsonObject outData;
        outData.insert(JsonKeys::JSONKEY_COMMAND, command);
        outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
        QJsonDocument outDoc(outData);
        emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

void MainServer::onMailSendResult(quint64 socketId, const QString receiverAddress, const QString username, bool result)
{
    QString command = QString(NetworkCommands::SERVERRESPONSRESETPASSWORD);
    GameEnums::LoginError mailSendResult = GameEnums::LoginError_None;
    if (!result)
    {
        mailSendResult = GameEnums::LoginError_SendingMailFailed;
    }
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(mailSendResult), GameConsole::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, mailSendResult);
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
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

QSqlDatabase &MainServer::getDatabase()
{
    return *m_serverData;
}

void MainServer::changeAccountPassword(qint64 socketId, const QJsonObject &objData)
{
    QByteArray oldPassword = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_OLDPASSWORD).toArray());
    QByteArray password = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_PASSWORD).toArray());
    bool success = false;
    QString username = objData.value(JsonKeys::JSONKEY_USERNAME).toString();
    CONSOLE_PRINT("Login to account with username " + username, GameConsole::eDEBUG);
    QSqlQuery query = getAccountInfo(*m_serverData, username, success);
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
            auto changeQuery = m_serverData->exec(QString("UPDATE ") + SQL_TABLE_PLAYERS + " SET " +
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
                }
            }
        }
    }
    else
    {
        result = GameEnums::LoginError_AccountDoesntExist;
    }
    QString command = QString(NetworkCommands::SERVERRESPONSCHANGEPASSWORD);
    CONSOLE_PRINT("Sending command " + command + " with result " + QString::number(result), GameConsole::eDEBUG);
    QJsonObject outData;
    outData.insert(JsonKeys::JSONKEY_COMMAND, command);
    outData.insert(JsonKeys::JSONKEY_ACCOUNT_ERROR, result);
    QJsonDocument outDoc(outData);
    emit m_pGameServer->sig_sendData(socketId, outDoc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

QSqlQuery MainServer::getAccountInfo(QSqlDatabase &database, const QString &username, bool &success)
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

QSqlQuery MainServer::getAllUsers(QSqlDatabase &database, bool &success)
{
    QSqlQuery query = database.exec(QString("SELECT ") + SQL_USERNAME + " from " + SQL_TABLE_PLAYERS + ";");
    success = !sqlQueryFailed(query);
    return query;
}

void MainServer::despawnServer(const QString savefile)
{
    m_despawning = true;
    m_despawningSavefile = savefile;
    despawnRunningSlaves();
    doDespawnServer();
}

void MainServer::despawnRunningSlaves()
{
    for (auto &game : m_games)
    {
        game->game->forceDespawn(m_pGameServer);
    }
}

void MainServer::doDespawnServer()
{
    if (m_despawning &&
        m_games.size() > 0)
    {
        QFile file(m_despawningSavefile);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        serializeObject(stream);
    }
}

void MainServer::serializeObject(QDataStream &stream) const
{
    stream << getVersion();
    stream << static_cast<qint32>(m_runningSlaves.size());
    for (auto &runningSlave : m_runningSlaves)
    {
        runningSlave.serializeObject(stream);
    }
    stream << static_cast<qint32>(m_runningLobbies.size());
    for (auto &runningLobbies : m_runningLobbies)
    {
        runningLobbies.serializeObject(stream);
    }
    m_matchMakingCoordinator.serializeObject(stream);
}

void MainServer::deserializeObject(QDataStream &stream)
{
    qint32 version = 0;
    stream >> version;
    qint32 size = 0;
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        SuspendedSlaveInfo slaveInfo;
        slaveInfo.deserializeObject(stream);
        m_runningSlaves.append(slaveInfo);
    }
    stream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        SuspendedSlaveInfo slaveInfo;
        slaveInfo.deserializeObject(stream);
        m_runningLobbies.append(slaveInfo);
    }
    if (version > 2)
    {
        m_matchMakingCoordinator.deserializeObject(stream);
    }
}

void MainServer::SuspendedSlaveInfo::serializeObject(QDataStream &stream) const
{
    stream << getVersion();
    stream << relaunched;
    stream << runningGame;
    stream << savefile;
    auto obj = game.toJson();
    QJsonDocument doc(obj);
    Filesupport::writeByteArray(stream, doc.toJson(QJsonDocument::Compact));
}

void MainServer::SuspendedSlaveInfo::deserializeObject(QDataStream &stream)
{
    qint32 version = 0;
    stream >> version;
    stream >> relaunched;
    stream >> runningGame;
    stream >> savefile;
    QByteArray data;
    data = Filesupport::readByteArray(stream);
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject objData = doc.object();
    game.fromJson(objData);
}
