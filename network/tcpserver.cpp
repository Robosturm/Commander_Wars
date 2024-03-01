#include <QSslServer>
#include <QSslSocket>

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
    CONSOLE_PRINT("Server is closed", GameConsole::eLogLevels::eDEBUG);
}

void TCPServer::connectTCP(QString primaryAdress, quint16 port, QString secondaryAdress, bool sendAll)
{
    if (secondaryAdress == primaryAdress && !primaryAdress.isEmpty())
    {
        CONSOLE_PRINT("TCP Server launched ignored on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port) + " cause the primary adress is equal to the secondary.", GameConsole::eLogLevels::eERROR);
    }
    else if (m_pTCPServer[0].get() == nullptr)
    {    

        m_pTCPServer[0] = MemoryManagement::createNamedQObject<QSslServer>("QSslServer", this);
            m_pTCPServer[0]->setSslConfiguration(getSslConfiguration());
        if (primaryAdress.isEmpty())
        {
            m_pTCPServer[0]->listen(QHostAddress::Any, port);
        }
        else
        {
            m_pTCPServer[0]->listen(QHostAddress(primaryAdress), port);
        }
        connect(m_pTCPServer[0].get(), &QTcpServer::pendingConnectionAvailable, this, &TCPServer::onConnect, Qt::QueuedConnection);
        if (!secondaryAdress.isEmpty())
        {
            m_pTCPServer[1] = MemoryManagement::createNamedQObject<QSslServer>("QSslServer", this);
            m_pTCPServer[1]->setSslConfiguration(getSslConfiguration());
            m_pTCPServer[1]->listen(QHostAddress(secondaryAdress), port);
            connect(m_pTCPServer[1].get(), &QTcpServer::pendingConnectionAvailable, this, &TCPServer::onConnect, Qt::QueuedConnection);
        }
        connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient, Qt::QueuedConnection);
        connect(this, &TCPServer::sigDisconnectTCP, this, &TCPServer::disconnectTCP, Qt::QueuedConnection);
        connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);
        connect(this, &TCPServer::sigContinueListening, this, &TCPServer::continueListening, Qt::QueuedConnection);
        connect(this, &TCPServer::sigPauseListening, this, &TCPServer::pauseListening, Qt::QueuedConnection);

        CONSOLE_PRINT("TCP Server is running on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port), GameConsole::eLogLevels::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("TCP Server launched ignored on primary adress \"" + primaryAdress + "\" and secondary adress \""+ secondaryAdress + "\" and port " + QString::number(port) + " cause the server is already running.", GameConsole::eLogLevels::eERROR);
    }
}

void TCPServer::disconnectTCP()
{
    CONSOLE_PRINT("Server disconnectTCP for all clients", GameConsole::eLogLevels::eDEBUG);
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
        CONSOLE_PRINT("Client " + QString::number(socketID) + " disconnected.", GameConsole::eLogLevels::eDEBUG);
        auto client = iter.value();
        client->disconnectTCP();
        emit client->sigDisconnected(client->getSocketID());
        emit sigDisconnected(socketID);
        m_pClients.remove(socketID);
    }
}

void TCPServer::onConnect()
{
    for (auto & pTcpServer : m_pTCPServer)
    {
        if (pTcpServer != nullptr)
        {
            spQSslSocket nextSocket = MemoryManagement::createNamedFromPointer("QSslSocket", dynamic_cast<QSslSocket*>(pTcpServer->nextPendingConnection()));
            if (nextSocket.get() != nullptr)
            {
                connect(nextSocket.get(), &QAbstractSocket::errorOccurred, this, &TCPServer::displayTCPError, Qt::QueuedConnection);
                connect(nextSocket.get(), &QAbstractSocket::stateChanged, this, &TCPServer::displayStateChange, Qt::QueuedConnection);
                m_idCounter++;
                if (m_idCounter == 0)
                {
                    m_idCounter++;
                }
                // Start RX-Task
                spRxTask pRXTask = MemoryManagement::create<RxTask>(nextSocket.get(), m_idCounter, this, m_useReceivedId);
                connect(nextSocket.get(), &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

                // start TX-Task
                spTxTask pTXTask = MemoryManagement::create<TxTask>(nextSocket.get(), m_idCounter, this, false);
                connect(this, &TCPServer::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
                spTCPClient pClient = MemoryManagement::create<TCPClient>(this, pRXTask, pTXTask, nextSocket, m_idCounter);
                connect(pClient.get(), &TCPClient::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);

                quint64 socket = pClient->getSocketID();
                connect(nextSocket.get(), &QTcpSocket::disconnected, this, [this, socket]()
                {
                    emit sigDisconnectClient(socket);
                });
                pClient->setIsServer(true);
                m_pClients.insert(m_idCounter, pClient);
                CONSOLE_PRINT("Client connection. Socket: " + QString::number(m_idCounter), GameConsole::eLogLevels::eDEBUG);
                QByteArray data;
                pTXTask->send(m_idCounter, data, NetworkSerives::ServerSocketInfo, false);
                emit sigConnected(m_idCounter);
            }
            else
            {
                CONSOLE_PRINT("Failed to receive valid QSslSocket from QSslServer", GameConsole::eLogLevels::eERROR);
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
            emit client->sig_sendData(socketID, data, service, false);
        }
    }
}

void TCPServer::pauseListening()
{
    CONSOLE_PRINT("TCPServer pauseListening" + QString::number(m_idCounter), GameConsole::eLogLevels::eDEBUG);
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
    CONSOLE_PRINT("TCPServer continueListening" + QString::number(m_idCounter), GameConsole::eLogLevels::eDEBUG);
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
