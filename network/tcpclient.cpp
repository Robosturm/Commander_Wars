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
}

TCPClient::~TCPClient()
{
    disconnectTCP();
}

void TCPClient::connectTCP(const QString& adress, quint16 port)
{
    disconnectTCP();
    // Launch Socket
    pSocket = new QTcpSocket(this);
    QObject::connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnectTCP()));
    QObject::connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    pSocket->connectToHost(adress, port);
    pSocket->moveToThread(this);

    // Start RX-Task
    pRXTask = new RxTask(pSocket, this);
    pRXTask->moveToThread(this);
    QObject::connect(pSocket, SIGNAL(readyRead()), pRXTask, SLOT(recieveData()));

    // start TX-Task
    pTXTask = new TxTask(pSocket, this);
    pTXTask->moveToThread(this);
    QObject::connect(this, SIGNAL(sig_sendData(QByteArray,Mainapp::NetworkSerives, bool)), pTXTask, SLOT(send(QByteArray,Mainapp::NetworkSerives, bool)));

    isConnected = true;
    Console::print(tr("Client is running"), Console::eDEBUG);
}

void TCPClient::disconnectTCP()
{
    if (pRXTask != nullptr)
    {
        delete pRXTask;
        pRXTask = nullptr;
    }
    if (pTXTask != nullptr)
    {
        delete pTXTask;
        pTXTask = nullptr;
    }
    if (pSocket != nullptr)
    {
        pSocket->abort();
        // realize correct deletion
        pSocket->deleteLater();
    }
    pSocket = nullptr;
}

void TCPClient::sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking)
{
    emit sig_sendData(data, service, blocking);
}

void TCPClient::sessionOpened(quint16)
{
}
