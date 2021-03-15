#ifndef TxTask_H
#define TxTask_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "network/NetworkInterface.h"

class QIODevice;

class TxTask;
typedef oxygine::intrusive_ptr<TxTask> spTxTask;

/**
 * @brief The TXTask class
 */
class TxTask : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    TxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool sendAll);
    virtual ~TxTask();
    quint64 getSocketID() const;
    void setSocketID(const quint64 &SocketID);

public slots:
    /**
     * @brief send sends the Object via TCP
     * @param pObj
     */
    void send(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData);
private:
   QIODevice* m_pSocket;
   quint64 m_SocketID;
   NetworkInterface* pIF;
   bool m_sendAll{false};
};

#endif // TXTASK_H
