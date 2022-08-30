#include "objects/tableView/basetableitem.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

BaseTableItem::BaseTableItem(QObject *parent)
    : QObject{parent}
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("BaseTableItem");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
}
