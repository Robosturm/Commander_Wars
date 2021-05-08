#ifndef WIKIMENU_H
#define WIKIMENU_H

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "menue/basemenu.h"

class Wikimenu;
using spWikimenu = oxygine::intrusive_ptr<Wikimenu>;

class Wikimenu : public Basemenu
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
