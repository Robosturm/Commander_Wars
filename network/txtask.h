#ifndef TxTask_H
#define TxTask_H

#include <QObject>
#include "coreengine/mainapp.h"

#include "oxygine-framework.h"

class QTcpSocket;
class NetworkInterface;

class TxTask;
typedef oxygine::intrusive_ptr<TxTask> spTxTask;

/**
 * @brief The TXTask class
 */
class TxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    TxTask(QTcpSocket* pSocket, NetworkInterface* CommIF);
    virtual ~TxTask();
public slots:
    /**
     * @brief send sends the Object via TCP
     * @param pObj
     */
    void send(QByteArray data, Mainapp::NetworkSerives service, bool forwardData);
private:
   QTcpSocket* m_pSocket;
   NetworkInterface* pIF;
};

#endif // TXTASK_H
