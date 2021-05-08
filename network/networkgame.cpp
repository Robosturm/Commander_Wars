#include "network/networkgame.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"

#include "game/gamemap.h"

NetworkGame::NetworkGame()
    : QObject(),
      m_timer(this)
{
    connect(&m_gameConnection, &LocalClient::sigConnected, this, &NetworkGame::onConnectToLocalServer, Qt::QueuedConnection);
    connect(&m_gameConnection, &LocalClient::recieveData, this, &NetworkGame::recieveSlaveData, Qt::QueuedConnection);
    connect(&m_timer, &QTimer::timeout, this, &NetworkGame::checkServerRunning, Qt::QueuedConnection);
}

void NetworkGame::addClient(spTCPClient pClient)
{
    m_Clients.append(pClient);
    pClient->getRXTask()->swapNetworkInterface(pClient.get());
    disconnect(pClient.get(), &TCPClient::recieveData, nullptr, nullptr);
    disconnect(pClient.get(), &TCPClient::sigForwardData, nullptr, nullptr);
    connect(pClient.get(), &TCPClient::recieveData, this, &NetworkGame::recieveClientData, Qt::QueuedConnection);
    connect(pClient.get(), &TCPClient::sigForwardData, this, &NetworkGame::forwardData, Qt::QueuedConnection);
    connect(pClient.get(), &TCPClient::sigDisconnected, this, &NetworkGame::clientDisconnect, Qt::QueuedConnection);
    if (m_slaveRunning)
    {
        sendPlayerJoined(m_Clients.size() - 1);
    }
}

void NetworkGame::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    Console::print("Forwarding message from socket " + QString::number(socketID), Console::eDEBUG);
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        if (m_Clients[i]->getSocketID() != socketID)
        {
            emit m_Clients[i]->sig_sendData(0, data, service, false);
        }
    }
}

void NetworkGame::recieveSlaveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service)
{
    QString messageType;
    QDataStream stream(&data, QIODevice::ReadOnly);
    if (service == NetworkInterface::NetworkSerives::Multiplayer ||
        service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        stream >> messageType;
    }
    Console::print("Recieve Route message:" + messageType + " for socket " + QString::number(socket), Console::eDEBUG);
    if (messageType == NetworkCommands::GAMERUNNINGONSERVER)
    {
        slaveRunning(stream);
    }
    else if (messageType == NetworkCommands::SERVEROPENPLAYERCOUNT)
    {
        qint32 openPlayerCount = 0;
        stream >> openPlayerCount;
        m_data.setPlayers(m_data.getMaxPlayers() - openPlayerCount);
        emit sigDataChanged();
    }
    else if (messageType == NetworkCommands::PLAYERDISCONNECTEDGAMEONSERVER)
    {
        quint64 socket;
        stream >> socket;
        clientDisconnect(socket);
    }
    else if (m_slaveRunning)
    {
        if (messageType == NetworkCommands::INITGAME)
        {
            m_data.setLaunched(true);
        }
        else if (messageType == NetworkCommands::PLAYERCHANGED)
        {
            QString command = QString(NetworkCommands::SERVERREQUESTOPENPLAYERCOUNT);
            Console::print("Sending command " + command, Console::eDEBUG);
            QByteArray sendData;
            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
            sendStream << command;
            emit m_gameConnection.sig_sendData(0, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
        }
        Console::print("Routing message:" + messageType + " for socket " + QString::number(socket), Console::eDEBUG);
        // forward data to other clients
        for (qint32 i = 0; i < m_Clients.size(); i++)
        {
            if (socket == 0 || m_Clients[i]->getSocketID() == socket)
            {
                emit m_Clients[i]->sig_sendData(socket, data, service, false);
            }
        }
    }
}

void NetworkGame::slaveRunning(QDataStream &stream)
{
    QString description;
    stream >> description;
    bool hasPassword = false;
    stream >> hasPassword;
    m_data.setDescription(description);
    m_data.setLocked(hasPassword);
    if (m_Clients.size() == 1)
    {
        sendPlayerJoined(0);
        m_slaveRunning = true;
    }
    else
    {
        emit sigClose(this);
    }
}

void NetworkGame::sendPlayerJoined(qint32 player)
{
    QString command = QString(NetworkCommands::PLAYERJOINEDGAMEONSERVER);
    Console::print("Sending command " + command, Console::eDEBUG);
    quint64 socket = m_Clients[player]->getSocketID();;
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << command;
    sendStream << socket;
    emit m_gameConnection.sig_sendData(socket, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
}

void NetworkGame::setSlaveRunning(bool slaveRunning)
{
    m_slaveRunning = slaveRunning;
}

bool NetworkGame::getSlaveRunning() const
{
    return m_slaveRunning;
}

const NetworkGameData & NetworkGame::getData() const
{
    return m_data;
}

void NetworkGame::recieveClientData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // forward data to hosted game
    Console::print("Recieved Client data sending data to slave for socket " + QString::number(socket), Console::eDEBUG);
    emit m_gameConnection.sig_sendData(socket, data, service, false);
}

void NetworkGame::startAndWaitForInit()
{
    m_timer.setSingleShot(false);
    m_timer.start(200);
}

void NetworkGame::checkServerRunning()
{
    QString markername = "temp/" + m_serverName + ".marker";
    if (QFile::exists(markername))
    {
        emit m_gameConnection.sig_connect(m_serverName, 0);
        m_timer.stop();
    }
}

void NetworkGame::onConnectToLocalServer(quint64)
{
    emit m_gameConnection.sigChangeThread(0, thread());
    emit m_gameConnection.sig_sendData(0, m_dataBuffer, NetworkInterface::NetworkSerives::ServerHosting, false);

    m_data.setSlaveName(m_serverName);

    QDataStream stream(&m_dataBuffer, QIODevice::ReadOnly);
    QString messageType;
    stream >> messageType;
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    m_data.setMods(mods);
    qint32 version = 0;
    qint32 heigth = 0;
    qint32 width = 0;
    QString author;
    QString mapName;
    QString mapDescription;
    qint32 playerCount = 0;
    qint32 uniqueIdCounter = 0;
    GameMap::readMapHeader(stream, version, mapName, author, mapDescription,
                           width, heigth, playerCount, uniqueIdCounter);
    m_data.setMapName(mapName);
    m_data.setMaxPlayers(playerCount);
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

void NetworkGame::clientDisconnect(quint64 socketId)
{
    Console::print("Networkgame Client " + QString::number(socketId) + " disconnected.", Console::eDEBUG);
    bool isHost = false;
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        if (m_Clients[i]->getSocketID() == socketId)
        {
            isHost = (i == 0);
            m_Clients.removeAt(i);
            break;
        }
    }
    QString command = QString(NetworkCommands::PLAYERDISCONNECTEDGAMEONSERVER);
    Console::print("Sending command " + command, Console::eDEBUG);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << command;
    stream << socketId;
    emit m_gameConnection.sig_sendData(0, data, NetworkInterface::NetworkSerives::ServerHosting, false);
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        if (m_Clients[i]->getSocketID() != socketId)
        {
            emit m_Clients[i]->sig_sendData(0, data, NetworkInterface::NetworkSerives::ServerHosting, false);
        }
    }
    emit sigDisconnectSocket(socketId);
    if (isHost || m_Clients.size() == 0)
    {
        Console::print("Networkgame Closing game: " + getServerName() + " cause host has disconnected.", Console::eDEBUG);
        emit sigClose(this);
    }
}

void NetworkGame::processFinished(int, QProcess::ExitStatus)
{
    Console::print("Networkgame Closing game cause slave game has been terminated.", Console::eDEBUG);
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        emit sigDisconnectSocket(m_Clients[i]->getSocketID());
    }
    emit sigClose(this);
}
