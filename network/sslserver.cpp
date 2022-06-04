#include <QSslSocket>

#include "network/sslserver.h"
#include "network/rsakeygenerator.h"

SslServer::SslServer(QObject *parent)
    : QTcpServer{parent}
{

    bool success;
    auto data = RsaKeyGenerator::generatePrivateKey(success);
    if (success)
    {
        m_key = QSslKey(data, QSsl::KeyAlgorithm::Rsa);
    }
}

void SslServer::incomingConnection(qintptr socketDescriptor)
{
    QSslSocket *sslSocket = new QSslSocket(this);
    sslSocket->setSocketDescriptor(socketDescriptor);
    sslSocket->setPeerVerifyMode(QSslSocket::PeerVerifyMode::VerifyNone);
    sslSocket->setPrivateKey(m_key);
    addPendingConnection(sslSocket);
}
