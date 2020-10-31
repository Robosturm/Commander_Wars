#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include <QMutex>

#include "menue/gamemenue.h"
#include "objects/base/v_scrollbar.h"
#include "gameinput/humanplayerinput.h"
#include "game/viewplayer.h"
#include "game/GameEnums.h"

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
    bool _paused{false};
    bool _requestPause{false};
    spV_Scrollbar _progressBar;
    QMutex _replayMutex{QMutex::RecursionMode::Recursive};
    oxygine::spButton _playButton;
    oxygine::spButton _pauseButton;
    oxygine::spButton _fastForwardButton;
    oxygine::spButton _configButton;
    GameEnums::AnimationMode _StoredShowAnimations = GameEnums::AnimationMode_All;
    spHumanPlayerInput _HumanInput;
    spViewplayer m_Viewplayer;

    GameEnums::AnimationMode _storedAnimMode;
    GameEnums::BattleAnimationMode _storedBatteAnimMode;
    quint32 _storedAnimationSpeed = 1;
    quint32 _storedBattleAnimationSpeed = 1;

    bool _seekPause{false};
    bool _seeking{false};
    oxygine::spActor _seekActor;
    spLabel _seekDayLabel;
};

#endif // REPLAYMENU_H
