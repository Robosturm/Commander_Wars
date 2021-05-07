#include "resource_management/gamemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

#include "game/gamemap.h"
#include "game/unit.h"

GameManager::GameManager()
    : RessourceManagement<GameManager>("/images/game/res.xml",
                                       "")
{
    setObjectName("GameManager");
}

void GameManager::reset()
{
    RessourceManagement<GameManager>::reset(m_loadedRessources);
    RessourceManagement<GameManager>::reset(m_loadedHeavyAis);
}

void GameManager::loadAll()
{
    reset();
    m_scriptPath = "scripts/actions/";
    RessourceManagement<GameManager>::loadAll(m_loadedRessources);
    m_scriptPath = "aidata/heavy/";
    RessourceManagement<GameManager>::loadAll(m_loadedHeavyAis);
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
        BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
        if (pUnitSpriteManager->exists(icon))
        {
            spGameMap pMap = GameMap::getInstance();
            spPlayer pPlayer;
            if (pMap.get() == nullptr ||
                pMap->getCurrentPlayer() == nullptr ||
                pMap->getCurrentPlayer()->getColorTableAnim().get() == nullptr)
            {
                pPlayer = spPlayer::create();
                pPlayer->init();
            }
            else
            {
                pPlayer = pMap->getCurrentPlayer();
            }
            oxygine::spSprite ret = spUnit::create(icon, pPlayer.get(), false);
            return ret;
        }
        else if (pBuildingSpriteManager->exists(icon))
        {
            // check buildings?
            spGameMap pMap = GameMap::getInstance();
            spPlayer pPlayer = nullptr;
            if (pMap.get() != nullptr)
            {
                pPlayer = pMap->getCurrentPlayer();
            }
            spBuilding pBuilding = spBuilding::create(icon);
            pBuilding->setOwner(pPlayer.get());
            pBuilding->scaleAndShowOnSingleTile();
            return pBuilding;
        }
    }
    return nullptr;
}

oxygine::spSprite GameManager::getIconSprite(QString icon)
{
    oxygine::spSprite pSprite = oxygine::spSprite::create();
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
        pSprite->setScale(GameMap::getImageSize() / pAnim->getWidth());
    }
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
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
