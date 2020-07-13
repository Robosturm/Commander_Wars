#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>

#include "network/NetworkInterface.h"
#include "network/localclient.h"

/**
 * @brief The NetworkGame class needs to be run in it's own thread.
 * Handles sending data between the locally spawned pipe connected game instance and the joined players.
 */
class NetworkGame : public QObject
{
    Q_OBJECT
public:
    explicit NetworkGame();
    virtual ~NetworkGame() = default;
    QByteArray getDataBuffer() const;
    void setDataBuffer(const QByteArray &dataBuffer);

    QString getServerName() const;
    void setServerName(const QString &serverName);

public slots:
    void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief recieveData
     * @param socket
     * @param data
     * @param service
     */
    void recieveClientData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief recieveServerData
     * @param socket
     * @param data
     * @param service
     */
    void recieveServerData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief startAndWaitForInit
     */
    void startAndWaitForInit();
    /**
     * @brief onConnectToLocalServer
     */
    void onConnectToLocalServer(quint64);

protected slots:
    void checkServerRunning();
private:
    QVector<NetworkInterface*> m_Clients;
    QVector<quint64> m_SocketIDs;
    LocalClient m_gameConnection;
    QByteArray m_dataBuffer;
    QString m_serverName;
    QTimer m_timer;
};

#endif // NETWORKGAME_H
