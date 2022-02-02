#include "resource_management/achievementmanager.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"

AchievementManager::AchievementManager()
    : RessourceManagement<AchievementManager>("/images/achievements/res.xml",
                                              "/scripts/achievements")
{
    setObjectName("AchievementManager");
}

void AchievementManager::loadAll()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    RessourceManagement<AchievementManager>::loadAll();
    for (const auto & achievement : qAsConst(m_loadedRessources))
    {
        pInterpreter->doFunction(achievement, "registerAchievements");
    }
}


void AchievementManager::onVictory(qint32 team, bool humanWin, GameMap* pMap)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    args << team;
    args << humanWin;
    QJSValue value1 = pInterpreter->newQObject(pMap);
    args << value1;
    for (const auto & achievement : qAsConst(m_loadedRessources))
    {
        pInterpreter->doFunction(achievement, "onVictory", args);
    }
}
