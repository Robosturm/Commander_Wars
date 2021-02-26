#ifndef GAMEMENUE_H
#define GAMEMENUE_H

#include <QObject>
#include <QTimer>

#include <menue/ingamemenue.h>

#include "network/NetworkInterface.h"

#include "objects/base/chat.h"
#include "objects/base/closeablepopup.h"
#include "objects/base/label.h"

#include "game/gamerecording/replayrecorder.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/ui/playerinfo.h"
#include "game/ui/ingameinfobar.h"

#include "coreengine/LUPDATE_MACROS.h"

class GameMenue;
typedef oxygine::intrusive_ptr<GameMenue> spGameMenue;

/**
 * @brief The GameMenue class handles the game :)
 */
class GameMenue : public InGameMenue
{
    Q_OBJECT
public:
    ENUM_CLASS  AnimationSkipMode
    {
        None,
        All,
        Battle
    };

    explicit GameMenue(bool saveGame, spNetworkInterface pNetworkInterface);
    explicit GameMenue(QString map, bool saveGame);
    explicit GameMenue();
    virtual ~GameMenue();
    static GameMenue* getInstance()
    {
        return m_pInstance.get();
    }
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
    IngameInfoBar* getGameInfoBar()
    {
        return m_IngameInfoBar.get();
    }
    Chat* getChat() const;
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    virtual Player* getCurrentViewPlayer();
    /**
     * @brief getSyncCounter
     * @return
     */
    qint64 getSyncCounter() const;
    /**
     * @brief getSkipMode
     * @return
     */
    AnimationSkipMode getSkipMode();
    /**
     * @brief autoScroll
     */
    virtual void autoScroll() override;
signals:
    void sigActionPerformed();
    void sigGameStarted();
    void sigSaveGame();
    void sigExitGame();
    void sigShowExitGame();
    void sigShowSurrenderGame();
    void sigNicknameUnit(qint32 x, qint32 y, QString name);
public slots:
    /**
     * @brief centerMapOnAction
     * @param pGameAction
     */
    void centerMapOnAction(GameAction* pGameAction);
    /**
     * @brief actionPerformed
     */
    void actionPerformed();
    /**
     * @brief performAction performs the given action and deletes it afterwards.
     * @param pGameAction
     */
    void performAction(spGameAction pGameAction);
    /**
     * @brief isTrap
     * @param pAction
     * @param pMoveUnit
     * @param currentPoint
     * @param previousPoint
     * @return
     */
    bool isTrap(QString function, spGameAction pAction, Unit* pMoveUnit, QPoint currentPoint, QPoint previousPoint, qint32 moveCost);
    /**
     * @brief finsihActionPerformed
     */
    void finishActionPerformed();
    /**
     * @brief skipAnimations
     */
    void skipAnimations();
    /**
     * @brief doMultiTurnMovement
     * @param pGameAction
     */
    spGameAction doMultiTurnMovement(spGameAction pGameAction);
    /**
     * @brief updatePlayerinfo
     */
    void updatePlayerinfo();
    /**
     * @brief cursorMoved
     * @param x
     * @param y
     */
    void cursorMoved(qint32 x, qint32 y);
    /**
     * @brief victory
     * @param team
     */
    void victory(qint32 team);
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
     * @brief showOptions
     */
    void showOptions();
    /**
     * @brief showChangeSound
     */
    void showChangeSound();
    /**
     * @brief showAttackLog
     */
    void showAttackLog(qint32 player);
    /**
     * @brief showAttackLog
     */
    void showUnitInfo(qint32 player);
    /**
     * @brief saveMap
     * @param file
     */
    void saveMap(QString filename);
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
     * @brief keyInput
     * @param event
     */
    virtual void keyInput(oxygine::KeyEvent event) override;
    /**
     * @brief editFinishedCanceled
     */
    void editFinishedCanceled();
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
protected slots:
    /**
     * @brief updateTimer
     */
    void updateTimer();
protected:
    void loadUIButtons();
    void loadGameMenue();
    void connectMap();
    void keyInputAll(Qt::Key cur);
    QString getSaveFileEnding();
    void skipAllAnimations();
    void skipExceptBattle();
protected:
    ReplayRecorder m_ReplayRecorder;
    spPlayerInfo m_pPlayerinfo;
    spIngameInfoBar m_IngameInfoBar;
    spLabel xyTextInfo;
    oxygine::spActor m_XYButtonBox;
    static spGameMenue m_pInstance;
    spChat m_pChat{nullptr};
    oxygine::spButton m_ChatButton{nullptr};
    spNetworkInterface m_pNetworkInterface;
    bool gameStarted{false};
    QVector<quint64> m_PlayerSockets;
    QVector<quint64> m_ReadySockets;
    oxygine::spTextField m_CurrentRoundTime;
    bool m_SaveGame{false};
    bool m_Multiplayer{false};

    QTimer m_UpdateTimer;
    QTimer m_AutoSavingTimer;
    spGameAction m_pStoredAction{nullptr};
    spGameAction m_pCurrentAction{nullptr};

    qint64 m_syncCounter{0};
    qint32 m_autoSaveCounter{0};
};

#endif // GAMEMENUE_H
