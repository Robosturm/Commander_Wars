#include <QTcpServer>
#include <QTcpSocket>

#include "coreengine/mainapp.h"

#include "network/tcpserver.h"

TCPServer::TCPServer()
{
    setObjectName("TCPServer");
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = true;
    isConnected = true;
}

TCPServer::~TCPServer()
{
    disconnect();
    TCPServer::disconnectTCP();
    Console::print("Server is closed", Console::eLogLevels::eDEBUG);
}

void TCPServer::connectTCP(QString, quint16 port)
{
    m_pTCPServer = new QTcpServer(this);
    m_pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    m_pTCPServer->listen(QHostAddress::Any, port);
    connect(m_pTCPServer, &QTcpServer::newConnection, this, &TCPServer::onConnect, Qt::QueuedConnection);
    connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient, Qt::QueuedConnection);
    connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);
    connect(this, &TCPServer::sigContinueListening, this, &TCPServer::continueListening, Qt::QueuedConnection);
    connect(this, &TCPServer::sigPauseListening, this, &TCPServer::pauseListening, Qt::QueuedConnection);

    Console::print("TCP Server is running", Console::eLogLevels::eDEBUG);
}

void TCPServer::disconnectTCP()
{
    if (m_pTCPServer != nullptr)
    {
        m_pTCPServer->pauseAccepting();
    }
    while (m_pClients.size() > 0)
    {
        m_pClients[0]->disconnectTCP();
        m_pClients.removeAt(0);
    }
    if (m_pTCPServer != nullptr)
    {
        m_pTCPServer->close();
        m_pTCPServer = nullptr;
    }
}

void TCPServer::disconnectClient(quint64 socketID)
{
    for (qint32 i = 0; i < m_pClients.size(); i++)
    {
        if (m_pClients[i]->getSocketID() == socketID)
        {
            Console::print("Client " + QString::number(socketID) + " disconnected.", Console::eLogLevels::eDEBUG);
            emit m_pClients[i]->sigDisconnected(m_pClients[i]->getSocketID());
            m_pClients[i]->disconnectTCP();
            m_pClients.removeAt(i);            
            emit sigDisconnected(socketID);
            break;
        }
    }
}

void TCPServer::onConnect()
{
    if (m_pTCPServer != nullptr)
    {
        QTcpSocket* nextSocket = m_pTCPServer->nextPendingConnection();
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(nextSocket, &QAbstractSocket::errorOccurred, this, &TCPServer::displayTCPError, Qt::QueuedConnection);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        // Start RX-Task
        spRxTask pRXTask = spRxTask::create(nextSocket, m_idCounter, this, false);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(nextSocket, &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

        // start TX-Task
        spTxTask pTXTask = spTxTask::create(nextSocket, m_idCounter, this, false);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(this, &TCPServer::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
        spTCPClient pClient = spTCPClient::create(pRXTask, pTXTask, nextSocket, m_idCounter);
        connect(pClient.get(), &TCPClient::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);

        quint64 socket = pClient->getSocketID();
        connect(nextSocket, &QTcpSocket::disconnected, [=]()
        {
            emit sigDisconnectClient(socket);
        });

        QByteArray data;
        pTXTask->send(m_idCounter, data, NetworkSerives::ServerSocketInfo, false);
        pClient->setIsServer(true);
        m_pClients.append(pClient);
        Console::print("New Client connection. Socket: " + QString::number(m_idCounter), Console::eLogLevels::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

void TCPServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (qint32 i = 0; i < m_pClients.size(); i++)
    {
        if (m_pClients[i]->getSocketID() != socketID)
        {
            emit m_pClients[i]->sig_sendData(0, data, service, false);
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
    for (qint32 i = 0; i < size; i++)
    {
        socketIds.append(m_pClients[i]->getSocketID());
    }
    return socketIds;
}

void TCPServer::changeThread(quint64 socketID, QThread* pThread)
{
    for (qint32 i = 0; i < m_pClients.size(); i++)
    {
        if (m_pClients[i]->getSocketID() == socketID)
        {
            m_pClients[i]->changeThread(0, pThread);
            break;
        }
    }
}

spTCPClient TCPServer::getClient(quint64 socketID)
{
    for (qint32 i = 0; i < m_pClients.size(); i++)
    {
        if (m_pClients[i]->getSocketID() == socketID)
        {
            return m_pClients[i];
        }
    }
    return nullptr;
}
