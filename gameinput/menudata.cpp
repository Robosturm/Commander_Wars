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

void MenuData::addUnitData(QString text, QString actionID, Unit* pIcon, qint32 costs, bool enabled)
{
    texts.append(text);
    actionIDs.append(actionID);
    costList.append(costs);
    enabledList.append(enabled);
    GameManager* pGameManager = GameManager::getInstance();
    spUnit menuIcon = dynamic_cast<Unit*>(pGameManager->getIcon(pIcon->getUnitID()).get());
    if (menuIcon.get() != nullptr)
    {
        menuIcon->setHasMoved(pIcon->getHasMoved());
        menuIcon->setHp(pIcon->getHp());
        menuIcon->setAmmo1(pIcon->getAmmo1());
        menuIcon->setAmmo2(pIcon->getAmmo2());
        menuIcon->setFuel(pIcon->getFuel());
        menuIcon->setUnitRank(pIcon->getUnitRank());
        iconList.append(menuIcon);
    }
    else
    {
        iconList.append(new oxygine::Sprite());
    }

}
