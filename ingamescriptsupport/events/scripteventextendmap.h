#ifndef SCRIPTEVENTEXTENDMAP_H
#define SCRIPTEVENTEXTENDMAP_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventExtendMap;
using spScriptEventExtendMap = oxygine::intrusive_ptr<ScriptEventExtendMap>;

class ScriptEventExtendMap final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventExtendMap(GameMap* pMap);
};

#endif // SCRIPTEVENTEXTENDMAP_H
