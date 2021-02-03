#ifndef SHOPMENU_H
#define SHOPMENU_H

#include <QObject>

#include "oxygine-framework.h"

class Shopmenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Shopmenu();

signals:
    void sigExitMenue();

public slots:
    void exitMenue();

};

#endif // SHOPMENU_H
