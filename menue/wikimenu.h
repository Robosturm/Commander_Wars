#ifndef WIKIMENU_H
#define WIKIMENU_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

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
