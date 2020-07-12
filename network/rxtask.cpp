#include <QIODevice>

#include "network/rxtask.h"

#include "network/NetworkInterface.h"

RxTask::RxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF)
    : m_pSocket(pSocket),
      pIF(CommIF),
      m_SocketID(socketID),
      m_pStream(m_pSocket)
{
    m_pStream.setVersion(QDataStream::Qt_5_12);
}

RxTask::~RxTask()
{

}
void RxTask::recieveData()
{
    while (m_pSocket->bytesAvailable() > 0)
    {
        m_pStream.startTransaction();
        qint32 service;
        m_pStream >> service;
        NetworkInterface::NetworkSerives eService = static_cast<NetworkInterface::NetworkSerives>(service);
        bool forwardData = false;
        m_pStream >> forwardData;
        QByteArray data;
        m_pStream >> data;
        if (!m_pStream.commitTransaction())
        {
            return;
        }

        if ((eService < NetworkInterface::NetworkSerives::Game) || (eService >= NetworkInterface::NetworkSerives::Max))
        {
            // don't send an event :)
            Q_ASSERT(false);
        }
        else
        {
            if (pIF->getIsServer() && forwardData)
            {
                emit sigForwardData(m_SocketID, data, eService);
            }
            emit pIF->recieveData(m_SocketID, data, eService);
        }
    }
}
