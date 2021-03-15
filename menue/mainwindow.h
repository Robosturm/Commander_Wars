#pragma once


#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include <QObject>

class Mainwindow : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Mainwindow();
    virtual ~Mainwindow();
signals:
    void sigEnterSingleplayer();
    void sigEnterMultiplayer();
    void sigEnterEditor();
    void sigEnterOptionmenue();
    void sigEnterCreditsmenue();
    void sigQuit();
    void sigEnterLoadGame();
    void sigEnterReplayGame();
    void sigLastSaveGame();
    void sigEnterWikimenue();
    void sigEnterCOStyleMenu();
    void sigEnterLoadCampaign();
    void sigEnterAchievementmenue();
    void sigImport();
    void sigOnEnter();
    void sigEnterShopMenu();
public slots:
    void enterSingleplayer();
    void enterEditor();
    void enterOptionmenue();
    void enterWikimenue();
    void enterLoadGame();
    void enterReplayGame();
    void enterCreditsmenue();
    void loadGame(QString filename);
    void replayGame(QString filename);
    void lastSaveGame();
    void enterMultiplayer();
    void changeUsername(QString name);
    void leaveMenue();
    void quitGame();
    void enterCOStyleMenu();
    void enterLoadCampaign();
    void enterShopMenu();
    void loadCampaign(QString filename);
    void enterAchievementmenue();
    void import();
    void importFromDirectory(QString file);
    void onEnter();

protected:
    void setButtonPosition(oxygine::spButton pButton, qint32 btnI);

    oxygine::spResAnim test;
};
