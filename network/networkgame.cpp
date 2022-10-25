#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "network/networkgame.h"
#include "network/JsonKeys.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/interpreter.h"

#include "game/gamemap.h"

NetworkGame::NetworkGame(QObject* pParent)
    : QObject(pParent),
      m_closeTimer(pParent)
{
    connect(&m_closeTimer, &QTimer::timeout, this, &NetworkGame::closeTimerExpired, Qt::QueuedConnection);
}

void NetworkGame::startCloseTimer()
{
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
        QString description = objData.value(JsonKeys::JSONKEY_GAMEDESCRIPTION).toString();
        bool hasPassword = objData.value(JsonKeys::JSONKEY_HASPASSWORD).toBool();
        m_data.setDescription(description);
        m_data.setLocked(hasPassword);
        auto pClient = pGameServer->getClient(m_hostingSocket);
        if (pClient.get() != nullptr)
        {
            // send data
            QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
            CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
            QJsonObject data;
            data.insert(JsonKeys::JSONKEY_COMMAND, command);
            data.insert(JsonKeys::JSONKEY_ADDRESS, m_data.getSlaveAddress());
            data.insert(JsonKeys::JSONKEY_SECONDARYADDRESS, m_data.getSlaveSecondaryAddress());
            data.insert(JsonKeys::JSONKEY_PORT, static_cast<qint64>(m_data.getSlavePort()));
            QJsonDocument doc(data);
            emit pClient->sig_sendData(m_hostingSocket, doc.toJson(), NetworkInterface::NetworkSerives::ServerHostingJson, false);
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
    QStringList playerNames;
    QJsonArray usernames = objData.value(JsonKeys::JSONKEY_USERNAMES).toArray();
    CONSOLE_PRINT("Adding players to slavegame " + m_serverName, Console::eDEBUG);
    for (const auto & username : usernames)
    {
        QString user = username.toString();
        playerNames.append(user);
        CONSOLE_PRINT("user: " + user, Console::eDEBUG);
    }
    m_data.setPlayerNames(playerNames);
}

const QString & NetworkGame::getId() const
{
    return m_id;
}

void NetworkGame::setId(QString & id)
{
    m_id = id;
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
    CONSOLE_PRINT("onConnectToLocalServer reading game data", Console::eDEBUG);
    if (m_slaveRespawnFile.isEmpty())
    {
        m_data.setSlaveName(m_serverName);
        QDataStream stream(&m_dataBuffer, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        QStringList mods;
        mods = Filesupport::readVectorList<QString, QList>(stream);
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
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_MAPNAME, m_slaveRespawnFile);
        QJsonDocument doc(data);
        emit pTcpServer->sig_sendData(socketId, doc.toJson(), NetworkInterface::NetworkSerives::ServerHosting, false);
    }
}

QString NetworkGame::getServerName() const
{
    return m_serverName;
}

void NetworkGame::setServerName(const QString &serverName)
{
    m_serverName = serverName;
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
    CONSOLE_PRINT("Networkgame Closing game cause slave game has been terminated with code " + QString::number(value), Console::eDEBUG);
    closeGame();
    Interpreter* pInterpreter = Interpreter::getInstance();
    emit pInterpreter->sigNetworkGameFinished(value - 1, m_id);
}

void NetworkGame::closeGame()
{
    if (!m_closing)
    {
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
