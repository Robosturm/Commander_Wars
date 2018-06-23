#include <QTcpSocket>
#include <QDataStream>

#include "network/RxTask.h"
#include "network/NetworkInterface.h"
RxTask::RxTask(QTcpSocket* pSocket, NetworkInterface* CommIF)
    : m_pSocket(pSocket),
      pIF(CommIF),
      dataSize(0),
      m_serive(Mainapp::NetworkSerives::Max)
{
}

RxTask::~RxTask()
{

}

void RxTask::recieveData()
{
        QDataStream pStream(m_pSocket);
        pStream.setVersion(QDataStream::Qt_5_11);

        qint32 bytes = 0;

        if (dataSize == 0)
        {
            bytes = m_pSocket->bytesAvailable();
            if (bytes < static_cast<qint32>(sizeof(qint32)))
            {
                // not enough data recieved
                return;
            }
            // read message size
            pStream >> dataSize;
        }

        bytes = m_pSocket->bytesAvailable();
        if (bytes < static_cast<qint32>(sizeof(qint32)))
        {
            // not enough data recieved
            return;
        }
        qint32 service;
        pStream >> service;
        m_serive = static_cast<Mainapp::NetworkSerives>(service);

        bytes = m_pSocket->bytesAvailable();
        if (bytes < static_cast<qint32>(dataSize))
        {
            // check if object was recieved
            return;
        }
        QByteArray data;
        pStream >> data;

        // read Object        
        dataSize = 0;
        if ((m_serive < Mainapp::NetworkSerives::Console) || (m_serive >= Mainapp::NetworkSerives::Max))
        {
            // don't send an event :)
        }
        else
        {
            // note only one Service can recieve a message!!!
            // since the service needs to delete the object.
            // otherwise you get some nice null-pointer exeptions
            emit pIF->recieveData(data, m_serive);
        }
}
