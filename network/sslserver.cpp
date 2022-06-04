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
    connect(sslSocket, QSslSocket::sslErrors, this, SslServer::sslErrors, Qt::QueuedConnection);
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
    sslSocket->setPrivateKey(m_key);
    addPendingConnection(sslSocket);
}

void SslServer::sslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
    {
        CONSOLE_PRINT(error.errorString(), Console::eDEBUG);
    }
}
