#include <QTcpSocket>

#include "network/rxtask.h"
#include "network/txtask.h"
#include "network/tcpclient.h"

#include "coreengine/mainapp.h"


TCPClient::TCPClient(QObject* pParent)
    : NetworkInterface(pParent),
      m_pRXTask(nullptr),
      m_pTXTask(nullptr),
      m_pSocket(nullptr)
{
    setObjectName("TCPClient");
    m_isServer = false;
}

TCPClient::TCPClient(QObject* pParent, spRxTask pRXTask, spTxTask pTXTask, QTcpSocket* pSocket, quint64 socketId)
    : NetworkInterface(pParent),
      m_pRXTask(pRXTask),
      m_pTXTask(pTXTask),
      m_pSocket(pSocket),
      m_onServer(true)
{
    setObjectName("TCPClient");
    TCPClient::setSocketID(socketId);
    connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
}

TCPClient::~TCPClient()
{
    CONSOLE_PRINT("Client gets closed", Console::eLogLevels::eDEBUG);
    disconnect();
    if (!m_onServer)
    {
        TCPClient::disconnectTCP();
    }
}

void TCPClient::connectTCP(QString adress, quint16 port)
{
    // Launch Socket
    m_pSocket = std::make_shared<QTcpSocket>(this);
    connect(m_pSocket.get(), &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::errorOccurred, this, &TCPClient::displayTCPError, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::connected, this, &TCPClient::connected, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::stateChanged, this, &TCPClient::displayStateChange, Qt::QueuedConnection);

    m_pSocket->connectToHost(adress, port);

    // Start RX-Task
    m_pRXTask = spRxTask::create(m_pSocket, 0, this, false);
    connect(m_pSocket.get(), &QTcpSocket::readyRead, m_pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    m_pTXTask = spTxTask::create(m_pSocket, 0, this, false);
    connect(this, &TCPClient::sig_sendData, m_pTXTask.get(), &TxTask::send, Qt::QueuedConnection);

    CONSOLE_PRINT("Client is running and connecting to " + adress + " and port " + QString(port), Console::eLogLevels::eDEBUG);
}

void TCPClient::disconnectTCP()
{
    CONSOLE_PRINT("TCP Client " + QString::number(m_socketID) + " disconnected.", Console::eLogLevels::eDEBUG);
    if (m_pSocket != nullptr)
    {
        m_pRXTask = nullptr;
        m_pTXTask = nullptr;
        m_pSocket->disconnect();
        m_pSocket->close();
        m_pSocket = nullptr;
    }
    if (!m_onServer)
    {
        emit sigDisconnected(m_socketID);
    }
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
    CONSOLE_PRINT("Client is connected", Console::eLogLevels::eDEBUG);
    m_isConnected = true;
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
