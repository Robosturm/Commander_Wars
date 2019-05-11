#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QThread>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QVector>
#include <QAbstractSocket>
#include <QSemaphore>

#include "coreengine/mainapp.h"
#include "coreengine/settings.h"
#include "coreengine/console.h"

class Serializable;

/**
 * @brief The NetworkInterface class use this in the Context of a Network-Task
 */
class NetworkInterface : public QThread
{
    Q_OBJECT
public:
    NetworkInterface()
        : isServer(false),
          isConnected(false)
    {
        Mainapp* pApp = Mainapp::getInstance();
        this->moveToThread(this);
        QObject::connect(this, SIGNAL(sig_connect(QString)), this, SLOT(connectTCP(QString)));
        QNetworkConfigurationManager manager;
        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.defaultConfiguration();
        networkSession = new QNetworkSession(config, this);
        QObject::connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
        this->moveToThread(this);
        networkSession->open();

        connect(this, SIGNAL(recieveData(QByteArray,Mainapp::NetworkSerives)), Console::getInstance(), SLOT(recieveNetworkMessage(QByteArray,Mainapp::NetworkSerives)));
    }

    virtual ~NetworkInterface()
    {
        delete networkSession;
    }

    QString getIPAdresse()
    {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (qint32 i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
            {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (ipAddress.isEmpty())
        {
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        }
        return ipAddress;
    }

    bool getIsServer()
    {
        return isServer;
    }

    bool getIsConnected()
    {
        return isConnected;
    }

public slots:
    virtual void connectTCP(const QString& adress, quint16 port) = 0;
    virtual void disconnectTCP() = 0;
    /**
     * @brief sendData send Data with this Connection
     * @param data
     */
    virtual void sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking) = 0;

    virtual void sessionOpened(quint16 port) = 0;

    void displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError)
        {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            Console::print(tr("The host was not found. Please check the host name and port settings."), Console::eERROR);
            break;
        case QAbstractSocket::ConnectionRefusedError:
            Console::print(tr("The connection was refused by the peer."), Console::eERROR);
            break;
        default:
            Console::print(tr("Error inside the Socket happened."), Console::eERROR);
        }
    }

    void acquireBlock()
    {
        m_Blocking.release();
    }

protected:
    QNetworkSession *networkSession;
    bool isServer;
    bool isConnected;
    QSemaphore m_Blocking;

    virtual void run()
    {
        while (true) {
            exec();
        }
    }
signals:
    /**
     * @brief recieveData emitted when Data is recieved
     * @param data
     */
    void recieveData(QByteArray data, Mainapp::NetworkSerives service);
    void sig_connect(const QString& adress);

};

#endif // NETWORKINTERFACE_H
