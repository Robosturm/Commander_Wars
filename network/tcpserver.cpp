#include <QTcpServer>
#include <QTcpSocket>
#include <QMutexLocker>

#include "coreengine/mainapp.h"
#include "network/tcpserver.h"
#include "network/rxtask.h"
#include "network/txtask.h"

TCPServer::TCPServer()
    : pTCPServer(NULL)
{
    isServer = true;
    isConnected = true;
}



void TCPServer::connectTCP(const QString&)
{
}

TCPServer::~TCPServer()
{
    disconnectTCP();
    delete pTCPServer;
}

void TCPServer::disconnectTCP()
{
    QMutexLocker locker(&TaskMutex);
    qint32 i = 0;
    while (i < pTCPSockets.size())
    {
        if (pTCPSockets[i]->isOpen())
        {
            delete pRXTasks[i];
            pRXTasks[i] = NULL;
            pRXTasks.removeAt(i);

            delete pTXTasks[i];
            pTXTasks[i] = NULL;
            pTXTasks.removeAt(i);

            // realize correct deletion
            pTCPSockets[i]->deleteLater();
            pTCPSockets[i] = NULL;
            pTCPSockets.removeAt(i);

            Console::print(tr("Client disconnected."), Console::eDEBUG);
        }
        else
        {
            i++;
        }
    }
}

void TCPServer::onConnect()
{
    QMutexLocker locker(&TaskMutex);
    QTcpSocket* pSocket = pTCPServer->nextPendingConnection();


    pTCPSockets.append(pSocket);

    pSocket->moveToThread(this);

    QObject::connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnectTCP()));
    QObject::connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    // Start RX-Task
    RxTask* pRXTask = new RxTask(pSocket, this);
    pRXTask->moveToThread(this);
    pRXTasks.append(pRXTask);
    QObject::connect(pSocket, SIGNAL(readyRead()), pRXTask, SLOT(recieveData()));

    // start TX-Task
    TxTask* pTXTask = new TxTask(pSocket, this);
    pTXTask->moveToThread(this);
    pTXTasks.append(pTXTask);
    QObject::connect(this, SIGNAL(sig_sendData(QByteArray, Mainapp::NetworkSerives, bool)), pTXTask, SLOT(send(QByteArray, Mainapp::NetworkSerives, bool)));

    Console::print(tr("New Client connection."), Console::eDEBUG);
}

void TCPServer::sessionOpened()
{
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
    {
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    }
    else
    {
        id = config.identifier();
    }

    Mainapp* pApp = Mainapp::getInstance();

    pApp->getSettings()->setNetworkData(id);

    pTCPServer = new QTcpServer(this);
    pTCPServer->moveToThread(this);
    pTCPServer->listen(QHostAddress::Any, pApp->getSettings()->getGamePort());
    QObject::connect(pTCPServer, SIGNAL(newConnection()), this, SLOT(onConnect()));

    Console::print(tr("Server is running"), Console::eDEBUG);
}

void TCPServer::sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking)
{
    if (blocking)
    {
        m_Blocking.acquire(pTXTasks.size());
    }
    emit sig_sendData(data, service, blocking);
}
