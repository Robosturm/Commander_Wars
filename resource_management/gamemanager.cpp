#include "resource_management/gamemanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/buildingspritemanager.h"

#include "game/gamemap.h"
#include "game/unit.h"

GameManager::GameManager()
    : RessourceManagement<GameManager>("/images/game/res.xml",
                                       "")
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("GameManager");
#endif
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
    m_raiseErrors = false;
    m_scriptPath = "aidata/normal/";
    RessourceManagement<GameManager>::loadAll(m_loadedNormalAis);
    m_scriptPath = "aidata/heavy/";    
    RessourceManagement<GameManager>::loadAll(m_loadedHeavyAis);
    m_scriptPath = "aidata/very_easy/";
    RessourceManagement<GameManager>::loadAll(m_loadedVeryEasyAis);
    m_raiseErrors = true;
}

QString GameManager::getActionIcon(const QString & actionID)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    QJSValue ret = pInterpreter->doFunction(actionID, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

oxygine::spSprite GameManager::getIcon(GameMap* pMap, const QString & icon)
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
        COPerkManager* pCOPerkManager = COPerkManager::getInstance();
        if (pUnitSpriteManager->exists(icon))
        {
            
            spPlayer pPlayer;
            if (pMap == nullptr ||
                pMap->getCurrentPlayer() == nullptr ||
                pMap->getCurrentPlayer()->getColorTableAnim().get() == nullptr)
            {
                pPlayer = MemoryManagement::create<Player>(pMap);
                pPlayer->init();
            }
            else
            {
                pPlayer = pMap->getSpCurrentPlayer();
            }
            oxygine::spSprite ret = MemoryManagement::create<Unit>(icon, pPlayer.get(), false, pMap);
            return ret;
        }
        else if (pBuildingSpriteManager->exists(icon))
        {
            // check buildings?
            
            spPlayer pPlayer;
            if (pMap != nullptr)
            {
                pPlayer = pMap->getSpCurrentPlayer();
            }
            spBuilding pBuilding = MemoryManagement::create<Building>(icon, pMap);
            pBuilding->setOwner(pPlayer.get());
            pBuilding->scaleAndShowOnSingleTile();
            return pBuilding;
        }
        else
        {
            oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
            oxygine::ResAnim* pAnim = pCOPerkManager->getResAnim(icon);
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
                pSprite->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
            }
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
            return pSprite;
        }
    }
    return oxygine::spSprite();
}

oxygine::spSprite GameManager::getIconSprite(const QString & icon)
{
    oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
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
        pSprite->setScale(static_cast<float>(GameMap::getImageSize()) / static_cast<float>(pAnim->getWidth()));
    }
    pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
    return pSprite;
}

QString GameManager::getDescription(qint32 position)
{
    if ((position >= 0) && (position < m_loadedRessources.size()))
    {
        QString name = m_loadedRessources[position];
        QJSValueList args({name});
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(name, "getDescription", args);
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

QStringList GameManager::getDefaultActionbBannlist()
{
    QStringList ret;
    for (const auto & action : std::as_const(m_loadedRessources))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(action, "getBannedByDefault");
        if (value.isNull() || !value.toBool())
        {
            ret.append(action);
        }
    }
    return ret;
}
