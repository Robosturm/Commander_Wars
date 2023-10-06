
#include "coreengine/qmlvector.h"
#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#include "ai/heavyai/heavyai.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"

HeavyAi::HeavyAi(GameMap* pMap, QString type, GameEnums::AiTypes aiType)
    : CoreAI(pMap, aiType, type)
{
#ifdef GRAPHICSUPPORT
    setObjectName("HeavyAi");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);

    if (m_pMap != nullptr &&
        !m_pMap->getSavegame())
    {
        loadIni("heavy/" + m_aiName.toLower() + ".ini");
    }
    CONSOLE_PRINT("Creating heavy ai", GameConsole::eDEBUG);
}

void HeavyAi::process()
{

}
