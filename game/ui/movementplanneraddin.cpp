#include "game/ui/movementplanneraddin.h"

#include "coreengine/interpreter.h"

MovementPlannerAddIn::MovementPlannerAddIn()
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setObjectName("MovementPlannerAddIn");
    Interpreter::setCppOwnerShip(this);
}

MovementPlannerAddIn::MovementPlannerAddIn(QString addIn)
    : m_addIn(addIn)
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    setObjectName("MovementPlannerAddIn");
    Interpreter::setCppOwnerShip(this);
}

QString MovementPlannerAddIn::getAddIn() const
{
    return m_addIn;
}
