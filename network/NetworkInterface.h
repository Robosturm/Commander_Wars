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
        LobbyChat,              /**< used for the lobby chat */
        GameChat,               /**< used for ingame chat */
        Multiplayer,            /**< used for the multiplayer game selection */
        ServerHostingJson,      /**< used for data when starting a game on the host or when communicating between slave and master. Packages are in JSON-Format */
        ServerHosting,          /**< used for data when starting a game on the host or when communicating between slave and master. Packages are in Binary-Format */
        ServerSocketInfo,       /**< used inside the rx-task data is not emitted when recieving this data */
        CryptedMessage,
        Max,
    };

    explicit NetworkInterface(QObject* pParent)
        : QObject(pParent),
          m_isServer(false),
          m_isConnected(false)

    {
        connect(this, &NetworkInterface::sig_connect, this, &NetworkInterface::connectTCP, Qt::QueuedConnection);
        connect(this, &NetworkInterface::sigChangeThread, this, &NetworkInterface::changeThread, Qt::QueuedConnection);
    }
    virtual ~NetworkInterface() = default;

    static QString getIPAdresse()
    {
        QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (auto & ipAddr : ipAddressesList)
        {
            if (ipAddr != QHostAddress::LocalHost && ipAddr.toIPv4Address())
            {
                ipAddress = ipAddr.toString();
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
        for (auto & ipAddr : ipAddressesList)
        {
            if (ipAddr != QHostAddress::LocalHost)
            {
                ipAddresses.append(ipAddr.toString());
            }
        }
        return ipAddresses;
    }

    bool getIsServer()
    {
        return m_isServer;
    }

    bool getIsConnected()
    {
        return m_isConnected;
    }

    void setIsServer(bool value)
    {
        m_isServer = value;
    }
    quint64 getSocketID() const
    {
        return m_socketID;
    }
    virtual void setSocketID(const quint64 &socketID)
    {
        m_socketID = socketID;
    }
    /**
     * @brief getIsObserver if the client is primarly observing the game and data stream
     * @return
     */
    bool getIsObserver() const
    {
        return m_isObserver;
    }
    void setIsObserver(bool newIsObserver)
    {
        m_isObserver = newIsObserver;
    }
    bool getIsActive() const
    {
        return m_active;
    }
    void setIsActive(bool isActive)
    {
        m_active = isActive;
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
        CONSOLE_PRINT("Error catched for " + QString::number(m_socketID), Console::eDEBUG);
        switch (socketError)
        {
            case QAbstractSocket::RemoteHostClosedError:
                CONSOLE_PRINT("The connection was closed by the peer.", Console::eDEBUG);
                break;
            case QAbstractSocket::HostNotFoundError:
                CONSOLE_PRINT("The host was not found. Please check the host name and port settings.", Console::eERROR);
                break;
            case QAbstractSocket::ConnectionRefusedError:
                CONSOLE_PRINT("The connection was refused by the peer.", Console::eDEBUG);
                break;
            case QAbstractSocket::SocketAccessError:
                CONSOLE_PRINT("Socket connection access error.", Console::eDEBUG);
                break;
            case QAbstractSocket::SocketResourceError:
                CONSOLE_PRINT("Socket Connection resource error.", Console::eDEBUG);
                break;
            case QAbstractSocket::SocketTimeoutError:
                CONSOLE_PRINT("Socket timeout error.", Console::eDEBUG);
                break;
            case QAbstractSocket::DatagramTooLargeError:
                CONSOLE_PRINT("Datagram too large error.", Console::eDEBUG);
                break;
            case QAbstractSocket::NetworkError:
                CONSOLE_PRINT("Network error.", Console::eDEBUG);
                break;
            case QAbstractSocket::AddressInUseError:
                CONSOLE_PRINT("Address in user error.", Console::eDEBUG);
                break;
            case QAbstractSocket::SocketAddressNotAvailableError:
                CONSOLE_PRINT("Socket Address not available error.", Console::eDEBUG);
                break;
            case QAbstractSocket::UnsupportedSocketOperationError:
                CONSOLE_PRINT("Unsupported socket operation error.", Console::eDEBUG);
                break;
            case QAbstractSocket::UnfinishedSocketOperationError:
                CONSOLE_PRINT("Unfinished socket operation error.", Console::eDEBUG);
                break;
            case QAbstractSocket::SslHandshakeFailedError:
                CONSOLE_PRINT("Ssl Handshake failed.", Console::eDEBUG);
                break;
            case QAbstractSocket::SslInternalError:
                CONSOLE_PRINT("Ssl internal error.", Console::eDEBUG);
                break;
            case QAbstractSocket::SslInvalidUserDataError:
                CONSOLE_PRINT("Ssl invalid user data errror.", Console::eDEBUG);
                break;
            default:
                CONSOLE_PRINT("Error inside the Socket happened. Error: " + QString::number(socketError), Console::eERROR);
        }
    }
    void displayStateChange(QAbstractSocket::SocketState socketState)
    {
        CONSOLE_PRINT("State changed for " + QString::number(m_socketID), Console::eDEBUG);
        switch (socketState)
        {
            case QAbstractSocket::UnconnectedState:
                CONSOLE_PRINT("State is now Unconnected.", Console::eDEBUG);
                break;
            case QAbstractSocket::HostLookupState:
                CONSOLE_PRINT("State is now Lookup.", Console::eDEBUG);
                break;
            case QAbstractSocket::ConnectingState:
                CONSOLE_PRINT("State is now Connecting.", Console::eDEBUG);
                break;
            case QAbstractSocket::ConnectedState:
                CONSOLE_PRINT("State is now Connected.", Console::eDEBUG);
                break;
            case QAbstractSocket::BoundState:
                CONSOLE_PRINT("State is now Bound.", Console::eDEBUG);
                break;
            case QAbstractSocket::ListeningState:
                CONSOLE_PRINT("State is now Listening.", Console::eDEBUG);
                break;
            case QAbstractSocket::ClosingState:
                CONSOLE_PRINT("State is now Closing.", Console::eDEBUG);
                break;
        }
    }
protected:
    quint64 m_socketID{0};
    bool m_isServer{false};
    bool m_isConnected{false};
    bool m_isObserver{false};
    bool m_active{true};
};

#endif // NETWORKINTERFACE_H
