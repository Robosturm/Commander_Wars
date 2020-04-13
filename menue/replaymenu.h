#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include <QMutex>

#include "menue/gamemenue.h"
#include "objects/v_scrollbar.h"
#include "gameinput/humanplayerinput.h"

class ReplayMenu : public GameMenue
{
    Q_OBJECT
    static constexpr qint32 actionPixelSize = 5;
public:
    ReplayMenu(QString filename);
    virtual ~ReplayMenu();
signals:
    void sigShowRecordInvalid();
    void sigExitReplay();
    void sigSwapPlay();
    void sigStartFastForward();
    void sigStopFastForward();
public slots:
    void showRecordInvalid();
    void exitReplay();
    void nextReplayAction();
    virtual void showExitGame() override;
    void swapPlay();
    void startFastForward();
    void stopFastForward();
protected:
    void loadUIButtons();
private:
    bool _paused{false};
    bool requestPause{false};
    spV_Scrollbar _progressBar;
    QMutex _replayMutex;
    oxygine::spButton _playButton;
    oxygine::spButton _pauseButton;
    GameEnums::AnimationMode _StoredShowAnimations = GameEnums::AnimationMode_All;
    spHumanPlayerInput _HumanInput;
};

#endif // REPLAYMENU_H
