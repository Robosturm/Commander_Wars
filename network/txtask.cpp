#include <QIODevice>
#include <QDataStream>

#include "coreengine/console.h"

#include "network/txtask.h"
#include "network/NetworkInterface.h"

TxTask::TxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool sendAll)
 : m_pSocket(pSocket),
   m_SocketID(socketID),
   m_pIF(CommIF),
   m_sendAll(sendAll)
{
}

TxTask::~TxTask()
{

}

void TxTask::send(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData)
{
    bool open = m_pSocket->isOpen();
    if (open &&
        (m_SocketID == socketID ||
         socketID == 0 ||
         m_sendAll))
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        // write default-buffersize
        out << static_cast<qint32>(service);
        out << socketID;
        out << forwardData;
        //write Object
        out << data;
        // send data this function should be blocking.
        if (m_pSocket->write(block) != block.size())
        {
            Console::print("Error during writing data via TCP.", Console::eERROR);
        }
    }
}

quint64 TxTask::getSocketID() const
{
    return m_SocketID;
}

void TxTask::setSocketID(const quint64 &SocketID)
{
    m_SocketID = SocketID;
}
