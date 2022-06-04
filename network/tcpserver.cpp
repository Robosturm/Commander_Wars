#include <QTcpServer>
#include <QSslSocket>

#include "network/tcpserver.h"
#include "network/sslserver.h"

TCPServer::TCPServer(QObject* pParent)
    : NetworkInterface(pParent)
{
    setObjectName("TCPServer");
    m_isServer = true;
    m_isConnected = true;
}

TCPServer::~TCPServer()
{
    disconnect();
    TCPServer::disconnectTCP();
    CONSOLE_PRINT("Server is closed", Console::eLogLevels::eDEBUG);
}

void TCPServer::connectTCP(QString adress, quint16 port, bool secure)
{
    if (QSslSocket::supportsSsl() && secure)
    {
        CONSOLE_PRINT("Ssl not supported, aborting connection", Console::eWARNING);
        return;
    }

    if (m_pTCPServer.get() == nullptr)
    {
        m_secure = secure;
        if (m_secure)
        {
            m_pTCPServer = std::make_shared<SslServer>(this);
        }
        else
        {
            m_pTCPServer = std::make_shared<QTcpServer>(this);
        }
        if (adress.isEmpty())
        {
            m_pTCPServer->listen(QHostAddress::Any, port);
        }
        else
        {
            m_pTCPServer->listen(QHostAddress(adress), port);
        }
        connect(m_pTCPServer.get(), &QTcpServer::newConnection, this, &TCPServer::onConnect, Qt::QueuedConnection);
        connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient, Qt::QueuedConnection);
        connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);
        connect(this, &TCPServer::sigContinueListening, this, &TCPServer::continueListening, Qt::QueuedConnection);
        connect(this, &TCPServer::sigPauseListening, this, &TCPServer::pauseListening, Qt::QueuedConnection);
        CONSOLE_PRINT("TCP Server is running on adress " + adress + " and port " + QString::number(port) + " and the connection is " + (m_secure ? "secure" : "unsecure"), Console::eLogLevels::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("TCP Server launched ignored on adress " + adress + " and port " + QString::number(port) + " cause the server is already running.", Console::eLogLevels::eFATAL);
    }
}

void TCPServer::disconnectTCP()
{
    if (m_pTCPServer != nullptr)
    {
        m_pTCPServer->pauseAccepting();
    }
    for (auto & client : m_pClients)
    {
        client->disconnectTCP();
    }
    m_pClients.clear();
    if (m_pTCPServer != nullptr)
    {
        m_pTCPServer->close();
        m_pTCPServer = nullptr;
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
    if (m_pTCPServer != nullptr)
    {
        auto* nextSocket = m_pTCPServer->nextPendingConnection();
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
            if (m_secure)
            {
                auto* socket = dynamic_cast<QSslSocket*>(nextSocket);
                quint64 currentId = m_idCounter;
                if (socket != nullptr)
                {
                    connect(socket, &QSslSocket::encrypted, this, [this, currentId]()
                    {
                        ready(currentId);
                    }, Qt::QueuedConnection);
                    socket->startServerEncryption();
                }
                else
                {
                    disconnectClient(currentId);
                }
            }
            else
            {
                ready(m_idCounter);
            }
        }
    }
}

void TCPServer::ready(quint64 idCounter)
{
    auto client = getClient(idCounter);
    if (client.get())
    {
        QByteArray data;
        client->sig_sendData(idCounter, data, NetworkSerives::ServerSocketInfo, false);
        emit sigConnected(idCounter);
    }
}

void TCPServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (auto & client : m_pClients)
    {
        if (client->getSocketID() != socketID)
        {
            emit client->sig_sendData(0, data, service, false);
        }
    }
}

void TCPServer::pauseListening()
{
    m_pTCPServer->pauseAccepting();
}

void TCPServer::continueListening()
{
    m_pTCPServer->resumeAccepting();
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
