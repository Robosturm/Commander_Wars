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

#include "oxygine-framework.h"

class Serializable;
class QTcpSocket;


class NetworkInterface;
typedef oxygine::intrusive_ptr<NetworkInterface> spNetworkInterface;

/**
 * @brief The NetworkInterface class use this in the Context of a Network-Task
 */
class NetworkInterface : public QThread, public oxygine::ref_counter
{
    Q_OBJECT
public:
    NetworkInterface()
        : isServer(false),
          isConnected(false)
    {
        this->moveToThread(this);
        QObject::connect(this, &NetworkInterface::sig_connect, this, &NetworkInterface::connectTCP, Qt::QueuedConnection);
    }

    virtual ~NetworkInterface()
    {

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
    virtual void sendData(QByteArray data, Mainapp::NetworkSerives service, bool forwardData) = 0;

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
    virtual void forwardData(QTcpSocket*, QByteArray, Mainapp::NetworkSerives){}
protected:
    QNetworkSession *networkSession;
    bool isServer;
    bool isConnected;

    virtual void run()
    {
        QNetworkConfigurationManager manager;
        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.defaultConfiguration();
        networkSession = new QNetworkSession(config, this);
        networkSession->open();
        exec();
        disconnectTCP();
    }
signals:
    /**
     * @brief recieveData emitted when Data is recieved
     * @param data
     */
    void recieveData(QByteArray data, Mainapp::NetworkSerives service);
    void sig_connect(const QString& adress, quint16 port);
    void sigConnected();
    void sig_sendData(QByteArray data, Mainapp::NetworkSerives service, bool forwardData);

};

#endif // NETWORKINTERFACE_H
