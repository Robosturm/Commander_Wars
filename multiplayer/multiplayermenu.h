#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include <QObject>
#include <QTimer>
#include <QDir>
#include <QMap>
#include <QSet>
#include <QPair>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "coreengine/filesupport.h"

#include "menue/mapselectionmapsmenue.h"

#include "network/networkInterface.h"

#include "multiplayer/password.h"
#include "multiplayer/networkcommands.h"

#include "objects/base/chat.h"
#include "objects/dialogs/dialogconnecting.h"
#include "objects/dialogs/dialogmodsyncprogress.h"

class Multiplayermenu;
using spMultiplayermenu = std::shared_ptr<Multiplayermenu>;

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
    virtual ~Multiplayermenu() = default;

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

    bool getSameVersionAsServer() const;
    void setSameVersionAsServer(bool newSameVersionAsServer);

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
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket);
    /**
     * @brief recieveServerData receive data from the server hosting this slave
     * @param socketID
     * @param data
     * @param service
     */
    void recieveServerData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 senderSocket);

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
    bool doDespawnSlave(const QString & savegame);

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
    void readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, QStringList & versions, QStringList & myMods, QStringList & myVersions, QStringList & mismatchedResourceFolders, QStringList & mismatchedMods, QMap<QString, QByteArray> & hostModHashes, quint32 & hostCapabilities, bool & sameMods, bool & differentHash, bool & sameVersion, bool & cosmeticAllowed);
    void handleVersionMissmatch(const QStringList & mods, const QStringList & versions, const QStringList & myMods, const QStringList & myVersions, const QStringList & mismatchedResourceFolders, const QStringList & mismatchedMods, const QMap<QString, QByteArray> & hostModHashes, quint32 hostCapabilities, bool sameMods, bool differentHash, bool sameVersion, bool cosmeticAllowed);
    void confirmModSync(const QStringList & modsToDownload, const QStringList & postSyncActiveMods);
    void startModSyncDownload(const QStringList & modsToDownload, const QStringList & postSyncActiveMods);
    void onModSyncProgress();
    void onModSyncSucceeded();
    void onModSyncFailed(const QString & reason);
    bool checkMods(const QStringList & mods, const QStringList & versions, QStringList & myMods, QStringList & myVersions, bool filter);
    void verifyGameData(QDataStream & stream, quint64 socketID);
    bool requestModSync(const QStringList & modsToDownload, const QStringList & postSyncActiveMods);
    void handleModSyncRequest(QDataStream & stream, quint64 socketID);
    void handleModSyncManifest(QDataStream & stream, quint64 socketID);
    void handleModSyncData(QDataStream & stream, quint64 socketID);
    void handleModSyncModBegin(QDataStream & stream, quint64 socketID);
    void handleModSyncModChunk(QDataStream & stream, quint64 socketID);
    void handleModSyncModEnd(QDataStream & stream, quint64 socketID);
    void handleModSyncReject(QDataStream & stream, quint64 socketID);
    void handleModSyncComplete(QDataStream & stream, quint64 socketID);
    void sendModSyncReject(quint64 socketID, qint32 reasonCode, const QString & modPath, const QString & message);
    void cancelModSyncSession();
    // Drives the host-side chunked send loop one chunk per event-loop iteration so a large mod cannot pin the GUI thread.
    void pumpModSyncSend();
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
    void sendLoginData(quint64 socketID, const QJsonObject & objData);
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
    void sendMapInfoUpdate(quint64 socketID);
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
     * @brief startGatewayGameOnServer
     */
    void startGatewayGameOnServer();
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
    /**
     * @brief waitForServerConnection
     */
    void waitForServerConnection();
    /**
     * @brief disconnectGateway
     */
    void disconnectGateway();
    /**
     * @brief isGateway
     */
    bool isGatewayGame();
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
    QString m_serverAddress;
    quint16 m_serverPort{0};
    spDialogConnecting m_pDialogConnecting;
    // Held as a member so the mod-sync flow can dismiss it before stacking a second Cancel button.
    spDialogConnecting m_pJoinConnectingDialog;
    QElapsedTimer m_slaveDespawnElapseTimer;
    QTimer m_slaveDespawnTimer{this};
    bool m_despawning{false};
    bool m_sameVersionAsServer{false};

    // Mod-sync client-session state; cleared on completion or abort.
    QList<QPair<QString, QString>> m_modSyncStagings;
    QSet<QString> m_modSyncRequestedSet;
    QStringList m_modSyncPostSyncActiveMods;
    qint64 m_modSyncReceivedBytes{0};
    qint64 m_modSyncReceivedUncompressedBytes{0};
    // Sum of declaredUncompressedSize from MODSYNCMANIFEST; 0 when older hosts skip the frame.
    qint64 m_modSyncExpectedUncompressedTotal{0};
    bool m_modSyncActive{false};
    spDialogModSyncProgress m_modSyncProgressDialog;

    // Client-side chunked-receive accumulator. modPath empty when no chunked mod is in flight (legacy single-frame path stays untouched).
    struct ModSyncChunkAccumulator
    {
        QString modPath;
        qint32 declaredUncompressedSize{0};
        qint32 fileCount{0};
        qint64 compressedTotal{0};
        qint32 expectedChunkCount{0};
        qint32 receivedChunkCount{0};
        qint64 uncompressedAdvanced{0};
        QByteArray blob;
    };
    ModSyncChunkAccumulator m_modSyncCurrentChunkMod;

    // Host-side chunked-send pump state. socketID==0 means no send in flight.
    struct ModSyncSendState
    {
        quint64 socketID{0};
        QVector<QPair<QString, Filesupport::ModSyncPackage>> packages;
        qint32 currentMod{0};
        qint32 currentChunk{0};
        bool useChunked{false};
        bool beginEmitted{false};
    };
    ModSyncSendState m_modSyncSendState;
};

Q_DECLARE_INTERFACE(Multiplayermenu, "Multiplayermenu");

#endif // MULTIPLAYERMENU_H
