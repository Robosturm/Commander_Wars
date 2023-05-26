
#ifndef SCRIPTEVENTVOLCANFIRE_H
#define SCRIPTEVENTVOLCANFIRE_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventVolcanFire;
using spScriptEventVolcanFire = oxygine::intrusive_ptr<ScriptEventVolcanFire>;

class ScriptEventVolcanFire final : public ScriptEventGeneric
{
public:
    explicit ScriptEventVolcanFire(GameMap* pMap);
};

#endif // SCRIPTEVENTVOLCANFIRE_H
