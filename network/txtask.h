#ifndef TxTask_H
#define TxTask_H

#include <QObject>
#include "coreengine/mainapp.h"

class QTcpSocket;
class NetworkInterface;
/**
 * @brief The TXTask class
 */
class TxTask : public QObject
{
    Q_OBJECT
public:
    TxTask(QTcpSocket* pSocket, NetworkInterface* CommIF);
public slots:
    /**
     * @brief send sends the Object via TCP
     * @param pObj
     */
    void send(QByteArray data, Mainapp::NetworkSerives service, bool blocking);
private:
   QTcpSocket* m_pSocket;
   NetworkInterface* pIF;
};

#endif // TXTASK_H
