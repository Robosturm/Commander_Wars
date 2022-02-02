#pragma once

#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"

class Mainwindow;
using spMainwindow = oxygine::intrusive_ptr<Mainwindow>;

class Mainwindow : public Basemenu
{
    Q_OBJECT
public:
    explicit Mainwindow();
    virtual ~Mainwindow() = default;
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
    void sigEnterShopMenu();
    void sigVersionClicked();
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
    virtual void onEnter() override;
    void versionClicked();
    qint32 getButtonX(qint32 btnI) const;
    qint32 getButtonY(qint32 btnI) const;
    bool isValidSavegame() const;
protected slots:
    void cheatTimeout();
    void unlockAllShopItems();
protected:
    oxygine::spResAnim test;
    QTimer m_cheatTimeout;
    qint32 m_cheatCounter{0};
};
