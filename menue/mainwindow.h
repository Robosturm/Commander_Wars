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

    void leaveMenue();

signals:
    void sigEnterSingleplayer();
};
