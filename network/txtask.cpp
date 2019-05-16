#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>

#include "coreengine/console.h"
#include "network/txtask.h"
#include "network/NetworkInterface.h"

TxTask::TxTask(QTcpSocket* pSocket, NetworkInterface* CommIF)
 : m_pSocket(pSocket),
   pIF(CommIF)
{
}

TxTask::~TxTask()
{

}

void TxTask::send(QByteArray data, Mainapp::NetworkSerives service, bool forwardData)
{
    if (m_pSocket->isOpen())
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_12);
        // write default-buffersize
        out << static_cast<qint32>(data.size());

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

