#ifndef SCRIPTEVENTCENTERMAP_H
#define SCRIPTEVENTCENTERMAP_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventCenterMap;
using spScriptEventCenterMap = oxygine::intrusive_ptr<ScriptEventCenterMap>;

class ScriptEventCenterMap: public ScriptEventGeneric
{
    Q_OBJECT
public:
    ScriptEventCenterMap();
};

#endif // SCRIPTEVENTCENTERMAP_H
