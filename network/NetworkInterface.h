#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QThread>
#include <QVector>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QSemaphore>
#include <QNetworkInterface>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "coreengine/console.h"
#include "coreengine/LUPDATE_MACROS.h"

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
    ENUM_CLASS NetworkSerives
    {
        None = -1,
        Game,                   /**< used for game stream data */
        Lobby,                  /**< used for lobby stream data */
        LobbyChat,              /**< used for the lobby chat */
        GameChat,               /**< used for ingame chat */
        Multiplayer,            /**< used for the multiplayer game selection */
        ServerHosting,          /**< used for data when starting a game on the host or when communicating between slave and master */
        ServerSocketInfo,       /**< used inside the rx-task data is not emitted when recieving this data */
        Max,
    };

    NetworkInterface()
        : isServer(false),
          isConnected(false)
    {
        m_pNetworkInterface = this;
        connect(this, &NetworkInterface::sig_connect, this, &NetworkInterface::connectTCP, Qt::QueuedConnection);
        connect(this, &NetworkInterface::sig_close, this, &NetworkInterface::closeNetworkInterface, Qt::QueuedConnection);
        connect(this, &NetworkInterface::sigChangeThread, this, &NetworkInterface::changeThread, Qt::QueuedConnection);
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
    quint64 getSocketID() const
    {
        return m_socketID;
    }
    virtual void setSocketID(const quint64 &socketID)
    {
        m_socketID = socketID;
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
    /**
     * @brief sigDisconnectClient
     * @param socketID
     */
    void sigDisconnectClient(quint64 socketID);
    /**
     * @brief sigPauseListening
     */
    void sigPauseListening();
    /**
     * @brief sigContinueListening
     */
    void sigContinueListening();
    /**
     * @brief sigForwardData forwards data to all clients except for the given socket
     * @param socketID
     * @param data
     * @param service
     */
    void sigForwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief sigChangeThread
     * @param socketID
     * @param pThread
     */
    void sigChangeThread(quint64 socketID, QThread* pThread);
public slots:
    virtual void connectTCP(QString adress, quint16 port) = 0;
    virtual void disconnectTCP() = 0;
    virtual void forwardData(quint64, QByteArray, NetworkInterface::NetworkSerives){}
    virtual QVector<quint64> getConnectedSockets() = 0;
    virtual void changeThread(quint64 socketID, QThread* pThread) = 0;
    void displayTCPError(QAbstractSocket::SocketError socketError)
    {
        Console::print("Error catched for " + QString::number(m_socketID), Console::eDEBUG);
        switch (socketError)
        {
            case QAbstractSocket::RemoteHostClosedError:
                Console::print("The connection was closed by the peer.", Console::eDEBUG);
                break;
            case QAbstractSocket::HostNotFoundError:
                Console::print("The host was not found. Please check the host name and port settings.", Console::eERROR);
                break;
            case QAbstractSocket::ConnectionRefusedError:
                Console::print("The connection was refused by the peer.", Console::eDEBUG);
                break;
            default:
                Console::print("Error inside the Socket happened. Error: " + QString::number(socketError), Console::eERROR);
        }
    }

    void displayLocalError(QLocalSocket::LocalSocketError socketError)
    {
        switch (socketError)
        {
            case QLocalSocket::ConnectionError:
                Console::print("The server was closed by the peer.", Console::eDEBUG);
                break;
            case QLocalSocket::ServerNotFoundError:
                Console::print("The host was not found. Please check the host name and port settings.", Console::eERROR);
                break;
            case QLocalSocket::ConnectionRefusedError:
                Console::print("The connection was refused by the peer.", Console::eDEBUG);
                break;
            default:
                Console::print("Error inside the Socket happened. Error: " + QString::number(socketError), Console::eERROR);
        }
    }
protected slots:
    void closeNetworkInterface()
    {
        m_pNetworkInterface = nullptr;
    }
protected:
    spNetworkInterface m_pNetworkInterface;
    bool isServer;
    bool isConnected;
    quint64 m_socketID{0};
};

#endif // NETWORKINTERFACE_H
