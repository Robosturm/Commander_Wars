#include <qlocalsocket.h>

#include "network/localclient.h"
#include "network/rxtask.h"
#include "network/txtask.h"
#include "coreengine/mainapp.h"

LocalClient::LocalClient()
    : pRXTask(nullptr),
      pTXTask(nullptr),
      pSocket(nullptr)
{
    setObjectName("LocalClient");
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = false;
}

LocalClient::~LocalClient()
{
    disconnect();
    disconnectTCP();
    Console::print("Client is closed", Console::eLogLevels::eDEBUG);
}

void LocalClient::connectTCP(QString adress, quint16)
{
    // Launch Socket
    pSocket = new QLocalSocket(this);
    pSocket->setObjectName("LocalclientSocket");
    pSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(pSocket, &QLocalSocket::disconnected, this, &LocalClient::disconnectTCP, Qt::QueuedConnection);
    QObject::connect(pSocket, &QLocalSocket::errorOccurred, this, &LocalClient::displayLocalError, Qt::QueuedConnection);
    QObject::connect(pSocket, &QLocalSocket::connected, this, &LocalClient::connected, Qt::QueuedConnection);

    // Start RX-Task
    pRXTask = spRxTask::create(pSocket, 0, this, true);
    pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(pSocket, &QLocalSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    pTXTask = spTxTask::create(pSocket, 0, this, true);
    pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(this, &LocalClient::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
    Console::print("Local Client is running to " + adress, Console::eLogLevels::eDEBUG);
    do
    {
        pSocket->connectToServer(adress);
    } while (!pSocket->waitForConnected(10000));
}

void LocalClient::disconnectTCP()
{
    if (pSocket != nullptr)
    {
        pRXTask = nullptr;
        pTXTask = nullptr;
        pSocket->disconnect();
        pSocket->close();
        pSocket = nullptr;
    }
    Console::print("Local Client disconnected.", Console::eDEBUG);
    emit sigDisconnected(0);
}

QVector<quint64> LocalClient::getConnectedSockets()
{
    return QVector<quint64>();
}

void LocalClient::changeThread(quint64, QThread* pThread)
{
    moveToThread(pThread);
    pRXTask->moveToThread(pThread);
    pTXTask->moveToThread(pThread);
}

void LocalClient::connected()
{
    Console::print("Client is connected", Console::eLogLevels::eDEBUG);
    isConnected = true;
    emit sigConnected(0);
}
