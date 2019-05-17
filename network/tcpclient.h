#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "network/NetworkInterface.h"

#include "network/rxtask.h"
#include "network/txtask.h"

class QTcpSocket;

class TCPClient : public NetworkInterface
{
    Q_OBJECT
public:
    TCPClient();
    virtual ~TCPClient();


public slots:
    virtual void connectTCP(const QString& adress, quint16 port) override;
    virtual void disconnectTCP() override;
    /**
     * @brief sendData send Data with this Connection
     * @param data
     */
    virtual void sendData(std::shared_ptr<QTcpSocket> pSocket, QByteArray data, NetworkSerives service, bool forwardData) override;

private:
    spRxTask pRXTask;
    spTxTask pTXTask;
    std::shared_ptr<QTcpSocket> pSocket;
};

#endif // TCPCLIENT_H
