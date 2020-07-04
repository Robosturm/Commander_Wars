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
    spGameMap pMap = GameMap::getInstance();
    // ignore error since we want to test if the icon exists
    oxygine::ResAnim* pAnim = pGameManager->getResAnim(icon, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        iconList.append(getIconSprite(icon));
    }
    else
    {
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        if (pUnitSpriteManager->exists(icon))
        {
            iconList.append(new Unit(icon, pMap->getCurrentPlayer(), false));
        }
        else
        {
            // check buildings?
        }
    }
}

oxygine::spSprite MenuData::getIconSprite(QString icon)
{
    oxygine::spSprite pSprite = new oxygine::Sprite();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim(icon);
    if (pAnim != nullptr)
    {
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
    }
    pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
    return pSprite;
}
