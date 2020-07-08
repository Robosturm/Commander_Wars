#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QThread>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <QVector>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QSemaphore>

#include "oxygine-framework.h"

#include "coreengine/console.h"

class Serializable;

class NetworkInterface;
typedef oxygine::intrusive_ptr<NetworkInterface> spNetworkInterface;

/**
 * @brief The NetworkInterface class use this in the Context of a Network-Task
 */
class NetworkInterface : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    /**
      * @brief this enum contains all message recievers of the network
      */
    enum class NetworkSerives
    {
        None = -1,
        Game,           /**< used for game stream data */
        Lobby,          /**< used for lobby stream data */
        LobbyChat,      /**< used for the lobby chat */
        GameChat,       /**< used for ingame chat */
        Multiplayer,    /**< used for the multiplayer game selection */
        ServerHosting,  /**< used for data when starting a new game on the host */
        Max,
    };

    NetworkInterface()
        : isServer(false),
          isConnected(false)
    {
        oxygine::intrusive_ptr_add_ref(this);
        QObject::connect(this, &NetworkInterface::sig_connect, this, &NetworkInterface::connectTCP, Qt::QueuedConnection);
        QObject::connect(this, &NetworkInterface::sig_close, this, &NetworkInterface::closeNetworkInterface, Qt::QueuedConnection);
    }

    virtual ~NetworkInterface()
    {
    }

    static QString getIPAdresse()
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

    static QStringList getIPAdresses()
    {
        QStringList ipAddresses;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (qint32 i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost)
            {
                ipAddresses.append(ipAddressesList.at(i).toString());
            }
        }
        return ipAddresses;
    }

    bool getIsServer()
    {
        return isServer;
    }

    bool getIsConnected()
    {
        return isConnected;
    }

    void setIsServer(bool value)
    {
        isServer = value;
    }

signals:
    /**
     * @brief recieveData emitted when Data is recieved
     * @param data
     */
    void recieveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service);
    void sig_connect(QString adress, quint16 port);
    void sigConnected(quint64 socket);
    void sigDisconnected(quint64 socket);
    void sig_sendData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service, bool forwardData);
    void sig_close();
public slots:
    virtual void connectTCP(QString adress, quint16 port) = 0;
    virtual void disconnectTCP() = 0;

    virtual QTcpSocket* getSocket(quint64 socketID) = 0;
    void displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError)
        {
            case QAbstractSocket::RemoteHostClosedError:
                Console::print(tr("The server was closed by the peer."), Console::eDEBUG);
                break;
            case QAbstractSocket::HostNotFoundError:
                Console::print(tr("The host was not found. Please check the host name and port settings."), Console::eERROR);
                break;
            case QAbstractSocket::ConnectionRefusedError:
                Console::print(tr("The connection was refused by the peer."), Console::eDEBUG);
                break;
            default:
                Console::print(tr("Error inside the Socket happened."), Console::eERROR);
        }
    }
    virtual void forwardData(quint64, QByteArray, NetworkInterface::NetworkSerives){}
protected slots:
    void closeNetworkInterface()
    {
        oxygine::intrusive_ptr_release(this);
    }
protected:
    bool isServer;
    bool isConnected;
};

#endif // NETWORKINTERFACE_H
