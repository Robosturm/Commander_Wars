#ifndef RXTASK_H
#define RXTASK_H

#include <QObject>
#include "coreengine/mainapp.h"

#include "oxygine-framework.h"

class QTcpSocket;
class Serializable;
class NetworkInterface;

class RxTask;
typedef oxygine::intrusive_ptr<RxTask> spRxTask;

class RxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    RxTask(QTcpSocket* pSocket, NetworkInterface* CommIF);
    virtual ~RxTask();
public slots:
    void recieveData();
private:
   QTcpSocket* m_pSocket;
   NetworkInterface* pIF;
   qint32 dataSize;
   Mainapp::NetworkSerives m_serive{Mainapp::NetworkSerives::None};
};

#endif // RXTASK_H
