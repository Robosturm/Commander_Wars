#include <qlocalsocket.h>

#include "network/localclient.h"
#include "network/rxtask.h"
#include "network/txtask.h"
#include "coreengine/mainapp.h"

LocalClient::LocalClient()
    : m_pRXTask(nullptr),
      m_pTXTask(nullptr),
      m_pSocket(nullptr)
{
    setObjectName("LocalClient");
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = false;
}

LocalClient::~LocalClient()
{
    disconnect();
    LocalClient::disconnectTCP();
    Console::print("Client is closed", Console::eLogLevels::eDEBUG);
}

void LocalClient::connectTCP(QString adress, quint16)
{
    // Launch Socket
    m_pSocket = new QLocalSocket(this);
    m_pSocket->setObjectName("LocalclientSocket");
    m_pSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
    connect(m_pSocket, &QLocalSocket::disconnected, this, &LocalClient::disconnectTCP, Qt::QueuedConnection);
    connect(m_pSocket, &QLocalSocket::errorOccurred, this, &LocalClient::displayLocalError, Qt::QueuedConnection);
    connect(m_pSocket, &QLocalSocket::connected, this, &LocalClient::connected, Qt::QueuedConnection);

    // Start RX-Task
    m_pRXTask = spRxTask::create(m_pSocket, 0, this, true);
    m_pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    connect(m_pSocket, &QLocalSocket::readyRead, m_pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    m_pTXTask = spTxTask::create(m_pSocket, 0, this, true);
    m_pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    connect(this, &LocalClient::sig_sendData, m_pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
    Console::print("Local Client is running to " + adress, Console::eLogLevels::eDEBUG);
    do
    {
        m_pSocket->connectToServer(adress);
    } while (!m_pSocket->waitForConnected(10000));
}

void LocalClient::disconnectTCP()
{
    if (m_pSocket != nullptr)
    {
        m_pRXTask = nullptr;
        m_pTXTask = nullptr;
        m_pSocket->disconnect();
        m_pSocket->close();
        m_pSocket = nullptr;
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
    m_pRXTask->moveToThread(pThread);
    m_pTXTask->moveToThread(pThread);
}

void LocalClient::connected()
{
    Console::print("Client is connected", Console::eLogLevels::eDEBUG);
    isConnected = true;
    emit sigConnected(0);
}
