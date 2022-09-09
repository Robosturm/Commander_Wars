#include "resource_management/uimanager.h"

UiManager::UiManager()
    : RessourceManagement<UiManager>("/images/ui/res.xml",
                                     "/ui",
                                     true,
                                     false)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("UiManager");
#endif
}
