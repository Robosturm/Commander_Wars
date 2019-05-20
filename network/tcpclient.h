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
     * @brief getSocket
     * @param socketID
     * @return
     */
    virtual QTcpSocket* getSocket(quint64) override
    {
        return pSocket.get();
    }
private:
    spRxTask pRXTask;
    spTxTask pTXTask;
    std::shared_ptr<QTcpSocket> pSocket;
};

#endif // TCPCLIENT_H
