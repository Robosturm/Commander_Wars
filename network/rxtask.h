#ifndef RXTASK_H
#define RXTASK_H

#include <QObject>

#include <QDataStream>

#include "oxygine-framework.h"

#include "network/NetworkInterface.h"

class QIODevice;

class RxTask;
typedef oxygine::intrusive_ptr<RxTask> spRxTask;

class RxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    RxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF);
    virtual ~RxTask();
signals:
    void sigForwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
public slots:
    void recieveData();
private:
   QIODevice* m_pSocket;
   NetworkInterface* pIF;
   quint64 m_SocketID;
   QDataStream m_pStream;
};

#endif // RXTASK_H
