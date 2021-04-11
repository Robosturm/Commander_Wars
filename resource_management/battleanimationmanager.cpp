#include "resource_management/battleanimationmanager.h"

BattleAnimationManager::BattleAnimationManager()
    : RessourceManagement<BattleAnimationManager>("/images/battleanimations/res.xml",
                                                  "/scripts/battleanimations")
{
    setObjectName("BattleAnimationManager");
}
