
#ifndef SCRIPTEVENTBUILDINGFIRECOUNTER_H
#define SCRIPTEVENTBUILDINGFIRECOUNTER_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventBuildingFireCounter;
using spScriptEventBuildingFireCounter = oxygine::intrusive_ptr<ScriptEventBuildingFireCounter>;

class ScriptEventBuildingFireCounter final : public ScriptEventGeneric
{
public:
    explicit ScriptEventBuildingFireCounter(GameMap* pMap);
};

#endif // SCRIPTEVENTBUILDINGFIRECOUNTER_H
