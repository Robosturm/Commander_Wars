#include "resource_management/buildingspritemanager.h"

BuildingSpriteManager::BuildingSpriteManager()
    : RessourceManagement<BuildingSpriteManager>("/images/building/res.xml",
                                                  "/scripts/building")
{
    setObjectName("BuildingSpriteManager");
}
