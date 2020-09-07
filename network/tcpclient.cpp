#include <QTcpSocket>

#include "network/rxtask.h"
#include "network/txtask.h"
#include "coreengine/mainapp.h"
#include "network/tcpclient.h"


TCPClient::TCPClient()
    : m_pRXTask(nullptr),
      m_pTXTask(nullptr),
      m_pSocket(nullptr)
{
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = false;
}

TCPClient::TCPClient(spRxTask pRXTask, spTxTask pTXTask, QTcpSocket* pSocket, quint64 socketId)
    : m_pRXTask(pRXTask),
      m_pTXTask(pTXTask),
      m_pSocket(pSocket),
      m_onServer(true)
{
    setSocketID(socketId);
    QObject::connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
}

TCPClient::~TCPClient()
{
    disconnect();
    if (!m_onServer)
    {
        disconnectTCP();
    }
    Console::print("Client is closed", Console::eLogLevels::eDEBUG);
}

void TCPClient::connectTCP(QString adress, quint16 port)
{
    // Launch Socket
    m_pSocket = new QTcpSocket(this);
    m_pSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(m_pSocket, &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    QObject::connect(m_pSocket, &QAbstractSocket::errorOccurred, this, &TCPClient::displayTCPError, Qt::QueuedConnection);
    QObject::connect(m_pSocket, &QAbstractSocket::connected, this, &TCPClient::connected, Qt::QueuedConnection);

    m_pSocket->connectToHost(adress, port);

    // Start RX-Task
    m_pRXTask = new RxTask(m_pSocket, 0, this, false);
    m_pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(m_pSocket, &QTcpSocket::readyRead, m_pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    m_pTXTask = new TxTask(m_pSocket, 0, this, false);
    m_pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
    QObject::connect(this, &TCPClient::sig_sendData, m_pTXTask.get(), &TxTask::send, Qt::QueuedConnection);

    Console::print("Client is running", Console::eLogLevels::eDEBUG);
}

void TCPClient::disconnectTCP()
{
    Console::print("TCP Client disconnected.", Console::eLogLevels::eDEBUG);
    if (m_pSocket != nullptr)
    {
        m_pRXTask = nullptr;
        m_pTXTask = nullptr;
        m_pSocket->disconnect();
        m_pSocket->close();
        m_pSocket = nullptr;
    }
    emit sigDisconnected(0);
}

QVector<quint64> TCPClient::getConnectedSockets()
{
    return QVector<quint64>();
}

void TCPClient::changeThread(quint64, QThread* pThread)
{
    moveToThread(pThread);
    if (m_pRXTask.get() != nullptr)
    {
        m_pRXTask->moveToThread(pThread);
    }
    if (m_pTXTask.get() != nullptr)
    {
        m_pTXTask->moveToThread(pThread);
    }
}

void TCPClient::connected()
{
    Console::print("Client is connected", Console::eLogLevels::eDEBUG);
    isConnected = true;
    emit sigConnected(0);
}

spTxTask TCPClient::getTXTask() const
{
    return m_pTXTask;
}


spRxTask TCPClient::getRXTask() const
{
    return m_pRXTask;
}

void TCPClient::setSocketID(const quint64 &socketID)
{
    NetworkInterface::setSocketID(socketID);
    m_pRXTask->setSocketID(socketID);
    m_pTXTask->setSocketID(socketID);
}
