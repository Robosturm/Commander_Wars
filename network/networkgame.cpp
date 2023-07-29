#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "network/networkgame.h"
#include "network/JsonKeys.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/interpreter.h"

#include "game/gamemap.h"

NetworkGame::NetworkGame(QObject* pParent, const QString & serverName)
    : QObject(pParent),
      m_closeTimer(pParent),
      m_serverName(serverName)

{
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Creating NetworkGame " + m_serverName, GameConsole::eDEBUG);
    connect(&m_closeTimer, &QTimer::timeout, this, &NetworkGame::closeTimerExpired, Qt::QueuedConnection);
}

NetworkGame::~NetworkGame()
{
    CONSOLE_PRINT("Deleting NetworkGame " + m_serverName, GameConsole::eDEBUG);
}

void NetworkGame::startCloseTimer()
{
    CONSOLE_PRINT("Starting close timer", GameConsole::eDEBUG);
    m_closeTimer.setSingleShot(true);
    m_closeTimer.start(std::chrono::minutes(5));
}

void NetworkGame::closeTimerExpired()
{
    if (m_data.getPlayers() == 0)
    {
        closeGame();
    }
}

bool NetworkGame::getRunningGame() const
{
    return m_runningGame;
}

void NetworkGame::setRunningGame(bool newRunningGame)
{
    m_runningGame = newRunningGame;
}

const QString &NetworkGame::getSlaveRespawnFile() const
{
    return m_slaveRespawnFile;
}

void NetworkGame::setSlaveRespawnFile(const QString &newSlaveRespawnFile)
{
    m_slaveRespawnFile = newSlaveRespawnFile;
}

void NetworkGame::slaveRunning(const QJsonObject & objData, spTCPServer & pGameServer)
{
    if (!m_slaveRunning)
    {
        QString description = objData.value(JsonKeys::JSONKEY_DESCRIPTION).toString();
        bool hasPassword = objData.value(JsonKeys::JSONKEY_HASPASSWORD).toBool();
        m_data.setDescription(description);
        m_data.setLocked(hasPassword);
        auto pClient = pGameServer->getClient(m_hostingSocket);
        if (pClient.get() != nullptr)
        {
            // send data
            QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
            CONSOLE_PRINT("slaveRunning sending command " + command +
                          " with address " + m_data.getSlaveAddress() +
                          " secondary address " + m_data.getSlaveSecondaryAddress() +
                          " and port " + QString::number(m_data.getSlavePort()), GameConsole::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_ADDRESS, m_data.getSlaveAddress());
            data.insert(JsonKeys::JSONKEY_SECONDARYADDRESS, m_data.getSlaveSecondaryAddress());
            data.insert(JsonKeys::JSONKEY_PORT, m_data.getSlavePort());
            QJsonDocument doc(data);
            emit pClient->sig_sendData(m_hostingSocket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
            m_slaveRunning = true;
        }
        else
        {
            closeGame();
        }
    }
}

void NetworkGame::slaveGameStarted(const QJsonObject & objData)
{
    m_data.setLaunched(true);
    updatePlayers(objData);
}

void NetworkGame::updatePlayers(const QJsonObject & objData)
{
    QStringList playerNames;
    QJsonArray usernames = objData.value(JsonKeys::JSONKEY_USERNAMES).toArray();
    CONSOLE_PRINT("Adding players to slavegame " + m_serverName, GameConsole::eDEBUG);
    for (const auto & username : usernames)
    {
        QString user = username.toString();
        playerNames.append(user);
        CONSOLE_PRINT("user: " + user, GameConsole::eDEBUG);
    }
    m_data.setPlayerNames(playerNames);
}

void NetworkGame::updateOnlineInfo(const QJsonObject & objData)
{
    QVector<bool> onlineInfos;
    QJsonArray jsonOnlineInfos = objData.value(JsonKeys::JSONKEY_ONLINEINFO).toArray();
    for (const auto & data : jsonOnlineInfos)
    {
        onlineInfos.append(data.toBool());
    }
    m_data.setOnlineData(onlineInfos);
}


const QString & NetworkGame::getId() const
{
    return m_id;
}

void NetworkGame::setId(QString & id)
{
    m_id = id;
}

void NetworkGame::onSlaveRelaunched()
{
    m_slaveRunning = true;
    if (m_runningGame)
    {
        m_data.setLaunched(true);
    }
}

bool NetworkGame::getSlaveRunning() const
{
    return m_slaveRunning;
}

const NetworkGameData & NetworkGame::getData() const
{
    return m_data;
}

NetworkGameData & NetworkGame::getData()
{
    return m_data;
}

void NetworkGame::onConnectToLocalServer(quint64 socketId, spTCPServer & pTcpServer)
{    
    CONSOLE_PRINT("onConnectToLocalServer reading game data", GameConsole::eDEBUG);
    if (m_slaveRespawnFile.isEmpty())
    {
        m_data.setSlaveName(m_serverName);
        QDataStream stream(&m_dataBuffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Version::Qt_6_5);
        QString messageType;
        stream >> messageType;
        QStringList mods;
        mods = Filesupport::readVectorList<QString, QList>(stream);
        Filesupport::readByteArray(stream); // minimap data
        m_data.setMods(mods);
        GameMap::MapHeaderInfo headerInfo;
        GameMap::readMapHeader(stream, headerInfo);
        m_data.setMapName(headerInfo.m_mapName);
        m_data.setMaxPlayers(headerInfo.m_playerCount);
        emit pTcpServer->sig_sendData(socketId, m_dataBuffer, NetworkInterface::NetworkSerives::ServerHosting, false);
        // free buffer after it has been send to the server
        m_dataBuffer.clear();
    }
    else
    {
        QString command = QString(NetworkCommands::SERVERRELAUNCHSLAVE);
        CONSOLE_PRINT("Sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_NAME, m_slaveRespawnFile);
        data.insert(JsonKeys::JSONKEY_RUNNINGGAME, m_runningGame);
        QJsonDocument doc(data);
        emit pTcpServer->sig_sendData(socketId, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}

QString NetworkGame::getServerName() const
{
    return m_serverName;
}

QByteArray NetworkGame::getDataBuffer() const
{
    return m_dataBuffer;
}

void NetworkGame::setDataBuffer(const QByteArray &dataBuffer)
{
    m_dataBuffer = dataBuffer;
}

void NetworkGame::processFinished(int value, QProcess::ExitStatus)
{
    CONSOLE_PRINT("Networkgame closing game " + m_serverName + " cause slave game has been terminated with code " + QString::number(value), GameConsole::eDEBUG);
    closeGame();
    Interpreter* pInterpreter = Interpreter::getInstance();
    emit pInterpreter->sigNetworkGameFinished(value - 1, m_id);
}

void NetworkGame::errorOccurred(QProcess::ProcessError error)
{
    CONSOLE_PRINT("Networkgame error in game " + m_serverName + " with code " + QString::number(error), GameConsole::eDEBUG);
}

void NetworkGame::closeGame()
{
    if (!m_closing)
    {
        CONSOLE_PRINT("Closing slave process.", GameConsole::eDEBUG);
        m_closing = true;
        emit sigClose(this);
    }
}

quint64 NetworkGame::getHostingSocket() const
{
    return m_hostingSocket;
}

void NetworkGame::setHostingSocket(quint64 newHostingSocket)
{
    m_hostingSocket = newHostingSocket;
}

void NetworkGame::forceDespawn(spTCPServer & pGameServer)
{
    auto pClient = pGameServer->getClient(m_hostingSocket);
    if (pClient.get() != nullptr)
    {
        QString command = QString(NetworkCommands::SLAVEFORCEDESPAWN);
        CONSOLE_PRINT("slaveRunning sending command " + command, GameConsole::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        QJsonDocument doc(data);
        emit pClient->sig_sendData(m_hostingSocket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}
