#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include "network/NetworkInterface.h"
#include "network/rxtask.h"
#include "network/txtask.h"

class QLocalSocket;

class LocalClient : public NetworkInterface
{
    Q_OBJECT
public:
    LocalClient();
    virtual ~LocalClient();
public slots:
    virtual void connectTCP(QString adress, quint16) override;
    virtual void disconnectTCP() override;
    virtual QVector<quint64> getConnectedSockets() override;
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
protected slots:
    void connected();
private:
    spRxTask m_pRXTask;
    spTxTask m_pTXTask;
    QLocalSocket* m_pSocket;
};

#endif // LOCALCLIENT_H
