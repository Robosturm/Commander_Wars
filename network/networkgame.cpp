#include <QFile>

#include "network/networkgame.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/interpreter.h"

#include "game/gamemap.h"

NetworkGame::NetworkGame(QObject* pParent)
    : QObject(pParent)
{
}

void NetworkGame::slaveRunning(QDataStream &stream, spTCPServer & pGameServer)
{
    QString description;
    stream >> description;
    bool hasPassword = false;
    stream >> hasPassword;
    m_data.setDescription(description);
    m_data.setLocked(hasPassword);
    auto pClient = pGameServer->getClient(m_hostingSocket);
    if (pClient.get() != nullptr)
    {
        // send data
        QString command = QString(NetworkCommands::SLAVEADDRESSINFO);
        CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
        QByteArray sendData;
        QDataStream sendStream(&sendData, QIODevice::WriteOnly);
        sendStream << command;
        sendStream << m_data.getAddress();
        sendStream << m_data.getPort();
        emit pClient->sig_sendData(m_hostingSocket, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
        m_slaveRunning = true;
    }
    else
    {
        closeGame();
    }
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

void NetworkGame::onConnectToLocalServer(quint64 socketId, spTCPServer & pTcpServer)
{
    CONSOLE_PRINT("onConnectToLocalServer reading game data", Console::eDEBUG);
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
    CONSOLE_PRINT("Networkgame Closing game cause slave game has been terminated.", Console::eDEBUG);
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
