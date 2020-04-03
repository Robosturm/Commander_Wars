#include "buildingspritemanager.h"

BuildingSpriteManager::BuildingSpriteManager()
    : RessourceManagement<BuildingSpriteManager>("/images/building/res.xml",
                                                  "/scripts/building")
{
}
