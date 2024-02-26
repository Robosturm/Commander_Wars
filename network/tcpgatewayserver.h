#pragma once

#include "network/tcpserver.h"

class TcpGatewayServer;
using spTcpGatewayServer = std::shared_ptr<TcpGatewayServer>;

class TcpGatewayServer final : public TCPServer
{
    Q_OBJECT
public:
    TcpGatewayServer(QObject* pParent);

    quint64 getGatewayHost() const;

public slots:
    virtual void onConnect() override;
    void receivedData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket);
    virtual void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service) override;
private:
    quint64 m_gatewayHost{0};
};

Q_DECLARE_INTERFACE(TcpGatewayServer, "TcpGatewayServer");
