#include "resource_management/uimanager.h"

UiManager::UiManager()
    : RessourceManagement<UiManager>("/images/ui/res.xml",
                                     "/ui")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("UiManager");
#endif
}
