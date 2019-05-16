#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QVector>
#include <QMutex>



#include "network/NetworkInterface.h"

class TxTask;
class RxTask;
class QTcpServer;
class QTcpSocket;
class QNetworkSession;

#include "network/rxtask.h"
#include "network/txtask.h"

class TCPServer : public NetworkInterface
{
    Q_OBJECT
public:
    TCPServer();
    virtual ~TCPServer();
public slots:
    virtual void connectTCP(const QString& adress, quint16 port) override;
    virtual void disconnectTCP() override;
    void disconnectSocket();
    void onConnect();

    /**
     * @brief sendData send Data with this Connection
     * @param data
     */
    virtual void sendData(QByteArray data, Mainapp::NetworkSerives service, bool forwardData) override;
    virtual void forwardData(QTcpSocket* pSocket, QByteArray data, Mainapp::NetworkSerives service) override;

private:
    QMutex TaskMutex;
    QVector<spRxTask> pRXTasks;
    QVector<spTxTask> pTXTasks;
    QVector<QTcpSocket*> pTCPSockets;
    QTcpServer* pTCPServer;
};

#endif // TCPSERVER_H
