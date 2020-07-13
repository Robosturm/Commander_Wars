#include "localserver.h"

#include <qlocalsocket.h>

#include "coreengine/mainapp.h"

LocalServer::LocalServer()
{
    this->moveToThread(Mainapp::getInstance()->getNetworkThread());
    isServer = true;
    isConnected = true;
}

LocalServer::~LocalServer()
{
    disconnect();
    disconnectTCP();
    Console::print(tr("Server is closed"), Console::eLogLevels::eDEBUG);
}

void LocalServer::connectTCP(QString adress, quint16)
{
    pTCPServer = new QLocalServer(this);
    pTCPServer->moveToThread(Mainapp::getInstance()->getNetworkThread());
    pTCPServer->listen(adress);
    QObject::connect(pTCPServer, &QLocalServer::newConnection, this, &LocalServer::onConnect, Qt::QueuedConnection);
    QObject::connect(this, &LocalServer::sigDisconnectClient, this, &LocalServer::disconnectClient, Qt::QueuedConnection);
    QObject::connect(this, &LocalServer::sigForwardData, this, &LocalServer::forwardData, Qt::QueuedConnection);
    QObject::connect(this, &LocalServer::sigContinueListening, this, &LocalServer::continueListening, Qt::QueuedConnection);
    QObject::connect(this, &LocalServer::sigPauseListening, this, &LocalServer::pauseListening, Qt::QueuedConnection);

    Console::print(tr("Server is running. ") + adress, Console::eLogLevels::eDEBUG);
    // create marker file
    QString markername = "temp/" + adress + ".marker";
    QFile file(markername);
    file.open(QIODevice::WriteOnly);
    file.close();
}

void LocalServer::disconnectTCP()
{
    while (pTCPSockets.size() > 0)
    {
        if (pTCPSockets[0]->isOpen())
        {
            // realize correct deletion
            pTCPSockets[0]->disconnect();
            pTCPSockets[0]->close();
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
        }
        pRXTasks.removeAt(0);
        pTXTasks.removeAt(0);
        pTCPSockets.removeAt(0);
    }
    if (pTCPServer != nullptr)
    {
        pTCPServer->close();
        pTCPServer = nullptr;
    }
}

void LocalServer::disconnectClient(quint64 socketID)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            if (pTCPSockets[i]->isOpen())
            {
                // realize correct deletion
                pTCPSockets[i]->disconnect(this);
                pTCPSockets[i]->disconnectFromServer();
                pTCPSockets[i]->close();
            }
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
            emit sigDisconnected(socketID);
            break;
        }
    }
}

void LocalServer::disconnectSocket()
{
    qint32 i = 0;
    while (i < pTCPSockets.size())
    {
        if (pTCPSockets[i]->state() == QLocalSocket::LocalSocketState::ClosingState ||
            pTCPSockets[i]->state() == QLocalSocket::LocalSocketState::UnconnectedState)
        {
            // realize correct deletion
            pTCPSockets[i]->disconnect();
            pTCPSockets[i]->close();
            pRXTasks.removeAt(i);
            pTXTasks.removeAt(i);
            pTCPSockets.removeAt(i);
            quint64 id = m_SocketIDs[i];
            m_SocketIDs.removeAt(i);
            Console::print(tr("Client disconnected."), Console::eLogLevels::eDEBUG);
            emit sigDisconnected(id);
        }
        else
        {
            i++;
        }
    }
}

void LocalServer::onConnect()
{
    if (pTCPServer != nullptr)
    {
        QLocalSocket* nextSocket = pTCPServer->nextPendingConnection();
        pTCPSockets.append(nextSocket);
        nextSocket->moveToThread(Mainapp::getInstance()->getNetworkThread());
        QObject::connect(nextSocket, &QLocalSocket::disconnected, this, &LocalServer::disconnectSocket, Qt::QueuedConnection);
        QObject::connect(nextSocket, &QLocalSocket::errorOccurred, this, &LocalServer::displayLocalError, Qt::QueuedConnection);
        m_idCounter++;
        if (m_idCounter == 0)
        {
            m_idCounter++;
        }
        m_SocketIDs.append(m_idCounter);

        // Start RX-Task
        RxTask* pRXTask = new RxTask(nextSocket, m_idCounter, this);
        pRXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pRXTasks.append(pRXTask);
        QObject::connect(nextSocket, &QLocalSocket::readyRead, pRXTask, &RxTask::recieveData, Qt::QueuedConnection);

        // start TX-Task
        TxTask* pTXTask = new TxTask(nextSocket, m_idCounter, this);
        pTXTask->moveToThread(Mainapp::getInstance()->getNetworkThread());
        pTXTasks.append(pTXTask);
        QObject::connect(this, &LocalServer::sig_sendData, pTXTask, &TxTask::send, Qt::QueuedConnection);

        Console::print(tr("New Client connection."), Console::eLogLevels::eDEBUG);
        emit sigConnected(m_idCounter);
    }
}

void LocalServer::forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] != socketID)
        {
            pTXTasks[i]->send(0, data, service, false);
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

void LocalServer::changeThread(quint64 socketID, QThread* pThread)
{
    for (qint32 i = 0; i < m_SocketIDs.size(); i++)
    {
        if (m_SocketIDs[i] == socketID)
        {
            pRXTasks[i]->moveToThread(pThread);
            pTXTasks[i]->moveToThread(pThread);
            break;
        }
    }
}
