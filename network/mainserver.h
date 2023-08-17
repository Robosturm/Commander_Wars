#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QProcess>

#include "network/tcpserver.h"
#include "network/networkgamedata.h"
#include "network/networkgame.h"
#include "network/smtpmailsender.h"
#include "network/matchmakingcoordinator.h"

#include "coreengine/fileserializable.h"

#include "multiplayer/networkcommands.h"

#include "game/GameEnums.h"

class MainServer;
using spMainServer = std::shared_ptr<MainServer>;

/**
 * @brief The MainServer class handling the server and it's data.
 */
class MainServer final : public QObject, public FileSerializable
{
    Q_OBJECT
    struct AddressInfo
    {
        QString address;
        QString secondaryAddress;
        quint16 minPort;
        quint16 maxPort;
    };
    struct SlaveAddress
    {
        QString address;
        QString secondaryAddress;
        quint16 port;
    };
    struct SuspendedSlaveInfo : public FileSerializable
    {
        bool relaunched{false};
        bool runningGame{false};
        QString currentPlayer;
        QVector<quint64> pendingSockets;
        QString savefile;
        NetworkGameData game;
        QElapsedTimer despawnTime;
        /**
         * @brief serializeObject
         * @param stream
         */
        virtual void serializeObject(QDataStream& stream) const override;
        /**
         * @brief deserialize restores the object
         * @param pStream
         */
        virtual void deserializeObject(QDataStream& stream) override;
        /**
         * @brief getVersion version of the file
         * @return
         */
        virtual qint32 getVersion() const override
        {
            return 1;
        }
    };

public:
    static const char* const SQL_TABLE_PLAYERS;
    static const char* const SQL_USERNAME;
    static const char* const SQL_PASSWORD;
    static const char* const SQL_MAILADRESS;
    static const char* const SQL_VALIDPASSWORD;
    static const char* const SQL_LASTLOGIN;
    static const char* const SQL_TABLE_PLAYERDATA;
    static const char* const SQL_COID;
    static const char* const SQL_GAMESMADE;
    static const char* const SQL_GAMESLOST;
    static const char* const SQL_GAMESWON;
    static const char* const SQL_GAMESDRAW;
    static const char* const SQL_METADATA;
    static const char* const SQL_TABLE_MATCH_DATA;
    static const char* const SQL_MMR;
    static const char* const SQL_MINGAMES;
    static const char* const SQL_MAXGAMES;
    static const char* const SQL_RUNNINGGAMES;
    static const char* const SQL_MATCHHISTORY;
    static const char* const SQL_SIGNEDUP;

    static MainServer* getInstance();
    static bool exists();
    static void initDatabase();
    static GameEnums::LoginError verifyLoginData(const QString & username, const QByteArray & password);
    void release();
    virtual ~MainServer();

    inline spTCPServer getGameServer()
    {
        return m_pGameServer;
    }
    /**
     * @brief getDatabase
     * @return
     */
    QSqlDatabase & getDatabase();
    /**
     * @brief sqlQueryFailed
     * @param query
     * @return
     */
    static bool sqlQueryFailed(const QSqlQuery & query);
    /**
     * @brief serializeObject
     * @param stream
     */
    virtual void serializeObject(QDataStream& stream) const override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& stream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    virtual qint32 getVersion() const override
    {
        return 3;
    }
    /**
     * @brief onSlaveInfoDespawning
     * @param socketID
     * @param objData
     */
    void onSlaveInfoDespawning(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief startRemoteGame used for ai training and to move data from one thread context to this one
     * @param map
     * @param configuration
     */
    Q_INVOKABLE void startRemoteGame(const QString initScript, const QString id);
    /**
     * @brief createRandomPassword
     * @return
     */
    Q_INVOKABLE QString createRandomPassword() const;
    /**
     * @brief despawnServer
     * @param savefile
     */
    Q_INVOKABLE void despawnServer(const QString savefile);
    /**
     * @brief despawnSlave
     * @param socketID
     */
    Q_INVOKABLE void despawnSlave(quint64 socketID);
    /**
     * @brief getAutoMatchMaker
     * @param matchMaker
     * @return
     */
    Q_INVOKABLE AutoMatchMaker* getAutoMatchMaker(const QString matchMaker);
    /**
     * @brief exit
     */
    Q_INVOKABLE void exit();
signals:
    void sigRemoveGame(NetworkGame* pGame);
    void sigStartRemoteGame(QString initScript, QString id);
    void sigExecuteServerScript();
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
     * @brief playerJoined a new player connected to server. Send him the initial lobby data
     * @param socketId
     */
    void playerJoined(qint64 socketId);

private slots:
    /**
     * @brief startRemoteGame used for ai training and to move data from one thread context to this one
     * @param map
     * @param configuration
     */
    void slotStartRemoteGame(QString initScript, QString id);
    /**
     * @brief disconnected
     * @param socketId
     */
    void disconnected(qint64 socketId);
    /**
     * @brief onMailSendResult
     * @param socketId
     * @param receiverAddress
     * @param username
     * @param result
     */
    void onMailSendResult(quint64 socketId, const QString receiverAddress, const QString username, bool result, NetworkCommands::PublicKeyActions action);
    /**
     * @brief executeScript
     */
    void executeScript();
    /**
     * @brief periodicTasks
     */
    void periodicTasks();
    /**
     * @brief doDespawnServer
     */
    void doDespawnServer();

private:
    /**
     * @brief despawnRunningSlaves
     */
    void despawnRunningSlaves();
    /**
     * @brief startDatabase
     */
    void startDatabase();
    /**
     * @brief restoreServer
     */
    void restoreServer();
    /**
     * @brief cleanUpSuspendedGames
     * @param games
     */
    void cleanUpSuspendedGames(QVector<SuspendedSlaveInfo> & games);
    /**
     * @brief setUuidForGame
     * @param game
     */
    void setUuidForGame(NetworkGameData & game);
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
     * @brief joinSlaveGame request of a client to join a specific slave game
     * @param socketID
     * @param stream
     */
    void joinSlaveGame(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief MainServer::tryJoinSuspendedGame
     * @param socketID
     * @param slave
     * @param games
     * @return
     */
    bool tryJoinSuspendedGame(quint64 socketID, const QString & slave, QVector<SuspendedSlaveInfo> & games);
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
    void spawnSlave(const QString & initScript, const QStringList & mods, QString id, quint64 socketID, QByteArray& data, QByteArray & minimapData, bool trainingSession = false);
    /**
     * @brief spawnSlave
     * @param slaveInfo
     */
    void spawnSlave(quint64 socketID, SuspendedSlaveInfo & slaveInfo);
    /**
     * @brief onSlaveReady called once a slave is in a state for receiving the data of the game that should be hosted
     * @param socketID
     * @param stream
     */
    void onSlaveReady(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onSlaveRelaunched
     * @param socketID
     * @param objData
     */
    void onSlaveRelaunched(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onRequestServerVersion
     * @param socketID
     * @param objData
     */
    void onRequestServerVersion(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief informClientsAboutRelaunch
     * @param games
     * @param slaveName
     * @param doc
     */
    bool informClientsAboutRelaunch(QVector<SuspendedSlaveInfo> & games, const QString & slaveName, const QJsonDocument & doc);
    /**
     * @brief onGameRunningOnServer called once the game has loaded all data needed for hosting a game and players can join
     * @param socketID
     * @param stream
     */
    void onGameRunningOnServer(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onSlaveGameStarted
     * @param socketID
     * @param objData
     */
    void onSlaveGameStarted(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onRequestUsergames
     * @param socketID
     * @param objData
     */
    void onRequestUsergames(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onRequestObservegames
     * @param socketID
     * @param objData
     */
    void onRequestObservegames(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onRequestGameData
     * @param socketId
     * @param objData
     */
    void onRequestGameData(quint64 socketId, const QJsonObject & objData);
    /**
     * @brief onOpenPlayerCount
     * @param socketID
     * @param stream
     */
    void onOpenPlayerCount(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief onRequestServerAutoMatchInfo
     * @param socketID
     * @param objData
     */
    void onRequestServerAutoMatchInfo(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief getNextFreeSlaveAddress
     * @param address
     * @param port
     */
    bool getNextFreeSlaveAddress(QString & address, quint16 & port, QString & secondaryAddress);
    /**
     * @brief parseSlaveAddressOptions
     */
    void parseSlaveAddressOptions();
    /**
     * @brief handleCryptedMessage
     * @param socketId
     * @param data
     * @param action
     */
    void handleCryptedMessage(qint64 socketId, const QJsonDocument & doc);
    /**
     * @brief createAccount
     * @param socketId
     * @param doc
     */
    void createAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
    /**
     * @brief deleteAccount
     * @param socketId
     * @param doc
     * @param action
     */
    void deleteAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
    /**
     * @brief loginToAccount
     * @param socketId
     * @param doc
     */
    void loginToAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
    /**
     * @brief createUserTable
     * @param username
     */
    void createUserTable(const QString & username);
    /**
     * @brief createMatchData
     * @param match
     */
    void createMatchData(const QString & match);
    /**
     * @brief checkPassword
     * @param database
     * @param username
     * @param password
     * @return
     */
    static GameEnums::LoginError checkPassword(QSqlDatabase & database, const QString & username, const QByteArray & password);
    /**
     * @brief resetAccountPassword
     * @param socketId
     * @param doc
     */
    void resetAccountPassword(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
    /**
     * @brief changeAccountPassword
     * @param socketId
     * @param doc
     * @param action
     */
    void changeAccountPassword(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
    /**
     * @brief getAccountInfo
     * @param username
     * @param success
     * @return
     */
    static QSqlQuery getAccountInfo(QSqlDatabase & database, const QString & username, bool & success);
    /**
     * @brief sendMail
     * @param message
     */
    void sendMail(QString message);
    /**
     * @brief getAllUsers
     * @param database
     * @param username
     * @param success
     * @return
     */
    QSqlQuery getAllUsers(QSqlDatabase & database, bool & success);
private:
    class InternNetworkGame;
    using spInternNetworkGame = std::shared_ptr<InternNetworkGame>;
    /**
     * @brief The InternNetworkGame class information about the process running a slave game
     */
    class InternNetworkGame final
    {
    public:
        std::shared_ptr<QProcess> process;
        spNetworkGame game;
        QString slaveName;
    };
    friend class MemoryManagement;
    explicit MainServer();
    static spMainServer m_pInstance;
    InternNetworkGame * getInternGame(const QString & slaveName, qint32 * index = nullptr);
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
    QVector<spInternNetworkGame> m_games;
    /**
     * @brief m_scriptExecutionTimer
     */
    QTimer m_periodicExecutionTimer;
    /**
     * @brief m_slaveAddressOptions address/port combination that can used for spawning a slave
     */
    QVector<AddressInfo> m_slaveAddressOptions;
    /**
     * @brief m_runningSlaves
     */
    QVector<SuspendedSlaveInfo> m_runningSlaves;
    /**
     * @brief m_runningLobbies
     */
    QVector<SuspendedSlaveInfo> m_runningLobbies;
    /**
     * @brief m_lastUsedAddressIndex last used index in the m_slaveAddressOptions
     */
    qint32 m_lastUsedAddressIndex{0};
    /**
     * @brief m_lastUsedPort last used port in the m_slaveAddressOptions
     */
    quint16 m_lastUsedPort{0};
    /**
     *
     */
    qint64 m_uuidGameCounter{1};
    /**
     * @brief m_freeAddresses addresses of slaves that have been used and are now free again
     */
    QVector<SlaveAddress> m_freeAddresses;

    MatchMakingCoordinator m_matchMakingCoordinator;
    /**
     * @brief m_serverData
     */
    static QSqlDatabase* m_serverData;
    /**
     * @brief m_mailSender
     */
    SmtpMailSender m_mailSender;
    /**
     * @brief m_mailSenderThread
     */
    QThread m_mailSenderThread;
    bool m_despawning{false};
    QString m_despawningSavefile;
};

Q_DECLARE_INTERFACE(MainServer, "MainServer");

#endif // MAINSERVER_H
