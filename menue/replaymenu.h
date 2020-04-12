#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include "menue/gamemenue.h"

class ReplayMenu : public GameMenue
{
    Q_OBJECT
public:
    ReplayMenu(QString filename);
    virtual ~ReplayMenu();
signals:
    void sigShowRecordInvalid();
    void sigExitReplay();
public slots:
    void showRecordInvalid();
    void exitReplay();
    void nextReplayAction();
    virtual void showExitGame() override;
protected:
    void loadUIButtons();
};

#endif // REPLAYMENU_H
