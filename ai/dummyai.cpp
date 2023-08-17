#include "ai/dummyai.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

DummyAi::DummyAi(GameMap* pMap, GameEnums::AiTypes type)
    : CoreAI (pMap, type, "DummyAi")
{
#ifdef GRAPHICSUPPORT
    setObjectName("DummyAi");
#endif
    Interpreter::setCppOwnerShip(this);
    CONSOLE_PRINT("Creating dummy ai", GameConsole::eDEBUG);
}
