#pragma once

#include <QObject>
#include <QTimer>

#include "menue/basegamemenu.h"

#include "network/NetworkInterface.h"

#include "objects/base/chat.h"
#include "objects/base/closeablepopup.h"
#include "objects/base/label.h"

#include "game/gamerecording/replayrecorder.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/ui/playerinfo.h"
#include "game/ui/ingameinfobar.h"
#include "game/ui/humanquickbuttons.h"
#include "game/actionperformer.h"

class WikiView;
class GameMenue;
using spGameMenue = oxygine::intrusive_ptr<GameMenue>;

class MovementPlanner;
using spMovementPlanner = oxygine::intrusive_ptr<MovementPlanner>;

/**
 * @brief The GameMenue class handles the game
 */
class GameMenue : public BaseGamemenu
{
    Q_OBJECT
public:
    explicit GameMenue(spGameMap pMap, bool saveGame, spNetworkInterface pNetworkInterface, bool rejoin = false, bool startDirectly = false);
    explicit GameMenue(QString map, bool saveGame);
    explicit GameMenue(spGameMap pMap);
    virtual ~GameMenue();
    /**
     * @brief attachInterface
     * @param pNetworkInterface
     */
    void attachInterface(spNetworkInterface pNetworkInterface);
    /**
     * @brief getGameStarted
     * @return always true for singleplayer games turns true on multiplayer games once all clients have started the game
     */
    bool getGameStarted() const;
    /**
     * @brief getGameInfoBar
     * @return
     */
    IngameInfoBar* getGameInfoBar();
    /**
     * @brief getChat
     * @return
     */
    Chat* getChat() const;
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    virtual Player* getCurrentViewPlayer() override;
    /**
     * @brief getSyncCounter
     * @return
     */
    qint64 getSyncCounter() const;
    /**
     * @brief autoScroll
     */
    virtual void autoScroll(QPoint cursorPosition) override;
    bool getIsReplay() const;
    void setIsReplay(bool isReplay);

    bool getActionRunning() const;
    void updateQuickButtons();
    void setSaveMap(bool newSaveMap);
    bool getSaveMap() const;

    bool getSaveAllowed() const;
    void setSaveAllowed(bool newSaveAllowed);
    /**
     * @brief doSaveMap
     */
    void doSaveMap();
    void setGameStarted(bool newGameStarted);
    /**
     * @brief getReplayRecorder
     * @return
     */
    ReplayRecorder &getReplayRecorder();
    /**
     * @brief getNetworkInterface
     * @return
     */
    NetworkInterface* getNetworkInterface();
    /**
     * @brief getActionPerformer
     * @return
     */
    ActionPerformer &getActionPerformer();
    /**
     * @brief getIsMultiplayer
     * @param pGameAction
     * @return
     */
    bool getIsMultiplayer(const spGameAction & pGameAction) const;

signals:
    void sigGameStarted();
    void sigSyncFinished();
    void sigSaveGame();
    void sigExitGame();
    void sigShowExitGame();
    void sigShowSurrenderGame();
    void sigNicknameUnit(qint32 x, qint32 y, QString name);
    void sigVictory(qint32 team);
    void sigLoadSaveGame(const QString savefile);
public slots:
    /**
     * @brief updatePlayerinfo
     */
    void updatePlayerinfo();
    /**
     * @brief updateMinimap
     */
    void updateMinimap();
    /**
     * @brief updateGameInfo
     */
    void updateGameInfo();
    /**
     * @brief cursorMoved
     * @param x
     * @param y
     */
    void cursorMoved(qint32 x, qint32 y);
    /**
     * @brief doPlayerInfoFlipping
     */
    void doPlayerInfoFlipping();
    /**
     * @brief victory
     * @param team
     */
    virtual void victory(qint32 team);
    /**
     * @brief saveGame
     */
    void saveGame();
    /**
     * @brief victoryInfo
     */
    void victoryInfo();
    /**
     * @brief showCOInfo
     */
    void showCOInfo();
    /**
     * @brief showGameInfo
     */
    void showGameInfo(qint32 player);
    /**
     * @brief showXmlFileDialog
     */
    void showXmlFileDialog(const QString & xmlFile, bool saveSettings = false);
    /**
     * @brief showAttackLog
     */
    void showAttackLog(qint32 player);
    /**
     * @brief showRuleSelection
     */
    void showRules();
    /**
     * @brief showAttackLog
     */
    void showUnitInfo(qint32 player);
    /**
     * @brief showUnitStatistics
     */
    void showUnitStatistics(qint32 player);
    /**
     * @brief showPlayerUnitStatistics
     * @param pPlayer
     */
    void showPlayerUnitStatistics(Player* pPlayer);
    /**
     * @brief saveMap
     * @param file
     */
    void saveMap(QString filename, bool skipAnimations = true);
    /**
     * @brief autoSaveMap
     */
    void autoSaveMap();
    /**
     * @brief saveMapAndExit
     * @param filename
     */
    void saveMapAndExit(QString filename);
    /**
     * @brief exitGame
     */
    void exitGame();
    /**
     * @brief surrenderGame
     */
    void surrenderGame();
    /**
     * @brief nicknameUnit
     */
    void showNicknameUnit(qint32 x, qint32 y);
    /**
     * @brief nicknameUnit
     * @param x
     * @param y
     * @param name
     */
    void nicknameUnit(qint32 x, qint32 y, QString name);
    /**
     * @brief editFinishedCanceled
     */
    void editFinishedCanceled();
    /**
     * @brief isNetworkGame
     * @return
     */
    bool isNetworkGame();
    /**
     * @brief playerJoined
     * @param socketID
     */
    void playerJoined(quint64 socketID);
    /**
     * @brief showDisconnectReason
     * @param socketID
     * @param objData
     */
    void showDisconnectReason(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief sendUsername
     * @param socketID
     * @param objData
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
     * @brief sendVerifyGameData
     * @param socketID
     */
    void sendVerifyGameData(quint64 socketID);
    /**
     * @brief sendRequestJoinReason
     * @param socketID
     */
    void sendRequestJoinReason(quint64 socketID);
    /**
     * @brief continueAfterSyncGame
     */
    void continueAfterSyncGame();
    /**
     * @brief showExitGame
     */
    virtual void showExitGame();
    /**
     * @brief showSurrenderGame
     */
    void showSurrenderGame();
    /**
     * @brief startGame
     * @param startPlayer
     */
    void startGame();
    /**
     * @brief showSaveAndExitGame
     */
    void showSaveAndExitGame();
    /**
     * @brief showWiki
     */
    WikiView* showWiki();
    /**
     * @brief showDamageCalculator
     */
    void showDamageCalculator();
    /**
     * @brief showMovementPlaner
     */
    void showMovementPlanner();
    /**
     * @brief hideMovementPlanner
     */
    void hideMovementPlanner();
    /**
     * @brief exitMovementPlanner
     */
    void exitMovementPlanner();
    /**
     * @brief unhideGameMenue
     */
    void unhideGameMenue();
    /**
     * @brief checkMovementPlanner
     */
    void checkMovementPlanner();
    /**
     * @brief showLoadSaveGame
     */
    void showLoadSaveGame();
    /**
     * @brief loadSaveGame
     */
    void loadSaveGame(const QString savefile);
protected slots:
    /**
     * @brief keyInput
     * @param event
     */
    virtual void keyInput(oxygine::KeyEvent event) override;
    /**
     * @brief recieveData
     * @param socketID
     * @param data
     * @param service
     */
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief disconnected
     * @param socketID
     */
    void disconnected(quint64 socketID);
    /**
     * @brief despawnSlave
     */
    void despawnSlave();
    /**
     * @brief joinAsObserver
     * @param stream
     * @param socketID
     */
    void joinAsObserver(QDataStream & stream, quint64 socketID);
    /**
     * @brief startGameSync
     */
    void startGameSync(quint64 socketID);
    /**
     * @brief joinAsPlayer
     * @param stream
     * @param socketID
     */
    void joinAsPlayer(QDataStream & stream, quint64 socketID);
    /**
     * @brief waitForPlayerJoinSyncFinished
     * @param stream
     * @param socketID
     */
    void waitForPlayerJoinSyncFinished(QDataStream & stream, quint64 socketID);
    /**
     * @brief waitingForPlayerJoinSyncFinished
     */
    void waitingForPlayerJoinSyncFinished(QDataStream & stream, quint64 socketID);
    /**
     * @brief syncPointReached
     */
    void syncPointReached();
    /**
     * @brief receivedUsername
     * @param socketID
     * @param objData
     */
    void receivedUsername(quint64 socketID, const QJsonObject & objData);
    /**
     * @brief playerRequestControl
     * @param stream
     * @param socketID
     */
    void playerRequestControlInfo(QDataStream & stream, quint64 socketId);
    /**
     * @brief checkSendPlayerRequestControlInfo
     */
    void checkSendPlayerRequestControlInfo();
    /**
     * @brief sendPlayerRequestControlInfo
     * @param playerNameId
     * @param socketId
     */
    void sendPlayerRequestControlInfo(const QString & playerNameId, quint64 socketId);
    /**
     * @brief removePlayerFromSyncWaitList
     * @param socketID
     */
    void removePlayerFromSyncWaitList(quint64 socketID);
    /**
     * @brief playerJoinedFinished
     */
    void playerJoinedFinished();
    /**
     * @brief updateTimer
     */
    void updateTimer();
    /**
     * @brief onEnter
     */
    virtual void onEnter() override;
protected:
    /**
     * @brief startDespawnTimer
     */
    void startDespawnTimer();
    void loadUIButtons();
    void loadGameMenue();
    void connectMap();
    void keyInputAll(Qt::Key cur);
    QString getSaveFileEnding();
    void showChat();
    /**
     * @brief sendGameStartedToServer
     */
    void sendGameStartedToServer();
protected:
    ReplayRecorder m_ReplayRecorder;
    spPlayerInfo m_pPlayerinfo;
    spIngameInfoBar m_IngameInfoBar;
    oxygine::spBox9Sprite m_pButtonBox;
    spLabel m_xyTextInfo;
    oxygine::spBox9Sprite m_XYButtonBox;
    spHumanQuickButtons m_humanQuickButtons;
    spChat m_pChat{nullptr};
    oxygine::spButton m_ChatButton{nullptr};
    oxygine::spTween m_chatButtonShineTween{nullptr};
    bool m_gameStarted{false};
    oxygine::spTextField m_CurrentRoundTime;
    bool m_SaveGame{false};

    // multiplayer data
    QVector<quint64> m_PlayerSockets;
    QVector<quint64> m_ReadySockets;
    spNetworkInterface m_pNetworkInterface;
    bool m_Multiplayer{false};
    QElapsedTimer m_slaveDespawnElapseTimer;
    QTimer m_slaveDespawnTimer{this};
    QTimer m_UpdateTimer{this};
    struct Userdata
    {
        QString username;
        quint64 socket;
    };
    QVector<Userdata> m_userData;
    QVector<Userdata> m_requestData;

    bool m_enabledAutosaving{false};

    QString m_saveFile;
    bool m_saveMap{false};
    bool m_exitAfterSave{false};
    bool m_saveAllowed{false};
    bool m_isReplay{false};

    ActionPerformer m_actionPerformer;
    spMovementPlanner m_pMovementPlanner;
};

Q_DECLARE_INTERFACE(GameMenue, "GameMenue");
