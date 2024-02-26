#include <QIODevice>

#include "network/rxtask.h"
#include "network/networkInterface.h"

RxTask::RxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF)
    : m_pSocket(pSocket),
      m_pIF(CommIF),
      m_SocketID(socketID),
      m_pStream(m_pSocket)
{
    m_pStream.setVersion(QDataStream::Version::Qt_6_5);
    Interpreter::setCppOwnerShip(this);
}

void RxTask::close()
{
    m_pSocket = nullptr;
    m_pIF = nullptr;
}

void RxTask::recieveData()
{
    if (m_pSocket != nullptr &&
        m_pIF != nullptr)
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
            if (eService == NetworkInterface::NetworkSerives::ServerSocketInfo)
            {
                m_SocketID = socketId;
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
                CONSOLE_PRINT("Updating Socket ID to: " + QString::number(socketId), GameConsole::eLogLevels::eDEBUG);
                m_pIF->setSocketID(socketId);
            }
            else if ((eService < NetworkInterface::NetworkSerives::Game) || (eService >= NetworkInterface::NetworkSerives::Max))
            {
                oxygine::handleErrorPolicy(oxygine::ep_show_error, "RxTask::recieveData received unknown service");
            }
            else
            {
                if (m_pIF->getIsServer() && forwardData)
                {
                    emit m_pIF->sigForwardData(socketId, data, eService);
                }
                emit m_pIF->recieveData(socketId, data, eService, m_SocketID);
            }
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
