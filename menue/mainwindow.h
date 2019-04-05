#pragma once

#include "oxygine-framework.h"
#include <QObject>

class Mainwindow : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Mainwindow();
    virtual ~Mainwindow();


public slots:
    void enterSingleplayer();
    void enterEditor();
    void enterOptionmenue();

    void leaveMenue();
    void quitGame();
signals:
    void sigEnterSingleplayer();
    void sigEnterEditor();
    void sigEnterOptionmenue();
    void sigQuit();
protected:
    void setButtonPosition(oxygine::spButton pButton, qint32 btnI);
};
