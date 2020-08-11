#include "networkgame.h"

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
    disconnect(pClient.get(), &TCPClient::recieveData, nullptr, nullptr);
    disconnect(pClient.get(), &TCPClient::sigForwardData, nullptr, nullptr);
    connect(pClient.get(), &NetworkInterface::recieveData, this, &NetworkGame::recieveClientData, Qt::QueuedConnection);
    connect(pClient.get(), &NetworkInterface::sigForwardData, this, &NetworkGame::forwardData, Qt::QueuedConnection);
    connect(pClient.get(), &NetworkInterface::sigDisconnected, this, &NetworkGame::clientDisconnect, Qt::QueuedConnection);
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
    // forward data to other clients
    for (qint32 i = 0; i < m_Clients.size(); i++)
    {
        if (socket == 0 || m_Clients[i]->getSocketId() == socket)
        {
            emit m_Clients[i]->sig_sendData(0, data, service, false);
        }
    }
}

void NetworkGame::recieveClientData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // forward data to hosted game
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
