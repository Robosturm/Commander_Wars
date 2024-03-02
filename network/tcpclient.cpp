#include "network/rxtask.h"
#include "network/txtask.h"
#include "network/tcpclient.h"

TCPClient::TCPClient(QObject* pParent)
    : NetworkInterface(pParent),
      m_pRXTask(nullptr),
      m_pTXTask(nullptr),
      m_pSocket(nullptr)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TCPClient");
#endif
    m_isServer = false;
}

TCPClient::TCPClient(QObject* pParent, spRxTask pRXTask, spTxTask pTXTask, spQSslSocket pSocket, quint64 socketId)
    : NetworkInterface(pParent),
      m_pRXTask(pRXTask),
      m_pTXTask(pTXTask),
      m_pSocket(pSocket),
      m_onServer(true)
{
#ifdef GRAPHICSUPPORT
    setObjectName("TCPClient");
#endif
    NetworkInterface::setSocketID(socketId);
    m_pRXTask->setSocketID(socketId);
    m_pTXTask->setSocketID(socketId);
    connect(this, &TCPClient::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
}

TCPClient::~TCPClient()
{
    CONSOLE_PRINT("Client gets closed", GameConsole::eLogLevels::eDEBUG);    
    disconnect();
    if (!m_onServer)
    {
        TCPClient::disconnectTCP();
    }
}

void TCPClient::connectTCP(QString address, quint16 port, QString secondaryAdress, bool gatewayClient)
{
    m_secondaryAdress = secondaryAdress;
    m_port = port;
    m_testedSecondaryAddress = false;
    m_pSocket = MemoryManagement::createNamedQObject<QSslSocket>("QSslSocket", this);
    connect(m_pSocket.get(), &QSslSocket::encrypted, this, &TCPClient::connected, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QTcpSocket::disconnected, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::errorOccurred, this, &TCPClient::displayTCPError, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::stateChanged, this, &TCPClient::displayStateChange, Qt::QueuedConnection);
    connect(this, &TCPClient::sigDisconnectClient, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    connect(this, &TCPClient::sigDisconnectTCP, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);
    connect(m_pSocket.get(), &QAbstractSocket::errorOccurred, this, &TCPClient::disconnectTCP, Qt::QueuedConnection);    
    m_pSocket->setSslConfiguration(getSslConfiguration());
    m_pSocket->connectToHostEncrypted(address, port);
    
    // Start RX-Task
    m_pRXTask = MemoryManagement::create<RxTask>(m_pSocket.get(), 0, this, gatewayClient);
    connect(m_pSocket.get(), &QTcpSocket::readyRead, m_pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

    // start TX-Task
    m_pTXTask = MemoryManagement::create<TxTask>(m_pSocket.get(), 0, this, gatewayClient);
    connect(this, &TCPClient::sig_sendData, m_pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
    CONSOLE_PRINT("Client is running and connecting to \"" + address + "\" and port " + QString::number(port), GameConsole::eLogLevels::eDEBUG);
    m_connectedAdress = address;
    m_connectedPort = port;
}

void TCPClient::disconnectTCP()
{
    if (!m_testedSecondaryAddress && !m_secondaryAdress.isEmpty() && !m_isConnected)
    {
        CONSOLE_PRINT("Unable to connect to primary address testing secondary", GameConsole::eDEBUG);
        m_pSocket->close();
        m_pSocket->connectToHostEncrypted(m_secondaryAdress, m_port);
        m_testedSecondaryAddress = true;
        CONSOLE_PRINT("Client is running and connecting to \"" + m_secondaryAdress + "\" and port " + QString::number(m_port), GameConsole::eLogLevels::eDEBUG);
        m_connectedAdress = m_secondaryAdress;
        m_connectedPort = m_port;
    }
    else
    {
        CONSOLE_PRINT("TCP Client " + QString::number(m_socketID) + " disconnected.", GameConsole::eLogLevels::eDEBUG);
        if (m_pSocket != nullptr)
        {
            if (m_pRXTask.get() != nullptr)
            {
                m_pRXTask->disconnect();
                m_pRXTask->close();
            }
            if (m_pTXTask.get() != nullptr)
            {
                m_pTXTask->disconnect();
                m_pTXTask->close();
            }
            m_pRXTask.reset();
            m_pTXTask.reset();
            m_pSocket->disconnect();
            m_pSocket->close();
            m_pSocket = nullptr;
            if (!m_onServer)
            {
                emit sigDisconnected(m_socketID);
            }
        }        
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
    if (m_pSocket->isEncrypted() && 
        m_socketID > 0 && 
        !m_isConnected)
    {
        CONSOLE_PRINT("Client is connected and encrypted ready using socket id " + QString::number(m_socketID), GameConsole::eLogLevels::eDEBUG);
        m_isConnected = true;
        m_testedSecondaryAddress = true; // no need to test the secondary address
        emit sigConnected(m_socketID);
    }
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
    connected();
}

void TCPClient::sslErrors(const QList<QSslError> &errors)
{
    for (const auto & error : std::as_const(errors))
    {
        CONSOLE_PRINT(error.errorString(), GameConsole::eLogLevels::eDEBUG);
    }
}
