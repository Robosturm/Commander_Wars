#include "objects/tableView/complextableview.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

ComplexTableView::ComplexTableView(QObject *parent)
    : QObject{parent}
{
    Interpreter::setCppOwnerShip(this);
    setObjectName("ComplexTableView");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

}

void ComplexTableView::setItems(const Items & items)
{
    m_items = items;
}
