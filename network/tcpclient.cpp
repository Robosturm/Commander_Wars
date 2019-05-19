#include <QTcpSocket>

#include "network/rxtask.h"
#include "network/txtask.h"
#include "coreengine/mainapp.h"
#include "network/tcpclient.h"


TCPClient::TCPClient()
    : pRXTask(nullptr),
      pTXTask(nullptr),
      pSocket(nullptr)
{
    isServer = false;
    start();
}

TCPClient::~TCPClient()
{
}

void TCPClient::connectTCP(const QString& adress, quint16 port)
{
    // Launch Socket
    pSocket = std::shared_ptr<QTcpSocket>(new QTcpSocket(this));
    pSocket->moveToThread(this);
    QObject::connect(pSocket.get(), &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    QObject::connect(pSocket.get(), QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TCPClient::displayError);
    pSocket->connectToHost(adress, port);

    // Start RX-Task
    pRXTask = new RxTask(pSocket, 0, this);
    pRXTask->moveToThread(this);
    QObject::connect(pSocket.get(), &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData);

    // start TX-Task
    pTXTask = new TxTask(pSocket, 0, this);
    pTXTask->moveToThread(this);
    QObject::connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send);

    isConnected = true;
    Console::print(tr("Client is running"), Console::eDEBUG);
    emit sigConnected(0);
}

void TCPClient::disconnectTCP()
{
    if (pSocket != nullptr)
    {
        pRXTask->disconnect();
        pRXTask = nullptr;
        pTXTask->disconnect();
        pTXTask = nullptr;
        pSocket->disconnect();
        pSocket->close();
        pSocket = nullptr;
        delete networkSession;
        networkSession = nullptr;
    }
    emit sigDisconnected(0);
}

void TCPClient::sendData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData)
{
    emit sig_sendData(socketID, data, service, forwardData);
}
