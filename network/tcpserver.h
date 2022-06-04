#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QVector>

#include "network/NetworkInterface.h"
#include "network/tcpclient.h"
#include "network/rxtask.h"
#include "network/txtask.h"

class QTcpServer;

class TCPServer;
using spTCPServer = oxygine::intrusive_ptr<TCPServer>;

class TCPServer : public NetworkInterface
{
    Q_OBJECT
public:
    TCPServer(QObject* pParent);
    virtual ~TCPServer();

    spTCPClient getClient(quint64 socketID);
public slots:
    virtual void connectTCP(QString adress, quint16 port, bool secure = false) override;
    virtual void disconnectTCP() override;
    virtual void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service) override;
    virtual QVector<quint64> getConnectedSockets() override;
    qint32 getObserverCount();
    void onConnect();
    void disconnectClient(quint64 socketID);
    void pauseListening();
    void continueListening();
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
private:
    void ready(quint64 ready);
private:
    QMap<quint64, spTCPClient> m_pClients;
    quint64 m_idCounter = 0;
    std::shared_ptr<QTcpServer> m_pTCPServer{nullptr};
    bool m_gameServer{false};
    bool m_secure{false};
};

#endif // TCPSERVER_H
