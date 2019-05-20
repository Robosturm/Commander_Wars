#include <QTcpServer>
#include <QTcpSocket>
#include <QMutexLocker>

#include "coreengine/mainapp.h"
#include "network/tcpserver.h"
#include "network/rxtask.h"
#include "network/txtask.h"

TCPServer::TCPServer()
    : pTCPServer(nullptr)
{
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = true;
    isConnected = true;
}



void TCPServer::connectTCP(const QString&, quint16 port)
{
    pTCPServer = new QTcpServer(this);
    pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    pTCPServer->listen(QHostAddress::Any, port);
    QObject::connect(pTCPServer, &QTcpServer::newConnection, this, &TCPServer::onConnect);
    QObject::connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient);
    QObject::connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData);
    Console::print(tr("Server is running"), Console::eDEBUG);
}

TCPServer::~TCPServer()
{
    disconnect();
    disconnectTCP();
    Console::print(tr("Server is closed"), Console::eDEBUG);
}

void TCPServer::disconnectTCP()
{
    QMutexLocker locker(&TaskMutex);
    if (pTCPServer != nullptr)
    {
        pTCPServer->pauseAccepting();
    }
    while (pTCPSockets.size() > 0)
    {
        if (pTCPSockets[0]->isOpen())
        {
            // realize correct deletion
            pTCPSockets[0]->disconnect();
            pTCPSockets[0]->close();
            Console::print(tr("Client disconnected."), Console::eDEBUG);
        }
        pRXTasks.removeAt(0);
        pTXTasks.removeAt(0);
        pTCPSockets.removeAt(0);
    }
    if (pTCPServer != nullptr)
    {
        pTCPServer->close();
        delete pTCPServer;
        pTCPServer = nullptr;
    }
}

void TCPServer::disconnectClient(quint64 socketID)
{
    QMutexLocker locker(&TaskMutex);
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            if (pTCPSockets[i]->isOpen())
            {
                // realize correct deletion
                pTCPSockets[i]->disconnect(this);
                pTCPSockets[i]->close();
            }
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eDEBUG);
            emit sigDisconnected(socketID);
            break;
        }
    }
}

void TCPServer::disconnectSocket()
{
    QMutexLocker locker(&TaskMutex);
    qint32 i = 0;
    while (i < pTCPSockets.size())
    {
        if (pTCPSockets[i]->state() == QAbstractSocket::SocketState::ClosingState ||
            pTCPSockets[i]->state() == QAbstractSocket::SocketState::UnconnectedState)
        {
            // realize correct deletion
            std::shared_ptr<QTcpSocket> pSocket = pTCPSockets[i];
            pTCPSockets[i]->disconnect();
            pTCPSockets[i]->close();
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            quint64 id = m_SocketIDs[i];
            m_SocketIDs.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eDEBUG);
            emit sigDisconnected(id);
        }
        else
        {
            i++;
        }
    }
}

void TCPServer::onConnect()
{
    QMutexLocker locker(&TaskMutex);
    if (pTCPServer != nullptr)
    {
        QTcpSocket* nextSocket = pTCPServer->nextPendingConnection();
        std::shared_ptr<QTcpSocket> pSocket = std::shared_ptr<QTcpSocket>(nextSocket);
        pTCPSockets.append(pSocket);
        pSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(pSocket.get(), &QTcpSocket::disconnected, this, &TCPServer::disconnectSocket, Qt::QueuedConnection);
        QObject::connect(pSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TCPServer::displayError);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        m_SocketIDs.append(m_idCounter);

        // Start RX-Task
        RxTask* pRXTask = new RxTask(pSocket, m_idCounter, this);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pRXTasks.append(pRXTask);
        QObject::connect(pSocket.get(), &QTcpSocket::readyRead, pRXTask, &RxTask::recieveData);

        // start TX-Task
        TxTask* pTXTask = new TxTask(pSocket, m_idCounter, this);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pTXTasks.append(pTXTask);
        QObject::connect(this, &TCPServer::sig_sendData, pTXTask, &TxTask::send);

        Console::print(tr("New Client connection."), Console::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

QTcpSocket* TCPServer::getSocket(quint64 socketID)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            return pTCPSockets[i].get();
        }
    }
    return nullptr;
}

void TCPServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    QMutexLocker locker(&TaskMutex);
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] != socketID)
        {
            pTXTasks[i]->send(0, data, service, false);
        }
    }
}
