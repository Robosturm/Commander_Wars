#ifndef RXTASK_H
#define RXTASK_H

#include <QObject>
#include <QDataStream>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "network/NetworkInterface.h"

class QIODevice;

class RxTask;
typedef oxygine::intrusive_ptr<RxTask> spRxTask;

class RxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    RxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool useReceivedId);
    virtual ~RxTask();
    void swapNetworkInterface(NetworkInterface* pCommIF)
    {
        pIF = pCommIF;
    }
    quint64 getSocketID() const;
    void setSocketID(const quint64 &SocketID);
public slots:
    void recieveData();
private:
   QIODevice* m_pSocket;
   NetworkInterface* pIF;
   quint64 m_SocketID;
   QDataStream m_pStream;
   bool m_useReceivedId{false};
};

#endif // RXTASK_H
