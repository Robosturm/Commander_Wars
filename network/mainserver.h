#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include <QSqlDatabase>
#include <QProcess>

#include "network/tcpserver.h"
#include "network/tcpclient.h"
#include "network/networkgamedata.h"
#include "network/networkgame.h"
#include "network/smtpmailsender.h"

#include "multiplayer/networkcommands.h"

#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"

#include "game/GameEnums.h"

class MainServer;
using spMainServer = oxygine::intrusive_ptr<MainServer>;

/**
 * @brief The MainServer class handling the server and it's data.
 */
class MainServer : public QObject, public oxygine::ref_counter
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

public:
    static MainServer* getInstance();
    static bool exists();
    static void initDatabase();
    static GameEnums::LoginError verifyLoginData(const QString & username, const QByteArray & password);
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
    /**
     * @brief createRandomPassword
     * @return
     */
    QString createRandomPassword() const;
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
    void onMailSendResult(quint64 socketId, const QString & receiverAddress, const QString & username, bool result, NetworkCommands::PublicKeyActions action);
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
    void joinSlaveGame(quint64 socketID, const QJsonObject & objData);
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
    void onSlaveReady(quint64 socketID, const QJsonObject & objData);
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
     * @brief loginToAccount
     * @param socketId
     * @param doc
     */
    void loginToAccount(qint64 socketId, const QJsonDocument & doc, NetworkCommands::PublicKeyActions action);
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
     * @brief sqlQueryFailed
     * @param query
     * @return
     */
    static bool sqlQueryFailed(const QSqlQuery & query);
    /**
     * @brief sendMail
     * @param message
     */
    void sendMail(QString message);
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

    /**
     * @brief m_slaveAddressOptions address/port combination that can used for spawning a slave
     */
    QVector<AddressInfo> m_slaveAddressOptions;
    /**
     * @brief m_lastUsedAddressIndex last used index in the m_slaveAddressOptions
     */
    qint32 m_lastUsedAddressIndex{0};
    /**
     * @brief m_lastUsedPort last used port in the m_slaveAddressOptions
     */
    quint16 m_lastUsedPort{0};
    /**
     * @brief m_freeAddresses addresses of slaves that have been used and are now free again
     */
    QVector<SlaveAddress> m_freeAddresses;
    /**
     * @brief m_serverData
     */
    static QSqlDatabase m_serverData;
    /**
     * @brief m_dataBaseLaunched
     */
    static bool m_dataBaseLaunched;
    /**
     * @brief m_mailSender
     */
    SmtpMailSender m_mailSender;
    /**
     * @brief m_mailSenderThread
     */
    QThread m_mailSenderThread;
};

#endif // MAINSERVER_H
