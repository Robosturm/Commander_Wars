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
    pSocket = new QTcpSocket(this);
    pSocket->moveToThread(this);
    QObject::connect(pSocket, &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    QObject::connect(pSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TCPClient::displayError);
    pSocket->connectToHost(adress, port);

    // Start RX-Task
    pRXTask = new RxTask(pSocket, this);
    pRXTask->moveToThread(this);
    QObject::connect(pSocket, &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData);

    // start TX-Task
    pTXTask = new TxTask(pSocket, this);
    pTXTask->moveToThread(this);
    QObject::connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send);

    isConnected = true;
    Console::print(tr("Client is running"), Console::eDEBUG);
    emit sigConnected();
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
        delete pSocket;
        pSocket = nullptr;
        delete networkSession;
        networkSession = nullptr;
    }
}

void TCPClient::sendData(QByteArray data, Mainapp::NetworkSerives service, bool forwardData)
{
    emit sig_sendData(data, service, forwardData);
}
