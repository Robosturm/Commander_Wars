#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include "qprocess.h"

#include "network/tcpserver.h"
#include "network/tcpclient.h"

#include "network/networkgamedata.h"
#include "network/networkgame.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class MainServer;
using spMainServer = oxygine::intrusive_ptr<MainServer>;

/**
 * @brief The MainServer class handling the server and it's data.
 */
class MainServer : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    static MainServer* getInstance();
    static bool exists();
    void release();
    virtual ~MainServer();

    inline TCPServer* getGameServer()
    {
        return m_pGameServer.get();
    }
signals:
    void sigRemoveGame(NetworkGame* pGame);
    void sigStartRemoteGame(QString initScript, QString id);
public slots:
    /**
     * @brief recieveData we received data from
     * @param socketID
     * @param data
     * @param service
     */
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief receivedSlaveData
     * @param socketID
     * @param data
     * @param service
     */
    void receivedSlaveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief updateGameData marks the lobby data as changed
     */
    void updateGameData();
    /**
     * @brief sendGameDataUpdate sends the lobby data to all clients if needed
     */
    void sendGameDataUpdate();
    /**
     * @brief playerJoined a new player connected to server. Send him the initial lobby data
     * @param socketId
     */
    void playerJoined(qint64 socketId);
    /**
     * @brief startRemoteGame used for ai training and to move data from one thread context to this one
     * @param map
     * @param configuration
     */
    void startRemoteGame(const QString & initScript, const QString & id);

private slots:
    /**
     * @brief startRemoteGame used for ai training and to move data from one thread context to this one
     * @param map
     * @param configuration
     */
    void slotStartRemoteGame(QString initScript, QString id);
private:
    /**
     * @brief spawnSlaveGame checks if a slave game can be spawned and spawns a slave game on the server
     * @param stream
     * @param socketID
     * @param data
     * @param configuration
     * @param id
     */
    void spawnSlaveGame(QDataStream & stream, quint64 socketID, QByteArray& data, QString configuration = "", QString id = "");
    /**
     * @brief validHostRequest checks if all requested mods are installed on the server
     * @param mods
     * @return
     */
    bool validHostRequest(QStringList mods);
    /**
     * @brief sendGameDataToClient sends the lobby-data to the client
     * @param socketId 0 for all clients
     */
    void sendGameDataToClient(qint64 socketId);
    /**
     * @brief joinSlaveGame request of a client to join a specific slave game
     * @param socketID
     * @param stream
     */
    void joinSlaveGame(quint64 socketID, QDataStream & stream);
    /**
     * @brief closeGame
     * @param pGame
     */
    void closeGame(NetworkGame* pGame);
    /**
     * @brief spawnSlave starts a new slave game on the server
     * @param initScript
     * @param mods
     * @param id
     * @param socketID
     * @param data
     */
    void spawnSlave(const QString & initScript, const QStringList & mods, QString id, quint64 socketID, QByteArray& data);
    /**
     * @brief onSlaveReady called once a slave is in a state for receiving the data of the game that should be hosted
     * @param socketID
     * @param stream
     */
    void onSlaveReady(quint64 socketID, QDataStream &stream);
    /**
     * @brief onGameRunningOnServer called once the game has loaded all data needed for hosting a game and players can join
     * @param socketID
     * @param stream
     */
    void onGameRunningOnServer(quint64 socketID, QDataStream &stream);

private:
    class InternNetworkGame;
    using spInternNetworkGame = oxygine::intrusive_ptr<InternNetworkGame>;
    /**
     * @brief The InternNetworkGame class information about the process running a slave game
     */
    class InternNetworkGame : public oxygine::ref_counter
    {
    public:
        std::shared_ptr<QProcess> process;
        spNetworkGame game;
    };
    explicit MainServer();
    friend spMainServer;
    static spMainServer m_pInstance;
private:
    /**
     *  TCP-Server used for clients to connect to the server
     */
    spTCPServer m_pGameServer{nullptr};
    /**
     * TCP-Server used to receive information about the slave games and
     * to forward the initial package about the game that should be hosted on the slave
     */
    spTCPServer m_pSlaveServer{nullptr};
    /**
     * iterator for naming each slave with a unique identifier
     */
    quint64 m_slaveGameIterator{0};
    /**
     * @brief m_games data of games currently run on the server as slaves
     */
    QMap<QString, spInternNetworkGame> m_games;
    /**
     * @brief m_updateTimer update timer to send lobby data to clients if needed
     */
    QTimer m_updateTimer;
    /**
     * guard marking if new lobby data is available or not.
     */
    bool m_updateGameData{false};
};

#endif // MAINSERVER_H
