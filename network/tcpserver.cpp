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
    Console::print(tr("Server is closed"), Console::eLogLevels::eDEBUG);
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

    Console::print(tr("Server is running"), Console::eLogLevels::eDEBUG);
}

void TCPServer::disconnectTCP()
{
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
    if (pTCPServer != nullptr)
    {
        QTcpSocket* nextSocket = pTCPServer->nextPendingConnection();
        pTCPSockets.append(nextSocket);
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(nextSocket, &QTcpSocket::disconnected, this, &TCPServer::disconnectSocket, Qt::QueuedConnection);
        QObject::connect(nextSocket, &QAbstractSocket::errorOccurred, this, &TCPServer::displayTCPError, Qt::QueuedConnection);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        m_SocketIDs.append(m_idCounter);

        // Start RX-Task
        RxTask* pRXTask = new RxTask(nextSocket, m_idCounter, this, false);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pRXTasks.append(pRXTask);
        QObject::connect(nextSocket, &QTcpSocket::readyRead, pRXTask, &RxTask::recieveData, Qt::QueuedConnection);
        QObject::connect(pRXTask, &RxTask::sigForwardData, this, &TCPServer::forwardData, Qt::QueuedConnection);

        // start TX-Task
        TxTask* pTXTask = new TxTask(nextSocket, m_idCounter, this, false);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pTXTasks.append(pTXTask);
        QObject::connect(this, &TCPServer::sig_sendData, pTXTask, &TxTask::send, Qt::QueuedConnection);

        Console::print(tr("New Client connection."), Console::eLogLevels::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

void TCPServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
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

void TCPServer::changeThread(quint64 socketID, QThread* pThread)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            pRXTasks[i]->moveToThread(pThread);
            pTXTasks[i]->moveToThread(pThread);
            break;
        }
    }
}
