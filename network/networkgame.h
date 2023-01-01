#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include <QObject>
#include <QLocalSocket>
#include <QTimer>
#include <QProcess>

#include "network/tcpserver.h"
#include "network/networkgamedata.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

class NetworkGame;
using spNetworkGame = oxygine::intrusive_ptr<NetworkGame>;

/**
 * @brief The NetworkGame class needs to be run in it's own thread.
 * Handles sending data between the locally spawned pipe connected game instance and the joined players.
 */
class NetworkGame final : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit NetworkGame(QObject* pParent, const QString & serverName);
    ~NetworkGame();
    QByteArray getDataBuffer() const;
    void setDataBuffer(const QByteArray &dataBuffer);

    QString getServerName() const;

    const NetworkGameData & getData() const;
    NetworkGameData & getData();
    /**
     * @brief getSlaveRunning
     * @return
     */
    bool getSlaveRunning() const;
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
    /**
     * @brief onConnectToLocalServer
     */
    void onConnectToLocalServer(quint64 socketId, spTCPServer & pTcpServer);
    /**
     * @brief getHostingSocket socket id of the socket hosting the game
     * @return
     */
    quint64 getHostingSocket() const;
    /**
     * @brief setHostingSocket socket id of the socket hosting the game
     * @param newHostingSocket
     */
    void setHostingSocket(quint64 newHostingSocket);
    /**
     * @brief slaveRunning
     * @param stream
     */
    void slaveRunning(const QJsonObject & objData, spTCPServer & pGameServer);
    /**
     * @brief slaveGameStarted
     * @param objData
     */
    void slaveGameStarted(const QJsonObject & objData);
    /**
     * @brief startCloseTimer
     */
    void startCloseTimer();
    const QString &getSlaveRespawnFile() const;
    void setSlaveRespawnFile(const QString &newSlaveRespawnFile);
    void onSlaveRelaunched();
    bool getRunningGame() const;
    void setRunningGame(bool newRunningGame);

signals:
    void sigDataChanged();
    void sigClose(NetworkGame* pGame);
public slots:
    /**
     * @brief finished
     * @param exitCode
     * @param exitStatus
     */
    void processFinished(qint32 exitCode, QProcess::ExitStatus exitStatus);
    /**
     * @brief errorOccurred
     * @param error
     */
    void errorOccurred(QProcess::ProcessError error);
    /**
     * @brief closeGame
     */
    void closeGame();
    /**
     * @brief closeTimerExpired
     */
    void closeTimerExpired();
private:
    QByteArray m_dataBuffer;
    spNetworkInterface m_hostingClient;
    quint64 m_hostingSocket;
    QString m_serverName;
    bool m_slaveRunning{false};
    bool m_closing{false};
    bool m_runningGame{false};
    NetworkGameData m_data;
    QString m_id;
    QString m_slaveRespawnFile;
    QTimer m_closeTimer;
};

Q_DECLARE_INTERFACE(NetworkGame, "NetworkGame");

#endif // NETWORKGAME_H
