#ifndef REPLAYMENU_H
#define REPLAYMENU_H

#include "menue/gamemenue.h"

class ReplayMenu : public GameMenue
{
    Q_OBJECT
public:
    ReplayMenu(QString filename);
    virtual ~ReplayMenu();
};

#endif // REPLAYMENU_H
