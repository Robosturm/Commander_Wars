#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QSslError>
#include <QList>

#include "network/networkInterface.h"
#include "network/rxtask.h"
#include "network/txtask.h"

class QSslSocket;
using spQSslSocket = std::shared_ptr<QSslSocket>;

class TCPClient;
using spTCPClient = std::shared_ptr<TCPClient>;

class TCPClient final : public NetworkInterface
{
    Q_OBJECT
public:
    TCPClient(QObject* pParent);
    TCPClient(QObject* pParent, spRxTask pRXTask, spTxTask pTXTask, spQSslSocket pSocket, quint64 socketId);
    virtual ~TCPClient();
    /**
     * @brief moveClientToThread
     * @param pThread
     */
    void moveClientToThread(QThread* pThread);
    spRxTask getRXTask() const;
    spTxTask getTXTask() const;
    virtual void setSocketID(const quint64 &socketID) override;

public slots:
    virtual void connectTCP(QString address, quint16 port, QString /* secondaryAdress */, bool gatewayClient = false) override;
    virtual void disconnectTCP() override;
    virtual QVector<quint64> getConnectedSockets() override;
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
protected slots:
    void connected();
private slots:
    void sslErrors(const QList<QSslError> &errors);

private:
    spRxTask m_pRXTask;
    spTxTask m_pTXTask;
    spQSslSocket m_pSocket;
    bool m_onServer{false};
    QString m_secondaryAdress;
    quint16 m_port{0};
    bool m_testedSecondaryAddress{false};
};

#endif // TCPCLIENT_H
