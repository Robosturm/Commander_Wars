#include "resource_management/gameanimationmanager.h"

GameAnimationManager::GameAnimationManager()
    : RessourceManagement<GameAnimationManager>("/images/animations/res.xml", "")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("GameAnimationManager");
#endif
}
