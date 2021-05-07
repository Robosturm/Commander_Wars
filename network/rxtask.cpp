#include <QIODevice>

#include "network/rxtask.h"
#include "network/NetworkInterface.h"

RxTask::RxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool useReceivedId)
    : m_pSocket(pSocket),
      m_pIF(CommIF),
      m_SocketID(socketID),
      m_pStream(m_pSocket),
      m_useReceivedId(useReceivedId)
{
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
        quint64 socketId;
        m_pStream >> socketId;
        if (!m_useReceivedId &&
            eService != NetworkInterface::NetworkSerives::ServerSocketInfo)
        {
            socketId = m_SocketID;
        }
        m_pStream >> forwardData;
        QByteArray data;
        m_pStream >> data;
        if (!m_pStream.commitTransaction())
        {
            return;
        }
        if (eService == NetworkInterface::NetworkSerives::ServerSocketInfo)
        {
            Console::print("Updating Socket ID to: " + QString::number(socketId), Console::eLogLevels::eDEBUG);
            m_pIF->setSocketID(socketId);
        }
        else if ((eService < NetworkInterface::NetworkSerives::Game) || (eService >= NetworkInterface::NetworkSerives::Max))
        {
            // don't send an event :)
            Q_ASSERT(false);
        }
        else
        {
            if (m_pIF->getIsServer() && forwardData)
            {
                emit m_pIF->sigForwardData(socketId, data, eService);
            }
            emit m_pIF->recieveData(socketId, data, eService);
        }
    }
}

quint64 RxTask::getSocketID() const
{
    return m_SocketID;
}

void RxTask::setSocketID(const quint64 &SocketID)
{
    m_SocketID = SocketID;
}
