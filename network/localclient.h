#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include "network/NetworkInterface.h"
#include "network/rxtask.h"
#include "network/txtask.h"

class QLocalSocket;

class LocalClient;
using spLocalClient = std::shared_ptr<LocalClient>;

class LocalClient final : public NetworkInterface
{
    Q_OBJECT
public:
    LocalClient(QObject* pParent);
    virtual ~LocalClient();
public slots:
    virtual void connectTCP(QString primaryAdress, quint16 port, QString secondaryAdress) override;
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
