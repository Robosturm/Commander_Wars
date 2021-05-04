#ifndef SCRIPTEVENTSPAWNBUILDING_H
#define SCRIPTEVENTSPAWNBUILDING_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventSpawnBuilding;
using spScriptEventSpawnBuilding = oxygine::intrusive_ptr<ScriptEventSpawnBuilding>;

class ScriptEventSpawnBuilding : public ScriptEventGeneric
{
    Q_OBJECT
public:
    ScriptEventSpawnBuilding();
};

#endif // SCRIPTEVENTSPAWNBUILDING_H
