#include "resource_management/backgroundmanager.h"

BackgroundManager::BackgroundManager()
    : RessourceManagement<BackgroundManager>("/images/backgrounds/res.xml", "")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("BackgroundManager");
#endif
}
