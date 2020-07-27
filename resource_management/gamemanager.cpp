#include "gamemanager.h"

#include "resource_management/unitspritemanager.h"
#include "game/gamemap.h"
#include "game/unit.h"

GameManager::GameManager()
    : RessourceManagement<GameManager>("/images/game/res.xml",
                                       "scripts/actions")
{
}

QString GameManager::getActionIcon(QString actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    QJSValueList args1;
    QJSValue ret = pInterpreter->doFunction(actionID, function1, args1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

oxygine::spSprite GameManager::getIcon(QString icon)
{
    oxygine::ResAnim* pAnim = getResAnim(icon, oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        return getIconSprite(icon);
    }
    else
    {
        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        if (pUnitSpriteManager->exists(icon))
        {
            spGameMap pMap = GameMap::getInstance();
            spPlayer pPlayer;
            if (pMap.get() == nullptr)
            {
                pPlayer = new Player();
                pPlayer->init();
            }
            else
            {
                pPlayer = pMap->getCurrentPlayer();
            }
            return new Unit(icon, pPlayer.get(), false);
        }
        else
        {
            // check buildings?
        }
    }
    return nullptr;
}

oxygine::spSprite GameManager::getIconSprite(QString icon)
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

QString GameManager::getDescription(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        QString name = m_loadedRessources[position];
        QJSValueList args;
        args << name;
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(name, "getDescription", args);
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}
