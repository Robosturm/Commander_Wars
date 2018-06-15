#pragma once

#include "oxygine-framework.h"
#include <QObject>

class Mainwindow : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Mainwindow();
    virtual ~Mainwindow() = default;

    void exitMenue();
    void enterSingleplayer(oxygine::Event *);

    void leaveMenue();
};
