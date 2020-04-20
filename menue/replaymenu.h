#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include <QMutex>

#include "menue/gamemenue.h"
#include "objects/v_scrollbar.h"
#include "gameinput/humanplayerinput.h"
#include "game/viewplayer.h"

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
protected:
    void loadUIButtons();
private:
    bool _paused{false};
    bool requestPause{false};
    spV_Scrollbar _progressBar;
    QMutex _replayMutex;
    oxygine::spButton _playButton;
    oxygine::spButton _pauseButton;
    oxygine::spButton _fastForwardButton;
    oxygine::spButton _configButton;
    GameEnums::AnimationMode _StoredShowAnimations = GameEnums::AnimationMode_All;
    spHumanPlayerInput _HumanInput;
    Viewplayer _Viewplayer;
};

#endif // REPLAYMENU_H
