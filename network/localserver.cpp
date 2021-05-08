#include <qlocalsocket.h>

#include "network/localserver.h"

#include "coreengine/mainapp.h"

LocalServer::LocalServer()
{
    setObjectName("LocalServer");
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = true;
    isConnected = true;
}

LocalServer::~LocalServer()
{
    disconnect();
    LocalServer::disconnectTCP();
    Console::print("Server is closed", Console::eLogLevels::eDEBUG);
}

void LocalServer::connectTCP(QString adress, quint16)
{
    m_pTCPServer = new QLocalServer(this);
    m_pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    m_pTCPServer->listen(adress);
    connect(m_pTCPServer, &QLocalServer::newConnection, this, &LocalServer::onConnect, Qt::QueuedConnection);
    connect(this, &LocalServer::sigDisconnectClient, this, &LocalServer::disconnectClient, Qt::QueuedConnection);
    connect(this, &LocalServer::sigForwardData, this, &LocalServer::forwardData, Qt::QueuedConnection);
    connect(this, &LocalServer::sigContinueListening, this, &LocalServer::continueListening, Qt::QueuedConnection);
    connect(this, &LocalServer::sigPauseListening, this, &LocalServer::pauseListening, Qt::QueuedConnection);

    Console::print("Local Server is running. " + adress, Console::eLogLevels::eDEBUG);
    // create marker file
    QString markername = "temp/" + adress + ".marker";
    QFile file(markername);
    file.open(QIODevice::WriteOnly);
    file.close();
}

void LocalServer::disconnectTCP()
{
    while (m_pTCPSockets.size() > 0)
    {
        if (m_pTCPSockets[0]->isOpen())
        {
            // realize correct deletion
            m_pTCPSockets[0]->disconnect();
            m_pTCPSockets[0]->close();
            Console::print("Client disconnected.", Console::eLogLevels::eDEBUG);
        }
        m_pRXTasks.removeAt(0);
        m_pTXTasks.removeAt(0);
        m_pTCPSockets.removeAt(0);
    }
    if (m_pTCPServer != nullptr)
    {
        m_pTCPServer->close();
        m_pTCPServer = nullptr;
    }
}

void LocalServer::disconnectClient(quint64 socketID)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            Console::print("Local Server Client disconnected.", Console::eLogLevels::eDEBUG);
            if (m_pTCPSockets[i]->isOpen())
            {
                // realize correct deletion
                m_pTCPSockets[i]->disconnect(this);
                m_pTCPSockets[i]->disconnectFromServer();
                m_pTCPSockets[i]->close();
            }
            m_pRXTasks.removeAt(i);
            m_pTXTasks.removeAt(i);
            m_pTCPSockets.removeAt(i);
            emit sigDisconnected(socketID);
            break;
        }
    }
}

void LocalServer::onConnect()
{
    if (m_pTCPServer != nullptr)
    {
        QLocalSocket* nextSocket = m_pTCPServer->nextPendingConnection();
        m_pTCPSockets.append(nextSocket);
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        connect(nextSocket, &QLocalSocket::errorOccurred, this, &LocalServer::displayLocalError, Qt::QueuedConnection);
        m_idCounter++;
        // Start RX-Task
        spRxTask pRXTask = spRxTask::create(nextSocket, m_idCounter, this, true);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        m_pRXTasks.append(pRXTask);
        connect(nextSocket, &QLocalSocket::readyRead, pRXTask.get(), &RxTask::recieveData, Qt::QueuedConnection);

        // start TX-Task
        spTxTask pTXTask = spTxTask::create(nextSocket, m_idCounter, this, true);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        m_pTXTasks.append(pTXTask);
        connect(this, &LocalServer::sig_sendData, pTXTask.get(), &TxTask::send, Qt::QueuedConnection);
        quint64 socket = m_idCounter;
        connect(nextSocket, &QLocalSocket::disconnected, [=]()
        {
            emit sigDisconnectClient(socket);
        });
        Console::print("New Client connection.", Console::eLogLevels::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

void LocalServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    Console::print("Forwarding data from local server to all clients except " + QString::number(socketID), Console::eDEBUG);
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] != socketID)
        {
            for (qint32 i2 = 0; i2 < m_pTXTasks.size(); i2++)
            {
                m_pTXTasks[i2]->send(m_SocketIDs[i], data, service, false);
            }
        }
    }
}

void LocalServer::pauseListening()
{
}

void LocalServer::continueListening()
{
}

QVector<quint64> LocalServer::getConnectedSockets()
{
    return m_SocketIDs;
}

void LocalServer::changeThread(quint64 socketID, QThread*)
{
    Console::print("Unsupported call to change thread on local server for socekt " + QString::number(socketID), Console::eFATAL);
}

void LocalServer::addSocket(quint64 socket)
{
    Console::print("Local Server added socket " + QString::number(socket), Console::eLogLevels::eDEBUG);
    m_SocketIDs.append(socket);
}

void LocalServer::removeSocket(quint64 socket)
{
    Console::print("Local Server removed socket " + QString::number(socket), Console::eLogLevels::eDEBUG);
    m_SocketIDs.removeAll(socket);
}
