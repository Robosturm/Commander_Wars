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

void TxTask::send(QByteArray data, Mainapp::NetworkSerives service, bool blocking)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_11);
    // write default-buffersize
    out << static_cast<qint32>(0);

    out << static_cast<qint32>(service);
    //write Object
    out << data;
    // write message size
    out.device()->seek(0);

    qint32 datasize = static_cast<qint32>(data.size());
    out << datasize;
    // send data this function should be blocking.
    if (m_pSocket->write(block) != block.size())
    {
        Console::print("Error during writing data via TCP.", Console::eERROR);
    }
    if (blocking)
    {
        pIF->acquireBlock();
    }
}

