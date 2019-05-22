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

class TCPServer;
typedef oxygine::intrusive_ptr<TCPServer> spTCPServer;

class TCPServer : public NetworkInterface
{
    Q_OBJECT
public:
    TCPServer();
    virtual ~TCPServer();
signals:
    void sigDisconnectClient(quint64 socketID);
    /**
     * @brief sigForwardData forwards data to all clients except for the given socket
     * @param socketID
     * @param data
     * @param service
     */
    void sigForwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);

    void sigPauseListening();
    void sigContinueListening();
public slots:
    virtual void connectTCP(const QString& adress, quint16 port) override;
    virtual void disconnectTCP() override;
    void disconnectSocket();
    void onConnect();
    virtual void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service) override;
    virtual QTcpSocket* getSocket(quint64 socketID) override;
    void disconnectClient(quint64 socketID);

    void pauseListening();
    void continueListening();
    QVector<quint64> getConnectedSockets();
private:
    QMutex TaskMutex;
    QVector<spRxTask> pRXTasks;
    QVector<spTxTask> pTXTasks;
    QVector<std::shared_ptr<QTcpSocket>> pTCPSockets;
    QVector<quint64> m_SocketIDs;
    quint64 m_idCounter = 0;
    QTcpServer* pTCPServer;
};

#endif // TCPSERVER_H
