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
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = false;
}

TCPClient::~TCPClient()
{
    disconnect();
    disconnectTCP();
    Console::print(tr("Client is closed"), Console::eLogLevels::eDEBUG);
}

void TCPClient::connectTCP(QString adress, quint16 port)
{
    // Launch Socket
    pSocket = new QTcpSocket(this);
    pSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(pSocket, &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    QObject::connect(pSocket, &QAbstractSocket::errorOccurred, this, &TCPClient::displayTCPError, Qt::QueuedConnection);
    QObject::connect(pSocket, &QAbstractSocket::connected, this, &TCPClient::connected, Qt::QueuedConnection);

    pSocket->connectToHost(adress, port);

    // Start RX-Task
    pRXTask = new RxTask(pSocket, 0, this, false);
    pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(pSocket, &QTcpSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    pTXTask = new TxTask(pSocket, 0, this, false);
    pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);

    Console::print(tr("Client is running"), Console::eLogLevels::eDEBUG);
}

void TCPClient::disconnectTCP()
{
    if (pSocket != nullptr)
    {
        pRXTask = nullptr;
        pTXTask = nullptr;
        pSocket->disconnect();
        pSocket->close();
        pSocket = nullptr;
    }
    emit sigDisconnected(0);
}

QVector<quint64> TCPClient::getConnectedSockets()
{
    return QVector<quint64>();
}

void TCPClient::changeThread(quint64, QThread* pThread)
{
    pRXTask->moveToThread(pThread);
    pTXTask->moveToThread(pThread);
}

void TCPClient::connected()
{
    Console::print(tr("Client is connected"), Console::eLogLevels::eDEBUG);
    isConnected = true;
    emit sigConnected(0);
}
