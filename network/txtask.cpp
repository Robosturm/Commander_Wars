#include <QIODevice>
#include <QDataStream>

#include "coreengine/gameconsole.h"

#include "network/txtask.h"
#include "network/networkInterface.h"

TxTask::TxTask(QIODevice* pSocket, quint64 socketID, NetworkInterface* CommIF, bool sendAll)
 : m_pSocket(pSocket),
   m_SocketID(socketID),
   m_pIF(CommIF),
   m_sendAll(sendAll)
{
    setObjectName("TxTask");
    Interpreter::setCppOwnerShip(this);
}

void TxTask::close()
{
    m_pSocket = nullptr;
    m_pIF = nullptr;
}

void TxTask::send(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData)
{
    if (m_pSocket != nullptr &&
        m_pIF != nullptr)
    {
        bool open = m_pSocket->isOpen();
        if (open &&
            (m_SocketID == socketID ||
             socketID == 0 ||
             m_sendAll))
        {
            QByteArray block;
            QDataStream out(&block, QIODevice::WriteOnly);
            out.setVersion(QDataStream::Version::Qt_6_5);
            // write default-buffersize
            out << static_cast<qint32>(service);
            out << socketID;
            out << forwardData;
            //write Object
            out << data;
            // send data this function should be blocking.
            if (m_pSocket->write(block) != block.size())
            {
                CONSOLE_PRINT("Error during writing data via TCP.", GameConsole::eERROR);
            }
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
