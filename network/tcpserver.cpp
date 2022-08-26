#include <QTcpServer>

#include "network/tcpserver.h"

TCPServer::TCPServer(QObject* pParent)
    : NetworkInterface(pParent)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TCPServer");
#endif
    m_isServer = true;
    m_isConnected = true;
    connect(this, &TCPServer::sigSetIsActive, this, &TCPServer::setIsActive, Qt::QueuedConnection);
}

TCPServer::~TCPServer()
{
    disconnect();
    TCPServer::disconnectTCP();
    CONSOLE_PRINT("Server is closed", Console::eLogLevels::eDEBUG);
}

void TCPServer::connectTCP(QString primaryAdress, quint16 port, QString secondaryAdress)
{
    if (secondaryAdress == primaryAdress && !primaryAdress.isEmpty())
    {
        CONSOLE_PRINT("TCP Server launched ignored on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port) + " cause the primary adress is equal to the secondary.", Console::eLogLevels::eFATAL);
    }
    else if (m_pTCPServer[0].get() == nullptr)
    {        
        m_pTCPServer[0] = std::make_shared<QTcpServer>(this);
        if (primaryAdress.isEmpty())
        {
            m_pTCPServer[0]->listen(QHostAddress::Any, port);
        }
        else
        {
            m_pTCPServer[0]->listen(QHostAddress(primaryAdress), port);
        }
        connect(m_pTCPServer[0].get(), &QTcpServer::newConnection, this, &TCPServer::onConnect, Qt::QueuedConnection);
        if (!secondaryAdress.isEmpty())
        {
            m_pTCPServer[1] = std::make_shared<QTcpServer>(this);
            m_pTCPServer[1]->listen(QHostAddress(secondaryAdress), port);
            connect(m_pTCPServer[1].get(), &QTcpServer::newConnection, this, &TCPServer::onConnect, Qt::QueuedConnection);
        }
        connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient, Qt::QueuedConnection);
        connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);
        connect(this, &TCPServer::sigContinueListening, this, &TCPServer::continueListening, Qt::QueuedConnection);
        connect(this, &TCPServer::sigPauseListening, this, &TCPServer::pauseListening, Qt::QueuedConnection);

        CONSOLE_PRINT("TCP Server is running on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port), Console::eLogLevels::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("TCP Server launched ignored on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port) + " cause the server is already running.", Console::eLogLevels::eFATAL);
    }
}

void TCPServer::disconnectTCP()
{
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            pTcpServer->pauseAccepting();
        }
    }
    for (auto & client : m_pClients)
    {
        client->disconnectTCP();
    }
    m_pClients.clear();
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            pTcpServer->close();
            pTcpServer = nullptr;
        }
    }
}

void TCPServer::disconnectClient(quint64 socketID)
{
    auto iter = m_pClients.find(socketID);
    if (iter != m_pClients.end())
    {
        CONSOLE_PRINT("Client " + QString::number(socketID) + " disconnected.", Console::eLogLevels::eDEBUG);
        auto client = iter.value();
        emit client->sigDisconnected(client->getSocketID());
        client->disconnectTCP();
        m_pClients.remove(socketID);
        emit sigDisconnected(socketID);
    }
}

void TCPServer::onConnect()
{
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            auto* nextSocket = pTcpServer->nextPendingConnection();
            if (nextSocket != nullptr)
            {
                connect(nextSocket, &QAbstractSocket::errorOccurred, this, &TCPServer::displayTCPError, Qt::QueuedConnection);
                connect(nextSocket, &QAbstractSocket::stateChanged, this, &TCPServer::displayStateChange, Qt::QueuedConnection);
                m_idCounter++;
                if (m_idCounter == 0)
                {
                    m_idCounter++;
                }
                // Start RX-Task
                spRxTask pRXTask = spRxTask::create(nextSocket, m_idCounter, this, false);
                connect(nextSocket, &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

                // start TX-Task
                spTxTask pTXTask = spTxTask::create(nextSocket, m_idCounter, this, false);
                connect(this, &TCPServer::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
                spTCPClient pClient = spTCPClient::create(this, pRXTask, pTXTask, nextSocket, m_idCounter);
                connect(pClient.get(), &TCPClient::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);

                quint64 socket = pClient->getSocketID();
                connect(nextSocket, &QTcpSocket::disconnected, this, [this, socket]()
                {
                    emit sigDisconnectClient(socket);
                });
                pClient->setIsServer(true);
                m_pClients.insert(m_idCounter, pClient);
                CONSOLE_PRINT("New Client connection. Socket: " + QString::number(m_idCounter), Console::eLogLevels::eDEBUG);
                QByteArray data;
                pTXTask->send(m_idCounter, data, NetworkSerives::ServerSocketInfo, false);
                emit sigConnected(m_idCounter);
            }
        }
    }
}

void TCPServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (auto & client : m_pClients)
    {
        if (client->getSocketID() != socketID &&
            client->getIsActive())
        {
            emit client->sig_sendData(0, data, service, false);
        }
    }
}

void TCPServer::pauseListening()
{
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            pTcpServer->pauseAccepting();
        }
    }
}

void TCPServer::continueListening()
{
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            pTcpServer->resumeAccepting();
        }
    }
}

QVector<quint64> TCPServer::getConnectedSockets()
{
    QVector<quint64> socketIds;
    qint32 size = m_pClients.size();
    socketIds.reserve(size);
    for (auto & client : m_pClients)
    {
        socketIds.append(client->getSocketID());
    }
    return socketIds;
}

void TCPServer::changeThread(quint64 socketID, QThread* pThread)
{
    auto iter = m_pClients.find(socketID);
    if (iter != m_pClients.end())
    {
        iter.value()->changeThread(0, pThread);
    }
}

spTCPClient TCPServer::getClient(quint64 socketID)
{
    auto iter = m_pClients.find(socketID);
    if (iter != m_pClients.end())
    {
        return iter.value();
    }
    return spTCPClient();
}

qint32 TCPServer::getObserverCount()
{
    qint32 count = 0;
    for (auto & client : m_pClients)
    {
        if (client->getIsObserver())
        {
            ++count;
        }
    }
    return count;
}

void TCPServer::setIsActive(quint64 socketID, bool active)
{
    auto client = getClient(socketID);
    if (client.get())
    {
        client->setIsActive(active);
    }
}
