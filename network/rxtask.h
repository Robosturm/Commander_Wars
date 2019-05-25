#ifndef RXTASK_H
#define RXTASK_H

#include <QObject>

#include <QDataStream>

#include "oxygine-framework.h"

#include "network/NetworkInterface.h"

class QTcpSocket;

class RxTask;
typedef oxygine::intrusive_ptr<RxTask> spRxTask;

class RxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    RxTask(std::shared_ptr<QTcpSocket> pSocket, quint64 socketID, NetworkInterface* CommIF);
    virtual ~RxTask();
public slots:
    void recieveData();
private:
   std::shared_ptr<QTcpSocket> m_pSocket;
   NetworkInterface* pIF;
   quint64 m_SocketID;
   QDataStream m_pStream;
};

#endif // RXTASK_H
