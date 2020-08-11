#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "network/NetworkInterface.h"

#include "network/rxtask.h"
#include "network/txtask.h"

class QTcpSocket;

class TCPClient;
typedef oxygine::intrusive_ptr<TCPClient> spTCPClient;

class TCPClient : public NetworkInterface
{
    Q_OBJECT
public:
    TCPClient();
    TCPClient(spRxTask pRXTask, spTxTask pTXTask, QTcpSocket* pSocket, quint64 socketId);
    virtual ~TCPClient();

    quint64 getSocketId() const
    {
        return m_socketId;
    }
    /**
     * @brief moveClientToThread
     * @param pThread
     */
    void moveClientToThread(QThread* pThread);
public slots:
    virtual void connectTCP(QString adress, quint16 port) override;
    virtual void disconnectTCP() override;
    virtual QVector<quint64> getConnectedSockets() override;
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
protected slots:
    void connected();
private:
    spRxTask m_pRXTask;
    spTxTask m_pTXTask;
    QTcpSocket* m_pSocket;
    quint64 m_socketId{0};
};

#endif // TCPCLIENT_H
