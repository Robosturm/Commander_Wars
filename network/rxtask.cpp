#include <QTcpSocket>
#include <QDataStream>

#include "network/rxtask.h"

#include "network/NetworkInterface.h"

RxTask::RxTask(QTcpSocket* pSocket, NetworkInterface* CommIF)
    : m_pSocket(pSocket),
      pIF(CommIF),
      dataSize(0)
{
}

RxTask::~RxTask()
{

}

void RxTask::recieveData()
{
        QDataStream pStream(m_pSocket);
        pStream.setVersion(QDataStream::Qt_5_12);

        qint64 bytes = 0;

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

        if (m_serive == Mainapp::NetworkSerives::None)
        {
            bytes = m_pSocket->bytesAvailable();
            if (bytes < static_cast<qint32>(sizeof(qint32)))
            {
                // not enough data recieved
                return;
            }
            qint32 service;
            pStream >> service;
            m_serive = static_cast<Mainapp::NetworkSerives>(service);
        }

        bytes = m_pSocket->bytesAvailable();
        if (bytes < static_cast<qint32>(dataSize) + static_cast<qint32>(sizeof(bool)))
        {
            // check if object was recieved
            return;
        }
        bool forwardData = false;
        pStream >> forwardData;
        QByteArray data;
        pStream >> data;

        // read Object        
        dataSize = 0;
        if ((m_serive < Mainapp::NetworkSerives::Game) || (m_serive >= Mainapp::NetworkSerives::Max))
        {
            // don't send an event :)
        }
        else
        {
            if (pIF->getIsServer() && forwardData)
            {
                pIF->forwardData(m_pSocket, data, m_serive);
            }
            // note only one Service can recieve a message!!!
            // since the service needs to delete the object.
            // otherwise you get some nice null-pointer exeptions
            emit pIF->recieveData(data, m_serive);
        }
        m_serive = Mainapp::NetworkSerives::None;
}
