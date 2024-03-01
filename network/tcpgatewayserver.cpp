#include <QJsonObject>
#include <QJsonDocument>
#include "network/tcpgatewayserver.h"
#include "network/JsonKeys.h"
#include "multiplayer/networkcommands.h"

#include "coreengine/gameconsole.h"

TcpGatewayServer::TcpGatewayServer(QObject* pParent)
    : TCPServer(pParent)
{
    m_useReceivedId = true;
    setObjectName("TcpGatewayServer");
    connect(this, &TCPServer::recieveData, this, &TcpGatewayServer::receivedData, Qt::QueuedConnection);
    connect(this, &TCPServer::sigDisconnectClient, this, &TcpGatewayServer::disconnectClient, Qt::QueuedConnection);
}

TcpGatewayServer::~TcpGatewayServer()
{
    CONSOLE_PRINT("~TcpGatewayServer", GameConsole::eDEBUG);
}

void TcpGatewayServer::onConnect()
{
    TCPServer::onConnect();
    if (m_gatewayHost == 0)
    {
        m_gatewayHost = m_idCounter;
    }
    else
    {
        QString command = NetworkCommands::GATEWAYCLIENTCONNECTED;
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_COMMAND, command);
        data.insert(JsonKeys::JSONKEY_SOCKETID, static_cast<qint64>(m_idCounter));
        QJsonDocument doc(data);
        CONSOLE_PRINT("Sending gateway info command " + command + " to socket " + QString::number(m_gatewayHost), GameConsole::eDEBUG);
        emit m_pClients[m_gatewayHost]->sig_sendData(m_gatewayHost, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::Gateway, false);
    }
}

void TcpGatewayServer::disconnectClient(quint64 socketID)
{
    QString command = NetworkCommands::GATEWAYCLIENTDISCONNECTED;
    QJsonObject data;
    data.insert(JsonKeys::JSONKEY_COMMAND, command);
    data.insert(JsonKeys::JSONKEY_SOCKETID, static_cast<qint64>(socketID));
    QJsonDocument doc(data);
    CONSOLE_PRINT("Sending gateway info command " + command + " to socket " + QString::number(m_gatewayHost), GameConsole::eDEBUG);
    emit m_pClients[m_gatewayHost]->sig_sendData(m_gatewayHost, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::Gateway, false);
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
        socket != m_gatewayHost &&
        m_pClients[socket].get() != nullptr)
    {
        emit m_pClients[socket]->sig_sendData(socket, data, service, false);
    }
    else if (senderSocket != m_gatewayHost)
    {
        emit m_pClients[m_gatewayHost]->sig_sendData(socket, data, service, false);
    }
}
