#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QVector>

#include "network/NetworkInterface.h"
#include "network/tcpclient.h"

class QTcpServer;

class TCPServer;
using spTCPServer = std::shared_ptr<TCPServer>;

class TCPServer final : public NetworkInterface
{
    Q_OBJECT
public:
    TCPServer(QObject* pParent);
    ~TCPServer();

    spTCPClient getClient(quint64 socketID);
signals:
    void sigSetIsActive(quint64 socketID, bool active);

public slots:
    void setIsActive(quint64 socketID, bool active);
    virtual void connectTCP(QString adress, quint16 port, QString secondaryAdress) override;
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
    QMap<quint64, spTCPClient> m_pClients;
    quint64 m_idCounter = 0;
    std::shared_ptr<QTcpServer> m_pTCPServer[2];
    bool m_gameServer{false};
};

#endif // TCPSERVER_H
