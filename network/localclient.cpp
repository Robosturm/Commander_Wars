#include <QLocalSocket>

#include "network/localclient.h"
#include "network/rxtask.h"
#include "network/txtask.h"

LocalClient::LocalClient(QObject* pParent)
    : NetworkInterface(pParent),
      m_pRXTask(nullptr),
      m_pTXTask(nullptr),
      m_pSocket(nullptr)
{
    setObjectName("LocalClient");
    m_isServer = false;
}

LocalClient::~LocalClient()
{
    disconnect();
    LocalClient::disconnectTCP();
    CONSOLE_PRINT("Client is closed", GameConsole::eLogLevels::eDEBUG);
}

void LocalClient::connectTCP(QString primaryAdress, quint16 port, QString secondaryAdress)
{
    // Launch Socket
    m_pSocket = MemoryManagement::create<QLocalSocket>(this);
    m_pSocket->setObjectName("LocalclientSocket");
    connect(m_pSocket.get(), &QLocalSocket::disconnected, this, &LocalClient::disconnectTCP, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QLocalSocket::errorOccurred, this, &LocalClient::displayLocalError, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QLocalSocket::connected, this, &LocalClient::connected, Qt::QueuedConnection);
    connect(this, &LocalClient::sigDisconnectTCP, this, &LocalClient::disconnectTCP, Qt::QueuedConnection);

    // Start RX-Task
    m_pRXTask = MemoryManagement::create<RxTask>(m_pSocket.get(), 0, this, true);
    connect(m_pSocket.get(), &QLocalSocket::readyRead, m_pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    m_pTXTask = MemoryManagement::create<TxTask>(m_pSocket.get(), 0, this, true);
    connect(this, &LocalClient::sig_sendData, m_pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
    CONSOLE_PRINT("Local Client is running to " + primaryAdress, GameConsole::eLogLevels::eDEBUG);
    do
    {
        m_pSocket->connectToServer(primaryAdress);
    } while (!m_pSocket->waitForConnected(10000));
    CONSOLE_PRINT("Local Client is connected to " + primaryAdress, GameConsole::eLogLevels::eDEBUG);
}

void LocalClient::disconnectTCP()
{
    if (m_pSocket != nullptr)
    {
        if (m_pRXTask.get() != nullptr)
        {
            m_pRXTask->close();
        }
        if (m_pTXTask.get() != nullptr)
        {
            m_pTXTask->close();
        }
        m_pRXTask.reset();
        m_pTXTask.reset();
        m_pSocket->disconnect();
        m_pSocket->close();
        m_pSocket.reset();
        CONSOLE_PRINT("Local Client disconnected.", GameConsole::eDEBUG);
        emit sigDisconnected(0);
    }
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
    CONSOLE_PRINT("Client is connected", GameConsole::eLogLevels::eDEBUG);
    m_isConnected = true;
    emit sigConnected(0);
}
