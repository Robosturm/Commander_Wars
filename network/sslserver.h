#pragma once

#include <QTcpServer>
#include <QSslError>
#include <QSslKey>
#include <QList>

class SslServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit SslServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QSslKey m_key;
};
