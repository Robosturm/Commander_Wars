#include "ai/dummyai.h"

#include "coreengine/interpreter.h"

DummyAi::DummyAi(GameMap* pMap, GameEnums::AiTypes type)
    : CoreAI (pMap, type, "DummyAi")
{
#ifdef GRAPHICSUPPORT
    setObjectName("DummyAi");
#endif
    Interpreter::setCppOwnerShip(this);
    AI_CONSOLE_PRINT("Creating dummy ai", Console::eDEBUG);
}
