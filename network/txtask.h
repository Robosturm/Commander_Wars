#ifndef TxTask_H
#define TxTask_H

#include <QObject>

#include "oxygine-framework.h"
#include "NetworkInterface.h"

class QTcpSocket;

class TxTask;
typedef oxygine::intrusive_ptr<TxTask> spTxTask;

/**
 * @brief The TXTask class
 */
class TxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    TxTask(std::shared_ptr<QTcpSocket> pSocket, NetworkInterface* CommIF);
    virtual ~TxTask();
public slots:
    /**
     * @brief send sends the Object via TCP
     * @param pObj
     */
    void send(std::shared_ptr<QTcpSocket> pSocket, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData);
private:
   std::shared_ptr<QTcpSocket> m_pSocket;
   NetworkInterface* pIF;
};

#endif // TXTASK_H
