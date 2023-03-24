#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include <QObject>
#include <QTimer>
#include <QDir>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "menue/mapselectionmapsmenue.h"

#include "network/NetworkInterface.h"

#include "multiplayer/password.h"

#include "objects/base/chat.h"
#include "objects/dialogs/dialogconnecting.h"

class Multiplayermenu;
using spMultiplayermenu = oxygine::intrusive_ptr<Multiplayermenu>;

class Multiplayermenu final : public MapSelectionMapsMenue
{
    Q_OBJECT
public:
    enum class NetworkMode
    {
        Client,
        Host,
        Observer,
    };

    explicit Multiplayermenu(const QString & address, const QString & secondaryAddress, quint16 port, const QString & password, NetworkMode networkMode);
    explicit Multiplayermenu(const QString & address, quint16 port, const Password * password, NetworkMode networkMode);
    explicit Multiplayermenu(spNetworkInterface pNetworkInterface, const QString & password, NetworkMode networkMode);
    ~Multiplayermenu() = default;

    /**
     * @brief existsMap
     * @param fileName
     * @param hash
     */
    bool existsMap(QString& fileName, QByteArray& hash, QString& scriptFileName, QByteArray& scriptHash);
    /**
     * @brief createChat
     */
    void createChat();
    /**
     * @brief disconnectNetwork
     */
    void disconnectNetwork();
    /**
     * @brief connectNetworkSlots
     */
    void connectNetworkSlots();
    void disconnectNetworkSlots();
    /**
     * @brief showRuleSelection
     */
    virtual void showRuleSelection() override;
    virtual void showPlayerSelection(bool relaunchedLobby = false) override;
    /**
     * @brief doSaveLobbyState
     * @param filename
     */
    QJsonDocument doSaveLobbyState(const QString & saveFile, const QString & command);
signals:
    void sigConnected();
    void sigHostGameLaunched();
    void sigLoadSaveGame();
    void sigShowIPs();
    void sigReadyAndLeave();
    void sigServerResponded();
public slots:

    // general slots
    virtual void buttonBack() override;
    virtual void buttonNext() override;
    virtual void startGame() override;
    virtual void exitMenu() override;
    void exitMenuToLobby();

    // network slots
    void playerJoined(quint64 socketID);
    void disconnected(quint64 socketID);
    /**
     * @brief recieveData receive data from an client
     * @param socketID
     * @param data
     * @param service
     */
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief recieveServerData receive data from the server hosting this slave
     * @param socketID
     * @param data
     * @param service
     */
    void recieveServerData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);

    virtual void hideMapSelection() override;
    virtual void showMapSelection() override;

    void showLoadSaveGameDialog();
    void loadSaveGame(QString filename);

    void slotCancelHostConnection();
    void slotHostGameLaunched();
    /**
     * @brief onSlaveConnectedToMaster
     * @param socketID
     */
    void onSlaveConnectedToMaster(quint64 socketID);
    /**
     * @brief onServerRelaunchSlave
     * @param socketID
     * @param objData
     */
    void onServerRelaunchSlave(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief despawnSlave
     */
    void despawnSlave();
    /**
     * @brief doDespawnSlave
     * @return
     */
    bool doDespawnSlave();

protected slots:
    /**
     * @brief countdown
     */
    void countdown();
    /**
     * @brief closeSlave
     */
    void closeSlave();
    /**
     * @brief readyAndLeave
     */
    void readyAndLeave();
    /**
     * @brief doReadyAndLeave
     */
    void doReadyAndLeave();

protected:
    /**
     * @brief relaunchRunningGame
     */
    void relaunchRunningGame(quint64 socketID, const QString & savefile);
    /**
     * @brief relaunchRunningLobby
     */
    void relaunchRunningLobby(quint64 socketID, const QString & savefile);
    /**
     * @brief sendSlaveRelaunched
     */
    void sendSlaveRelaunched(quint64 socketID);
    /**
     * @brief startDespawnTimer
     */
    void startDespawnTimer();
    /**
     * @brief saveLobbyState
     * @param filename
     */
    void saveLobbyState(const QString & filename);
    /**
     * @brief getGameReady
     * @return
     */
    bool getGameReady();
    void sendServerReady(bool value);
    void initClientGame(quint64 socketID, QDataStream &stream);
    void loadMultiplayerMap(bool relaunchedLobby = false);
    void showIPs();
    spGameMap createMapFromStream(QString mapFile, QString scriptFile, QDataStream &stream);
    QString getNewFileName(QString filename);    
    void clientMapInfo(QDataStream & stream, quint64 socketID);
    void readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, QStringList & versions, QStringList & myMods, QStringList & myVersions, bool & sameMods, bool & differentHash, bool & sameVersion);
    void handleVersionMissmatch(const QStringList & mods, const QStringList & versions, const QStringList & myMods, const QStringList & myVersions, bool sameMods, bool differentHash, bool sameVersion);
    bool checkMods(const QStringList & mods, const QStringList & versions, QStringList & myMods, QStringList & myVersions, bool filter);
    void verifyGameData(QDataStream & stream, quint64 socketID);
    /**
     * @brief filterCosmeticMods
     * @param mods
     * @param versions
     */
    void filterCosmeticMods(QStringList & mods, QStringList & versions, bool filter);
    /**
     * @brief requestRule
     * @param socketID
     */
    void requestRule(quint64 socketID);
    /**
     * @brief sendInitUpdate
     * @param stream
     * @param socketID
     */
    void sendInitUpdate(QDataStream & stream, quint64 socketID);
    /**
     * @brief requestMap
     * @param socketID
     */
    void requestMap(quint64 socketID);
    /**
     * @brief recieveMap
     * @param stream
     * @param socketID
     */
    void recieveMap(QDataStream & stream, quint64 socketID);
    /**
     * @brief playerJoinedServer
     * @param stream
     * @param socketID
     */
    void playerJoinedServer(QDataStream & stream, quint64 socketID);
    /**
     * @brief findAndLoadMap
     * @param dirIter
     * @param hash
     * @param m_saveGame
     * @return
     */
    bool findAndLoadMap(QDirIterator & dirIter, QByteArray& hash, bool m_saveGame);
    /**
     * @brief sendJoinReason
     * @param stream
     * @param socketID
     */
    void sendJoinReason(QDataStream & stream, quint64 socketID);
    /**
     * @brief receiveCurrentGameState
     * @param stream
     * @param socketID
     */
    void receiveCurrentGameState(QDataStream & stream, quint64 socketID);
    /**
     * @brief connectToSlave
     * @param objData
     * @param socketID
     */
    void connectToSlave(const QJsonObject & objData, quint64 socketID);
    /**
     * @brief startRejoinedGame
     * @param syncCounter
     */
    void startRejoinedGame(qint64 syncCounter);
    /**
     * @brief receivePlayerControlledInfo
     * @param stream
     * @param socketID
     */
    void receivePlayerControlledInfo(QDataStream & stream, quint64 socketID);
    /**
     * @brief showDisconnectReason
     * @param socketID
     * @param objData
     */
    void showDisconnectReason(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief sendUsername
     */
    void sendUsername(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief sendLoginData
     * @param socketID
     * @param objData
     * @param action
     */
    void sendLoginData(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
    /**
     * @brief verifyLoginData
     * @param objData
     * @param socketID
     */
    void verifyLoginData(const QJsonObject & objData, quint64 socketID);
    /**
     * @brief sendMapInfoUpdate
     * @param socketID
     * @param objData
     * @param action
     */
    void sendMapInfoUpdate(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
private:
    /**
     * @brief init
     */
    void init();
    /**
     * @brief startGameOnServer
     */
    void startGameOnServer();
    /**
     * @brief launchGameOnServer
     * @param stream
     */
    void launchGameOnServer(QDataStream & stream);
    /**
     * @brief sendSlaveReady
     */
    void sendSlaveReady();
    /**
     * @brief acceptNewConnection
     * @param socketID
     */
    void acceptNewConnection(quint64 socketID);
    /**
     * @brief initClientAndWaitForConnection
     */
    void initClientAndWaitForConnection();
    /**
     * @brief startCountdown
     */
    void startCountdown();
    /**
     * @brief markGameReady
     */
    void markGameReady(bool fixed = false);
    /**
     * @brief changeButtonText
     */
    void changeButtonText();
    /**
     * @brief showInformingServer
     */
    void showInformingServer();
    /**
     * @brief initClientConnection
     */
    void initClientConnection(const QString & address, const QString & secondaryAddress, quint16 port);
private:
    NetworkMode m_networkMode{NetworkMode::Client};
    spNetworkInterface m_pNetworkInterface;
    oxygine::spButton m_pHostAdresse;
    spChat m_Chat;
    QTimer m_GameStartTimer;
    qint32 m_counter{5};
    oxygine::spButton m_pButtonLoadSavegame;
    oxygine::spButton m_pReadyAndLeave;
    bool m_saveGame{false};
    bool m_local{true};
    bool m_slaveGameReady{false};
    Password m_password;
    quint64 m_hostSocket{0};
    spDialogConnecting m_pDialogConnecting;
    QElapsedTimer m_slaveDespawnElapseTimer;
    QTimer m_slaveDespawnTimer{this};
    bool m_despawning{false};
};

Q_DECLARE_INTERFACE(Multiplayermenu, "Multiplayermenu");

#endif // MULTIPLAYERMENU_H
