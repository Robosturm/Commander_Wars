#ifndef MULTIPLAYERMENU_H
#define MULTIPLAYERMENU_H

#include "memory.h"
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

class Multiplayermenu : public MapSelectionMapsMenue
{
    Q_OBJECT
public:
    enum class NetworkMode
    {
        Client,
        Host,
        Observer,
    };

    explicit Multiplayermenu(QString adress, quint16 port, QString password, NetworkMode host);
    explicit Multiplayermenu(spNetworkInterface pNetworkInterface, QString password, NetworkMode host);
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
    virtual void showPlayerSelection() override;
signals:
    void sigConnected();
    void sigHostGameLaunched();
    void sigLoadSaveGame();
    void sigShowIPs();
public slots:

    // general slots
    virtual void buttonBack() override;
    virtual void buttonNext() override;
    virtual void startGame() override;
    virtual void exitMenu() override;

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

protected slots:
    void countdown();
protected:
    bool getGameReady();
    void sendServerReady(bool value);
    void initClientGame(quint64 socketID, QDataStream &stream);
    void loadMultiplayerMap();
    void showIPs();
    spGameMap createMapFromStream(QString mapFile, QString scriptFile, QDataStream &stream);
    QString getNewFileName(QString filename);    
    void clientMapInfo(QDataStream & stream, quint64 socketID);
    void readHashInfo(QDataStream & stream, quint64 socketID, QStringList & mods, bool & sameMods, bool & differentHash, bool & sameVersion);
    void handleVersionMissmatch(const QStringList & mods, bool sameMods, bool differentHash, bool sameVersion);
    bool checkMods(const QStringList & mods, const QStringList & versions, bool filter);
    void verifyGameData(QDataStream & stream, quint64 socketID);
    /**
     * @brief filterCosmeticMods
     * @param mods
     * @param versions
     */
    void filterCosmeticMods(QStringList & mods, QStringList & versions, bool filter);
    void requestRule(quint64 socketID);
    void sendInitUpdate(QDataStream & stream, quint64 socketID);
    void requestMap(quint64 socketID);
    void recieveMap(QDataStream & stream, quint64 socketID);
    void playerJoinedServer(QDataStream & stream, quint64 socketID);
    bool findAndLoadMap(QDirIterator & dirIter, QByteArray& hash, bool m_saveGame);
    void sendJoinReason(QDataStream & stream, quint64 socketID);
    void receiveCurrentGameState(QDataStream & stream, quint64 socketID);
    void connectToSlave(const QJsonObject & objData, quint64 socketID);
    void startRejoinedGame(qint64 syncCounter);
    void receivePlayerControlledInfo(QDataStream & stream, quint64 socketID);
    void showDisconnectReason(quint64 socketID, const QJsonObject & objData);
    void sendLoginData(quint64 socketID, const QJsonObject & objData, NetworkCommands::PublicKeyActions action);
    void verifyLoginData(const QJsonObject & objData, quint64 socketID);
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
    void markGameReady();
    /**
     * @brief changeButtonText
     */
    void changeButtonText();
private:
    NetworkMode m_networkMode{NetworkMode::Client};
    spNetworkInterface m_pNetworkInterface;
    oxygine::spButton m_pHostAdresse;
    spChat m_Chat;
    QTimer m_GameStartTimer;
    qint32 m_counter{5};
    oxygine::spButton m_pButtonLoadSavegame;
    bool m_saveGame{false};
    bool m_local{true};
    bool m_slaveGameReady{false};
    Password m_password;
    quint64 m_hostSocket{0};
    spDialogConnecting m_pDialogConnecting;
};

#endif // MULTIPLAYERMENU_H
