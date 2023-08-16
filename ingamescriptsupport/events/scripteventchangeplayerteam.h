#ifndef SCRIPTEVENTCHANGEPLAYERTEAM_H
#define SCRIPTEVENTCHANGEPLAYERTEAM_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventChangePlayerTeam;
using spScriptEventChangePlayerTeam = std::shared_ptr<ScriptEventChangePlayerTeam>;

class ScriptEventChangePlayerTeam final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventChangePlayerTeam(GameMap* pMap);
};

#endif // SCRIPTEVENTCHANGEPLAYERTEAM_H
