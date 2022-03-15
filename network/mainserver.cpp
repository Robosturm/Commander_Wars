#include <QBuffer>
#include <QFile>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/commandlineparser.h"

#include "game/gamemap.h"

#include "multiplayer/networkcommands.h"

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

    emit m_pGameServer->sig_connect(Settings::getServerListenAdress(), Settings::getServerPort());
    emit m_pSlaveServer->sig_connect(Settings::getSlaveListenAdress(), Settings::getSlaveServerPort());
}

MainServer::~MainServer()
{
    m_pGameServer->disconnectTCP();
    // clean up server and client games.
    for (auto & game : m_games)
    {
        game->process->kill();
    }
    m_games.clear();
}

void MainServer::parseSlaveAddressOptions()
{
    QStringList addressOptions = Settings::getSlaveHostOptions().split(";");
    for (auto & option : addressOptions)
    {
        QStringList values = option.split("&");
        if (values.size() == 3)
        {
            bool ok = false;
            AddressInfo info;
            info.address = values[0];
            info.minPort = values[1].toUInt(&ok);
            if (info.minPort == 0)
            {
                info.minPort = 1;
            }
            info.maxPort = values[2].toUInt(&ok);
            m_slaveAddressOptions.append(info);
            if (m_slaveAddressOptions.size() == 1)
            {
                m_lastUsedPort = info.minPort -1;
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
        CONSOLE_PRINT("Network Server Command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::SERVERJOINGAME)
        {
            joinSlaveGame(socketID, objData);
        }
    }
    else if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Network Server Command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID, data);
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
        CONSOLE_PRINT("Network Server Command received: " + messageType, Console::eDEBUG);
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
    quint16 slavePort;
    if (getNextFreeSlaveAddress(slaveAddress, slavePort))
    {
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
        auto game = spInternNetworkGame::create();
        QString program = "Commander_Wars.exe";
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
                          QString(prefix) + CommandLineParser::ARG_SLAVEPORT,
                          QString::number(slavePort),
                          QString(prefix) + CommandLineParser::ARG_MASTERADDRESS,
                          Settings::getSlaveListenAdress(),
                          QString(prefix) + CommandLineParser::ARG_MASTERPORT,
                          QString::number(Settings::getSlaveServerPort()),
                          QString(prefix) + CommandLineParser::ARG_INITSCRIPT,
                          initScript});
        if (Mainapp::getInstance()->getCreateSlaveLogs())
        {
            args << QString(prefix) + CommandLineParser::ARG_CREATESLAVELOGS;
        }
        game->game = spNetworkGame::create(nullptr);
        game->game->setDataBuffer(data);
        game->game->setServerName(slaveName);
        game->game->setHostingSocket(socketID);
        game->game->getData().setSlaveAddress(slaveAddress);
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
    for (qint32 i = 0; i < mods.size(); i++)
    {
        QFile file(mods[i] + "/mod.txt");
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
    sendGameDataToClient(socketId);
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
            game->game->getSlaveRunning())
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
        freeAddress.port = game->game->getData().getSlavePort();
        m_freeAddresses.append(freeAddress);
        game->process->kill();
        game->game = nullptr;
        m_games.remove(slaveName);
        m_updateGameData = true;
    }
}

bool MainServer::getNextFreeSlaveAddress(QString & address, quint16 & port)
{
    bool success = false;
    if (m_freeAddresses.size() > 0)
    {
        auto & newAddress = m_freeAddresses.constLast();
        address = newAddress.address;
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
                    success = getNextFreeSlaveAddress(address, port);
                }
            }
        }
    }
    return success;
}
