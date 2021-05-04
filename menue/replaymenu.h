#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include <QMutex>

#include "menue/gamemenue.h"

#include "objects/base/v_scrollbar.h"

#include "gameinput/humanplayerinput.h"
#include "game/viewplayer.h"
#include "game/GameEnums.h"

class ReplayMenu;
using spReplayMenu = oxygine::intrusive_ptr<ReplayMenu>;

class ReplayMenu : public GameMenue
{
    Q_OBJECT
    static constexpr qint32 actionPixelSize = 5;
public:
    ReplayMenu(QString filename);
    virtual ~ReplayMenu();
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    virtual Player* getCurrentViewPlayer() override;
signals:
    void sigShowRecordInvalid();
    void sigExitReplay();
    void sigSwapPlay();
    void sigStartFastForward();
    void sigStopFastForward();
    void sigShowConfig();
public slots:
    void showRecordInvalid();
    void exitReplay();
    void nextReplayAction();
    virtual void showExitGame() override;
    void swapPlay();
    void startFastForward();
    void stopFastForward();
    void showConfig();
    void setViewTeam(qint32 item);
    /**
     * @brief startSeeking
     */
    void startSeeking();
    /**
     * @brief seekRecord
     * @param value
     */
    void seekRecord(float value);
    /**
     * @brief seekChanged
     * @param value
     */
    void seekChanged(float value);
    /**
     * @brief togglePlayUi
     */
    void togglePlayUi();
protected:
    /**
     * @brief loadUIButtons
     */
    void loadUIButtons();
    /**
     * @brief loadSeekUi
     */
    void loadSeekUi();
    /**
     * @brief seekToDay
     * @param day
     */
    void seekToDay(qint32 day);
private:
    bool m_paused{false};
    bool m_pauseRequested{false};
    spV_Scrollbar m_progressBar;
    QMutex m_replayMutex{QMutex::RecursionMode::Recursive};
    oxygine::spButton m_playButton;
    oxygine::spButton m_pauseButton;
    oxygine::spButton m_fastForwardButton;
    oxygine::spButton m_configButton;
    GameEnums::AnimationMode m_StoredShowAnimations = GameEnums::AnimationMode_All;
    spHumanPlayerInput m_HumanInput;
    spViewplayer m_Viewplayer;

    GameEnums::AnimationMode m_storedAnimMode;
    GameEnums::BattleAnimationMode m_storedBatteAnimMode;
    quint32 m_storedAnimationSpeed = 1;
    quint32 m_storedBattleAnimationSpeed = 1;

    bool m_seeking{false};
    oxygine::spActor m_seekActor;
    spLabel m_seekDayLabel;
};

#endif // REPLAYMENU_H
