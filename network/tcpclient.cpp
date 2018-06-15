#include <QTcpSocket>

#include "network/rxtask.h"
#include "network/txtask.h"
#include "coreengine/mainapp.h"
#include "network/tcpclient.h"


TCPClient::TCPClient()
    : pRXTask(NULL),
      pTXTask(NULL),
      pSocket(NULL)
{
    isServer = false;

}

TCPClient::~TCPClient()
{
    disconnectTCP();
}

void TCPClient::connectTCP(const QString& adress)
{
    disconnectTCP();
    Mainapp* pApp = Mainapp::getInstance();
    // Launch Socket
    pSocket = new QTcpSocket(this);
    QObject::connect(pSocket, SIGNAL(disconnected()), this, SLOT(disconnectTCP()));
    QObject::connect(pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    pSocket->connectToHost(adress, pApp->getSettings()->getGamePort());
    pSocket->moveToThread(this);

    // Start RX-Task
    pRXTask = new RxTask(pSocket, this);
    pRXTask->moveToThread(this);
    QObject::connect(pSocket, SIGNAL(readyRead()), pRXTask, SLOT(recieveData()));

    // start TX-Task
    pTXTask = new TxTask(pSocket, this);
    pTXTask->moveToThread(this);
    QObject::connect(this, SIGNAL(sig_sendData(QByteArray,Mainapp::NetworkSerives, bool)), pTXTask, SLOT(send(QByteArray,Mainapp::NetworkSerives, bool)));

    isConnected = true;
    Console::print(tr("Client is running"), Console::eDEBUG);
}

void TCPClient::disconnectTCP()
{
    if (pRXTask != NULL)
    {
        delete pRXTask;
        pRXTask = NULL;
    }
    if (pTXTask != NULL)
    {
        delete pTXTask;
        pTXTask = NULL;
    }
    if (pSocket != NULL)
    {
        pSocket->abort();
        // realize correct deletion
        pSocket->deleteLater();
    }
    pSocket = NULL;
}

void TCPClient::sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking)
{
    emit sig_sendData(data, service, blocking);
}

void TCPClient::sessionOpened()
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
}
