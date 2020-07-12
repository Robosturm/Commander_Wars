#include "networkgame.h"

NetworkGame::NetworkGame()
    : QObject()
{
    connect(this, &NetworkGame::sigStartAndWaitForInit, this, &NetworkGame::startAndWaitForInit, Qt::QueuedConnection);
    connect(&m_gameConnection, &LocalClient::sigConnected, this, &NetworkGame::onConnectToLocalServer, Qt::QueuedConnection);
}

void NetworkGame::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] != socketID)
        {
            emit m_Clients[i]->sig_sendData(0, data, service, false);
        }
    }
}

void NetworkGame::recieveClientData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service)
{

}

void NetworkGame::recieveServerData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service)
{
    // always root data to the actual server
    emit m_gameConnection.sig_sendData(0, data, service, false);
}

void NetworkGame::startAndWaitForInit(QString serverName)
{
    m_gameConnection.connectTCP(serverName, 0);
}

void NetworkGame::onConnectToLocalServer(quint64)
{
    emit m_gameConnection.sig_sendData(0, m_dataBuffer, NetworkInterface::NetworkSerives::ServerHosting, false);
    // free buffer after it has been send to the server
    m_dataBuffer.clear();
}

QByteArray NetworkGame::getDataBuffer() const
{
    return m_dataBuffer;
}

void NetworkGame::setDataBuffer(const QByteArray &dataBuffer)
{
    m_dataBuffer = dataBuffer;
}
