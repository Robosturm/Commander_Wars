#include "menudata.h"


#include "resource_management/gamemanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/unitspritemanager.h"

#include "game/gamemap.h"

#include "game/unit.h"

#include "game/player.h"
#include "game/co.h"

MenuData::MenuData()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void MenuData::addData(QString text, QString actionID, QString icon, qint32 costs, bool enabled)
{
    texts.append(text);
    actionIDs.append(actionID);
    costList.append(costs);
    enabledList.append(enabled);
    GameManager* pGameManager = GameManager::getInstance();
    iconList.append(pGameManager->getIcon(icon));
}
