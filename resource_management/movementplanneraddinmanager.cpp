#include "resource_management/movementplanneraddinmanager.h"

MovementPlannerAddInManager::MovementPlannerAddInManager()
    : RessourceManagement<MovementPlannerAddInManager>("/images/movementPlannerAddIns/res.xml",
                                              "/scripts/movementPlannerAddIns")
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("MovementPlannerAddInManager");
}
