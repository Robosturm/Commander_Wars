#ifndef WIKIMENU_H
#define WIKIMENU_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/panel.h"

class Wikimenu : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Wikimenu();
signals:
    void sigExitMenue();
public slots:
    void exitMenue();
};

#endif // WIKIMENU_H
