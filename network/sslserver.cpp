#include <QSslSocket>

#include "network/sslserver.h"

#include "coreengine/console.h"

SslServer::SslServer(QObject *parent)
    : QTcpServer{parent}
{
    // todo add key
}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
    sslSocket->setPrivateKey(m_key);
    addPendingConnection(sslSocket);
}
