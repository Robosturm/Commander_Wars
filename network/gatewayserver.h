#pragma once

#include <QObject>
#include "network/tcpgatewayserver.h"

class MainServer;

class GatewayServer : public QObject
{
    Q_OBJECT    
public:
    explicit GatewayServer(MainServer& parent);

    void onLaunchGatewayGameOnServer(quint64 socketID, const QJsonObject & objData);
private:
    bool disconnected(TcpGatewayServer* tcpGatewayServer, quint64 socket);
private:
    MainServer & m_pParent;
    QVector<spTcpGatewayServer> m_gatewayServers;
};

Q_DECLARE_INTERFACE(GatewayServer, "GatewayServer");
