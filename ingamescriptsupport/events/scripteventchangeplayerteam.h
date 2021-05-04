#ifndef SCRIPTEVENTCHANGEPLAYERTEAM_H
#define SCRIPTEVENTCHANGEPLAYERTEAM_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangePlayerTeam;
using spScriptEventChangePlayerTeam = oxygine::intrusive_ptr<ScriptEventChangePlayerTeam>;

class ScriptEventChangePlayerTeam : public ScriptEventGeneric
{
    Q_OBJECT
public:
    ScriptEventChangePlayerTeam();
};

#endif // SCRIPTEVENTCHANGEPLAYERTEAM_H
