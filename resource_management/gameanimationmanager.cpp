#include "resource_management/gameanimationmanager.h"

GameAnimationManager::GameAnimationManager()
    : RessourceManagement<GameAnimationManager>("/images/animations/res.xml", "")
{
    setObjectName("GameAnimationManager");
}
