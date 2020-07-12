#include <QIODevice>
#include <QDataStream>

#include "coreengine/console.h"
#include "network/txtask.h"
#include "network/NetworkInterface.h"

TxTask::TxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF)
 : m_pSocket(pSocket),
   m_SocketID(socketID),
   pIF(CommIF)
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
         socketID == 0))
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_12);
        // write default-buffersize
        out << static_cast<qint32>(service);
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
