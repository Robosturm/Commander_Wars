#include "resource_management/uimanager.h"

UiManager::UiManager()
    : RessourceManagement<UiManager>("/images/ui/res.xml",
                                     "/ui",
                                     false)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("UiManager");
#endif
}
