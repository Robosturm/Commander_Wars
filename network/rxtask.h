#ifndef RXTASK_H
#define RXTASK_H

#include <QObject>
#include "coreengine/mainapp.h"

class QTcpSocket;
class Serializable;
class NetworkInterface;

class RxTask : public QObject
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
   Mainapp::NetworkSerives m_serive;
};

#endif // RXTASK_H
