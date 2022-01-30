#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>
#include <QProcess>

#include "network/tcpclient.h"
#include "network/localclient.h"
#include "network/networkgamedata.h"
#include "3rd_party/oxygine-framework/oxygine-framework.h"

class NetworkGame;
using spNetworkGame = oxygine::intrusive_ptr<NetworkGame>;

/**
 * @brief The NetworkGame class needs to be run in it's own thread.
 * Handles sending data between the locally spawned pipe connected game instance and the joined players.
 */
class NetworkGame : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit NetworkGame(QObject* pParent);
    virtual ~NetworkGame() = default;
    QByteArray getDataBuffer() const;
    void setDataBuffer(const QByteArray &dataBuffer);

    QString getServerName() const;
    void setServerName(const QString &serverName);

    const NetworkGameData & getData() const;
    /**
     * @brief getSlaveRunning
     * @return
     */
    bool getSlaveRunning() const;
    /**
     * @brief setSlaveRunning
     * @param slaveRunning
     */
    void setSlaveRunning(bool slaveRunning);
    /**
     * @brief getId
     * @return
     */
    const QString & getId() const;
    /**
     * @brief getId
     * @param id
     */
    void setId(QString & id);
signals:
    void sigDataChanged();
    void sigClose(NetworkGame* pGame);
    void sigDisconnectSocket(quint64 socketID);
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
     * @brief recieveSlaveData
     * @param socket
     * @param data
     * @param service
     */
    void recieveSlaveData(quint64 socket, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief startAndWaitForInit
     */
    void startAndWaitForInit();
    /**
     * @brief onConnectToLocalServer
     */
    void onConnectToLocalServer(quint64);
    /**
     * @brief addClient
     * @param pClient
     */
    void addClient(spTCPClient pClient);
    /**
     * @brief clientDisconnect
     * @param socketId
     */
    void clientDisconnect(quint64 socketId);
    /**
     * @brief finished
     * @param exitCode
     * @param exitStatus
     */
    void processFinished(qint32 exitCode, QProcess::ExitStatus exitStatus);
    /**
     * @brief slaveRunning
     * @param stream
     */
    void slaveRunning(QDataStream &stream);
protected slots:
    void checkServerRunning();
    void sendPlayerJoined(qint32 player);
private:
    void closeGame();
private:
    QVector<spTCPClient> m_Clients;
    LocalClient m_gameConnection;
    QByteArray m_dataBuffer;
    QString m_serverName;
    QTimer m_timer;
    bool m_slaveRunning{false};
    bool m_closing{false};
    NetworkGameData m_data;
    QString m_id;
};

#endif // NETWORKGAME_H
