#include <QTcpServer>
#include <QTcpSocket>

#include "coreengine/mainapp.h"
#include "network/tcpserver.h"

TCPServer::TCPServer()
{
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = true;
    isConnected = true;
}

TCPServer::~TCPServer()
{
    disconnect();
    disconnectTCP();
    Console::print("Server is closed", Console::eLogLevels::eDEBUG);
}

void TCPServer::connectTCP(QString, quint16 port)
{
    pTCPServer = new QTcpServer(this);
    pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    pTCPServer->listen(QHostAddress::Any, port);
    QObject::connect(pTCPServer, &QTcpServer::newConnection, this, &TCPServer::onConnect, Qt::QueuedConnection);
    QObject::connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient, Qt::QueuedConnection);
    QObject::connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);
    QObject::connect(this, &TCPServer::sigContinueListening, this, &TCPServer::continueListening, Qt::QueuedConnection);
    QObject::connect(this, &TCPServer::sigPauseListening, this, &TCPServer::pauseListening, Qt::QueuedConnection);

    Console::print("Server is running", Console::eLogLevels::eDEBUG);
}

void TCPServer::disconnectTCP()
{
    if (pTCPServer != nullptr)
    {
        pTCPServer->pauseAccepting();
    }
    while (m_pClients.size() > 0)
    {
        m_pClients[0]->disconnectTCP();
        m_pClients.removeAt(0);
    }
    if (pTCPServer != nullptr)
    {
        pTCPServer->close();
        pTCPServer = nullptr;
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

void TCPServer::disconnectSocket()
{
    while (m_pClients.size() > 0)
    {
        emit m_pClients[0]->sigDisconnected(m_pClients[0]->getSocketID());
        m_pClients[0]->disconnectTCP();
        quint64 id = m_pClients[0]->getSocketID();
        m_pClients.removeAt(0);
        emit sigDisconnected(id);
    }
}

void TCPServer::onConnect()
{
    if (pTCPServer != nullptr)
    {
        QTcpSocket* nextSocket = pTCPServer->nextPendingConnection();
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(nextSocket, &QTcpSocket::disconnected, this, &TCPServer::disconnectSocket, Qt::QueuedConnection);
        QObject::connect(nextSocket, &QAbstractSocket::errorOccurred, this, &TCPServer::displayTCPError, Qt::QueuedConnection);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        // Start RX-Task
        RxTask* pRXTask = new RxTask(nextSocket, m_idCounter, this, false);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(nextSocket, &QTcpSocket::readyRead, pRXTask, &RxTask::recieveData, Qt::QueuedConnection);

        // start TX-Task
        TxTask* pTXTask = new TxTask(nextSocket, m_idCounter, this, false);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(this, &TCPServer::sig_sendData, pTXTask, &TxTask::send, Qt::QueuedConnection);
        spTCPClient pClient = new TCPClient(pRXTask, pTXTask, nextSocket, m_idCounter);
        QObject::connect(pClient.get(), &TCPClient::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);

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
    pTCPServer->pauseAccepting();
}

void TCPServer::continueListening()
{
    pTCPServer->resumeAccepting();
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
