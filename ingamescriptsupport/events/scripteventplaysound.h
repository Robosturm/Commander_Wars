#ifndef SCRIPTEVENTPLAYSOUND_H
#define SCRIPTEVENTPLAYSOUND_H

#include "ingamescriptsupport/events/scripteventgeneric.h"

class ScriptEventPlaySound;
using spScriptEventPlaySound = std::shared_ptr<ScriptEventPlaySound>;

class ScriptEventPlaySound final : public ScriptEventGeneric
{
    Q_OBJECT
public:
    explicit ScriptEventPlaySound(GameMap* pMap);
};

#endif // SCRIPTEVENTPLAYSOUND_H
