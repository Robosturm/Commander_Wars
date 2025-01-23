#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QVector>

#include "network/networkInterface.h"
#include "network/tcpclient.h"

class QSslServer ;
using spQSslServer = std::shared_ptr<QSslServer>;
class TCPServer;
using spTCPServer = std::shared_ptr<TCPServer>;

class TCPServer : public NetworkInterface
{
    Q_OBJECT
    static constexpr qint32 SERVER_SOCKETS = 2;
public:
    TCPServer(QObject* pParent);
    virtual ~TCPServer();

    spTCPClient getClient(quint64 socketID);
signals:
    void sigSetIsActive(quint64 socketID, bool active);

public slots:
    void setIsActive(quint64 socketID, bool active);
    virtual void connectTCP(QString adress, quint16 port, QString secondaryAdress, bool sendAll = false) override;
    virtual void disconnectTCP() override;
    virtual void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service) override;
    virtual QVector<quint64> getConnectedSockets() override;
    qint32 getObserverCount();
    virtual void onConnect();
    void disconnectClient(quint64 socketID);
    void pauseListening();
    void continueListening();
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
protected:
    void displayDetailedError() override;
protected:
    QMap<quint64, spTCPClient> m_pClients;
    quint64 m_idCounter = 0;
    spQSslServer m_pTCPServer[SERVER_SOCKETS];
    bool m_gameServer{false};
    bool m_useReceivedId{false};
    bool m_sendAll{false};
};

#endif // TCPSERVER_H
