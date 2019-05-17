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
    moveToThread(this);
    isServer = true;
    isConnected = true;
    start();
}



void TCPServer::connectTCP(const QString&, quint16 port)
{
    pTCPServer = new QTcpServer(this);
    pTCPServer->moveToThread(this);
    pTCPServer->listen(QHostAddress::Any, port);
    QObject::connect(pTCPServer, &QTcpServer::newConnection, this, &TCPServer::onConnect);

    Console::print(tr("Server is running"), Console::eDEBUG);
}

TCPServer::~TCPServer()
{
}

void TCPServer::disconnectTCP()
{
    QMutexLocker locker(&TaskMutex);
    if (pTCPServer != nullptr)
    {
        pTCPServer->close();
        delete pTCPServer;
        pTCPServer = nullptr;
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
    delete networkSession;
    networkSession = nullptr;
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
            Console::print(tr("Client disconnected."), Console::eDEBUG);
        }
    }
}

void TCPServer::onConnect()
{
    QMutexLocker locker(&TaskMutex);
    QTcpSocket* nextSocket = pTCPServer->nextPendingConnection();
    std::shared_ptr<QTcpSocket> pSocket = std::shared_ptr<QTcpSocket>(nextSocket);
    pTCPSockets.append(pSocket);

    pSocket->moveToThread(this);

    QObject::connect(pSocket.get(), &QTcpSocket::disconnected, this, &TCPServer::disconnectSocket, Qt::QueuedConnection);
    QObject::connect(pSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TCPServer::displayError);

    // Start RX-Task
    RxTask* pRXTask = new RxTask(pSocket, this);
    pRXTask->moveToThread(this);
    pRXTasks.append(pRXTask);
    QObject::connect(pSocket.get(), &QTcpSocket::readyRead, pRXTask, &RxTask::recieveData);

    // start TX-Task
    TxTask* pTXTask = new TxTask(pSocket, this);
    pTXTask->moveToThread(this);
    pTXTasks.append(pTXTask);
    QObject::connect(this, &TCPServer::sig_sendData, pTXTask, &TxTask::send);

    Console::print(tr("New Client connection."), Console::eDEBUG);
    emit sigConnected(pSocket);
}

void TCPServer::sendData(std::shared_ptr<QTcpSocket> pSocket, QByteArray data, NetworkSerives service, bool forwardData)
{
    QMutexLocker locker(&TaskMutex);
    emit sig_sendData(pSocket, data, service, forwardData);
}

void TCPServer::forwardData(std::shared_ptr<QTcpSocket> pSocket, QByteArray data, NetworkSerives service)
{
    QMutexLocker locker(&TaskMutex);
    for (qint32 i = 0; i < pTCPSockets.size(); i++)
    {
        if (pTCPSockets[i] != pSocket)
        {
            pTXTasks[i]->send(pTCPSockets[i], data, service, false);
        }
    }
}
