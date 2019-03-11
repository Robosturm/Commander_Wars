#include "coselection.h"

#include "resource_management/objectmanager.h"

COSelection::COSelection()
    : QObject()
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();

}

COSelection::~COSelection()
{

}

void COSelection::colorChanged(QColor color)
{

}
