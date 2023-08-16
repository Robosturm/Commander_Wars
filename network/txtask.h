#ifndef TxTask_H
#define TxTask_H

#include <QObject>

#include "network/NetworkInterface.h"

class QIODevice;
class TxTask;
using spTxTask = std::shared_ptr<TxTask>;

/**
 * @brief The TXTask class
 */
class TxTask final : public QObject
{
    Q_OBJECT
public:
    TxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool sendAll);
   virtual ~TxTask() = default;
    quint64 getSocketID() const;
    void setSocketID(const quint64 &SocketID);
    void close();
public slots:
    /**
     * @brief send sends the Object via TCP
     * @param pObj
     */
    void send(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData);
private:
   QIODevice* m_pSocket;
   quint64 m_SocketID;
   NetworkInterface* m_pIF;
   bool m_sendAll{false};
};

#endif // TXTASK_H
