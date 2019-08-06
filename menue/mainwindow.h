#pragma once

#include "oxygine-framework.h"
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
    void sigLastSaveGame();
    void sigEnterWikimenue();
public slots:
    void enterSingleplayer();
    void enterEditor();
    void enterOptionmenue();
    void enterWikimenue();
    void enterLoadGame();
    void enterCreditsmenue();
    void loadGame(QString filename);
    void lastSaveGame();
    void enterMultiplayer();
    void changeUsername(QString name);
    void leaveMenue();
    void quitGame();
protected:
    void setButtonPosition(oxygine::spButton pButton, qint32 btnI);
};
