#include "resource_management/achievementmanager.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

AchievementManager::AchievementManager()
    : RessourceManagement<AchievementManager>("/images/achievements/res.xml",
                                              "/scripts/achievements")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("AchievementManager");
#endif
}

void AchievementManager::loadAll()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    RessourceManagement<AchievementManager>::loadAll();
    for (const auto & achievement : std::as_const(m_loadedRessources))
    {
        pInterpreter->doFunction(achievement, "registerAchievements");
    }
}


void AchievementManager::onVictory(qint32 team, bool humanWin, GameMap* pMap)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({team,
                       humanWin,
                       GameMap::getMapJsThis(pMap)});
    for (const auto & achievement : std::as_const(m_loadedRessources))
    {
        pInterpreter->doFunction(achievement, "onVictory", args);
    }
}
