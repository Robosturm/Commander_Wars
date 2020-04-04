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



void TCPServer::connectTCP(QString, quint16 port)
{
    pTCPServer = new QTcpServer(this);
    pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    pTCPServer->listen(QHostAddress::Any, port);
    QObject::connect(pTCPServer, &QTcpServer::newConnection, this, &TCPServer::onConnect);
    QObject::connect(this, &TCPServer::sigDisconnectClient, this, &TCPServer::disconnectClient);
    QObject::connect(this, &TCPServer::sigForwardData, this, &TCPServer::forwardData);
    Console::print(tr("Server is running"), Console::eLogLevels::eDEBUG);
}

TCPServer::~TCPServer()
{
    disconnect();
    disconnectTCP();
    Console::print(tr("Server is closed"), Console::eLogLevels::eDEBUG);
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
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
        }
        pRXTasks.removeAt(0);
        pTXTasks.removeAt(0);
        pTCPSockets.removeAt(0);
    }
    if (pTCPServer != nullptr)
    {
        pTCPServer->close();
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
                pTCPSockets[i]->disconnectFromHost();
                pTCPSockets[i]->close();
            }
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
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
            pTCPSockets[i]->disconnect();
            pTCPSockets[i]->close();
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            quint64 id = m_SocketIDs[i];
            m_SocketIDs.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
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
        pTCPSockets.append(nextSocket);
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(nextSocket, &QTcpSocket::disconnected, this, &TCPServer::disconnectSocket, Qt::QueuedConnection);
        QObject::connect(nextSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TCPServer::displayError);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        m_SocketIDs.append(m_idCounter);

        // Start RX-Task
        RxTask* pRXTask = new RxTask(nextSocket, m_idCounter, this);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pRXTasks.append(pRXTask);
        QObject::connect(nextSocket, &QTcpSocket::readyRead, pRXTask, &RxTask::recieveData);

        // start TX-Task
        TxTask* pTXTask = new TxTask(nextSocket, m_idCounter, this);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pTXTasks.append(pTXTask);
        QObject::connect(this, &TCPServer::sig_sendData, pTXTask, &TxTask::send);

        Console::print(tr("New Client connection."), Console::eLogLevels::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

QTcpSocket* TCPServer::getSocket(quint64 socketID)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            return pTCPSockets[i];
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
    return m_SocketIDs;
}
