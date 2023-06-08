#pragma once

#include <QTimer>
#include "menue/basemenu.h"

#include "game/GameEnums.h"

class Mainwindow;
using spMainwindow = oxygine::intrusive_ptr<Mainwindow>;

class Mainwindow final : public Basemenu
{
    Q_OBJECT
public:
    explicit Mainwindow(const QString & initialView);
   virtual ~Mainwindow() = default;
signals:
    void sigImport();
    void sigVersionClicked();
public slots:
    void enterSingleplayer(const QStringList & filter);
    void enterEditor();
    void enterOptionmenue(const QString & xmlFile);
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
    void restartGame();
    void enterCOStyleMenu();
    void enterLoadCampaign();
    void enterShopMenu();
    void loadCampaign(QString filename);
    void enterGeneratorMenu(const QString & generatorUi);
    void enterAchievementmenue();
    void import();
    void importFromDirectory(QString file);
    virtual void onEnter() override;
    void versionClicked();
    bool isValidSavegame();
    void createRandomInis(GameEnums::AiTypes ai, QString baseName, qint32 amount);
protected slots:
    void cheatTimeout();
    void unlockAllShopItems();
protected:
    QTimer m_cheatTimeout;
    qint32 m_cheatCounter{0};
};

Q_DECLARE_INTERFACE(Mainwindow, "Mainwindow");
