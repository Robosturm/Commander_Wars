#include "networkgame.h"
#include "multiplayer/networkcommands.h"

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
    connect(pClient.get(), &NetworkInterface::recieveData, this, &NetworkGame::recieveClientData, Qt::QueuedConnection);
    connect(pClient.get(), &NetworkInterface::sigForwardData, this, &NetworkGame::forwardData, Qt::QueuedConnection);
    connect(pClient.get(), &NetworkInterface::sigDisconnected, this, &NetworkGame::clientDisconnect, Qt::QueuedConnection);
    if (m_slaveRunning)
    {
        sendPlayerJoined(m_Clients.size() - 1);
    }
}

void NetworkGame::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        if (m_Clients[i]->getSocketId() != socketID)
        {
            emit m_Clients[i]->sig_sendData(0, data, service, false);
        }
    }
}

void NetworkGame::recieveSlaveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service)
{
    QDataStream stream(&data, QIODevice::ReadOnly);
    QString messageType;
    stream >> messageType;
    Console::print("Routing message:" + messageType, Console::eDEBUG);
    if (messageType == NetworkCommands::GAMERUNNINGONSERVER)
    {
        m_slaveRunning = true;
        if (m_Clients.size() == 1)
        {
            sendPlayerJoined(0);
        }
        else
        {
            // todo despawn slave
        }
    }
    else if (m_slaveRunning)
    {
        // forward data to other clients
        for (qint32 i = 0; i < m_Clients.size(); i++)
        {
            if (socket == 0 || m_Clients[i]->getSocketId() == socket)
            {
                emit m_Clients[i]->sig_sendData(0, data, service, false);
            }
        }
    }
}

void NetworkGame::sendPlayerJoined(qint32 player)
{
    QByteArray sendData;
    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
    sendStream << NetworkCommands::PLAYERJOINEDGAMEONSERVER;
    sendStream << m_Clients[player]->getSocketId();
    emit m_gameConnection.sig_sendData(0, sendData, NetworkInterface::NetworkSerives::ServerHosting, false);
}

void NetworkGame::recieveClientData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // forward data to hosted game
    Console::print("Recieved Client data sending data to slave", Console::eDEBUG);
    emit m_gameConnection.sig_sendData(0, data, service, false);
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
    emit m_gameConnection.sigChangeThread(0, QObject::thread());
    emit m_gameConnection.sig_sendData(0, m_dataBuffer, NetworkInterface::NetworkSerives::ServerHosting, false);
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

}
