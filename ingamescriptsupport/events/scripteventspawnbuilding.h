#ifndef SCRIPTEVENTSPAWNBUILDING_H
#define SCRIPTEVENTSPAWNBUILDING_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventSpawnBuilding;
using spScriptEventSpawnBuilding = std::shared_ptr<ScriptEventSpawnBuilding>;

class ScriptEventSpawnBuilding final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventSpawnBuilding(GameMap* pMap);
};

#endif // SCRIPTEVENTSPAWNBUILDING_H
