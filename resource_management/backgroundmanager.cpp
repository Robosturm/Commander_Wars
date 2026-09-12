#include "resource_management/backgroundmanager.h"

BackgroundManager::BackgroundManager()
    : RessourceManagement<BackgroundManager>("/images/backgrounds/initial.xml", "")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("BackgroundManager");
#endif
}
