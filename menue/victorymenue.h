#ifndef VICTORYMENUE_H
#define VICTORYMENUE_H

#include <QObject>

#include "oxygine-framework.h"

class VictoryMenue : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit VictoryMenue();

signals:
    void sigExitMenue();
public slots:
    void exitMenue();
};

#endif // VICTORYMENUE_H
