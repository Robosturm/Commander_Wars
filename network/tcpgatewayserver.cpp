#include "network/tcpgatewayserver.h"

TcpGatewayServer::TcpGatewayServer(QObject* pParent)
    : TCPServer(pParent)
{
    connect(this, &TCPServer::recieveData, this, &TcpGatewayServer::receivedData, Qt::QueuedConnection);
}

void TcpGatewayServer::onConnect()
{
    TCPServer::onConnect();
    if (m_gatewayHost == 0)
    {
        m_gatewayHost = m_idCounter;
    }
}

void TcpGatewayServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (auto & client : m_pClients)
    {
        if (client->getSocketID() != socketID &&
            client->getSocketID() != m_gatewayHost &&
            client->getIsActive())
        {
            emit client->sig_sendData(socketID, data, service, false);
        }
    }
}

quint64 TcpGatewayServer::getGatewayHost() const
{
    return m_gatewayHost;
}

void TcpGatewayServer::receivedData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket)
{
    if (senderSocket == m_gatewayHost &&
        senderSocket != 0 &&
        m_pClients[senderSocket].get() != nullptr)
    {
        emit m_pClients[senderSocket]->sig_sendData(socket, data, service, false);
    }
    else if (senderSocket != m_gatewayHost)
    {
        emit m_pClients[m_gatewayHost]->sig_sendData(socket, data, service, false);
    }
}
