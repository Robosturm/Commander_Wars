#pragma once

#include <QObject>
#include <QSslServer>

class SslServer : public QSslServer
{
public:
    explicit SslServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socket) override;
};
