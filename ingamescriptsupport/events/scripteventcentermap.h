#ifndef SCRIPTEVENTCENTERMAP_H
#define SCRIPTEVENTCENTERMAP_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventCenterMap;
using spScriptEventCenterMap = oxygine::intrusive_ptr<ScriptEventCenterMap>;

class ScriptEventCenterMap final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventCenterMap(GameMap* pMap);
};

#endif // SCRIPTEVENTCENTERMAP_H
