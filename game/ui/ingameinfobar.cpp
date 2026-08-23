#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "game/ui/ingameinfobar.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gameanimation/battleanimationsprite.h"

#include "menue/gamemenue.h"

static const char *const FONT = "infoBar16";

IngameInfoBar::IngameInfoBar(GameMenue *pMenu, GameMap *pMap)
    : m_pMap(pMap),
      m_pMenu(pMenu)
{
    Mainapp::getInstance()->pauseRendering();
#ifdef GRAPHICSUPPORT
    setObjectName("IngameInfoBar");
#endif
    CONSOLE_PRINT("Creating IngameInfoBar", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
    Interpreter * pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                        JsThis::getJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction("IngameInfoBar", "preInitValues", args);

    UiFactory::getInstance().createUi("ui/game/ingameInfoBar.xml", this);
    m_pMinimap = dynamic_cast<Minimap*>(getObject("MINIMAP"));
    if (m_pMinimap)
    {
        m_pMinimap->setMenu(pMenu);
    }
    updateMinimap();
    m_pMinimapSlider = dynamic_cast<oxygine::SlidingActor*>(getObject("MINIMAP_BOX"));
    if (m_pMinimapSlider)
    {
        m_pMinimapSlider->updateDragBounds();
    }
    auto scale = pInterpreter->doFunction("IngameInfoBar", "getScale", args);
    if (scale.isNumber())
    {
        setScale(scale.toNumber());
    }
    pInterpreter->doFunction("IngameInfoBar", "postInitValues", args);


    m_pDetailedViewBox = MemoryManagement::create<oxygine::Box9Sprite>();
    auto* pAnim = ObjectManager::getInstance()->getResAnim("panel_transparent+mask");
    m_pDetailedViewBox->setResAnim(pAnim);
    if (getScaleX() < 0.75f)
    {
        m_pDetailedViewBox->setScale(1.0f / getScaleX() * 0.5f);
    }
    else if (getScaleX() < 1.0f)
    {
        m_pDetailedViewBox->setScale(1.0f / getScaleX());
    }
    m_pDetailedViewBox->setSize(130, 199);
    m_pDetailedViewBox->setPosition(-(m_pDetailedViewBox->getScaledWidth() - m_pDetailedViewBox->getWidth()),
                                    getHeight() - m_pDetailedViewBox->getScaledHeight());
    addChild(m_pDetailedViewBox);
    setX(oxygine::Stage::getStage()->getWidth() - getScaledWidth());
    connect(&m_debounceTimer, &QTimer::timeout, this, &IngameInfoBar::updateTerrainInfoInternal, Qt::QueuedConnection);
    updateTerrainInfo(0, 0, false);
    Mainapp::getInstance()->continueRendering();
}

void IngameInfoBar::updatePlayerInfo()
{
    Mainapp::getInstance()->pauseRendering();
    Interpreter * pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap)});
    pInterpreter->doFunction("IngameInfoBar", "updatePlayerInfo", args);
    Mainapp::getInstance()->continueRendering();
}

void IngameInfoBar::updateMinimap()
{
    if (m_pMinimap)
    {
        m_pMinimap->updateMinimap(m_pMap, true);
    }
}

void IngameInfoBar::updateCursorInfo(qint32 x, qint32 y)
{
    if (m_pMenu->getCursor()->getMapPointX() == x &&
        m_pMenu->getCursor()->getMapPointY() == y)
    {
        updateTerrainInfo(x, y, false);
    }
}

void IngameInfoBar::updateTerrainInfo(qint32 x, qint32 y, bool update)
{
    if (m_pMap != nullptr && m_pMap->onMap(x, y))
    {
        if (m_LastX != x || m_LastY != y || update)
        {
            m_newX = x;
            m_newY = y;
            if (!m_pMap->onMap(m_LastX, m_LastY))
            {
                updateTerrainInfoInternal();
            }
            else
            {
                m_debounceTimer.setSingleShot(true);
                m_debounceTimer.start(250);
            }
        }
        else
        {
            // cursor returned to the shown tile, drop the pending stale update
            m_debounceTimer.stop();
        }
    }
}

void IngameInfoBar::updateTerrainInfoInternal()
{
    Mainapp::getInstance()->pauseRendering();
    m_LastX = m_newX;
    m_LastY = m_newY;
    Interpreter * pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_pMap),
                       m_LastX,
                       m_LastY});
    auto erg = pInterpreter->doFunction("IngameInfoBar", "updateTerrainInfo", args);
    if (erg.isBool() && erg.toBool())
    {
        auto* pCurrentPlayer = m_pMap->getCurrentPlayer();
        if (pCurrentPlayer != nullptr)
        {
            m_pDetailedViewBox->setColorTable(pCurrentPlayer->getColorTableAnim(), true);
        }
        updateDetailedView(m_LastX, m_LastY);
    }
    Mainapp::getInstance()->continueRendering();
}

void IngameInfoBar::updateDetailedView(qint32 x, qint32 y)
{
    static constexpr qint32 xOffset = 2;
    static constexpr qint32 yOffset = 4;
    bool hpHidden = false;
    Terrain *pTerrain = m_pMap->getTerrain(x, y);
    Building *pBuilding = pTerrain->getBuilding();
    Unit *pUnit = pTerrain->getUnit();
    Player *pPlayer = m_pMenu->getCurrentViewPlayer();
    if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    if (pUnit != nullptr)
    {
        hpHidden = pUnit->getHpHidden(m_pMenu->getCurrentViewPlayer());
    }
    GameManager *pGameManager = GameManager::getInstance();
    m_pDetailedViewBox->removeChildren();

    oxygine::ResAnim *pAnimBase = nullptr;
    oxygine::ResAnim *pAnimFore = nullptr;
    oxygine::ResAnim *pAnimBack = nullptr;
    float speed = 0;
    if (pUnit != nullptr)
    {
        pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase(), oxygine::ep_ignore_error);
        pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
        pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
        speed = pUnit->getTerrainAnimationMoveSpeed();
    }
    else if (pBuilding != nullptr)
    {
        pAnimBase = pGameManager->getResAnim(pBuilding->getTerrainAnimationBase(), oxygine::ep_ignore_error);
        pAnimFore = pGameManager->getResAnim(pBuilding->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
        pAnimBack = pGameManager->getResAnim(pBuilding->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
        speed = pBuilding->getTerrainAnimationMoveSpeed();
    }
    else
    {
        pAnimBase = pGameManager->getResAnim(pTerrain->getTerrainAnimationBase(), oxygine::ep_ignore_error);
        pAnimFore = pGameManager->getResAnim(pTerrain->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
        pAnimBack = pGameManager->getResAnim(pTerrain->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
        speed = pTerrain->getTerrainAnimationMoveSpeed();
    }
    oxygine::spSlidingSprite pTerrainSprite = MemoryManagement::create<oxygine::SlidingSprite>();
    pTerrainSprite->setPosition(xOffset, yOffset);
    pTerrainSprite->setResAnim(pAnimBase);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeedX(speed);
    m_pDetailedViewBox->addChild(pTerrainSprite);
    pTerrainSprite = MemoryManagement::create<oxygine::SlidingSprite>();
    pTerrainSprite->setPosition(xOffset, yOffset);
    pTerrainSprite->setResAnim(pAnimBack);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeedX(speed);
    m_pDetailedViewBox->addChild(pTerrainSprite);
    pTerrainSprite = MemoryManagement::create<oxygine::SlidingSprite>();
    pTerrainSprite->setPosition(xOffset, yOffset);
    pTerrainSprite->setResAnim(pAnimFore);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeedX(speed);
    m_pDetailedViewBox->addChild(pTerrainSprite);

    oxygine::ResAnim *pAnimWeather = pGameManager->getResAnim(pTerrain->getWeatherOverlayId(), oxygine::ep_ignore_error);
    if (pAnimWeather != nullptr)
    {
        oxygine::spSlidingSprite pWeatherOverlay = MemoryManagement::create<oxygine::SlidingSprite>();
        pWeatherOverlay->setPosition(xOffset, yOffset);
        pWeatherOverlay->setResAnim(pAnimWeather);
        pWeatherOverlay->setSize(spriteWidth, spriteHeigth);
        QPoint speed = pTerrain->getWeatherOverlaySpeed();
        pWeatherOverlay->setSpeedX(speed.x());
        pWeatherOverlay->setSpeedY(speed.y());
        pWeatherOverlay->setPriority(100000);
        pWeatherOverlay->setLocked(true);
        m_pDetailedViewBox->addChild(pWeatherOverlay);
    }

    if (pUnit != nullptr)
    {
        qint32 hp = -1;
        if (hpHidden)
        {
            hp = Unit::MAX_UNIT_HP;
        }
        spBattleAnimationSprite pBattleAnimationSprite = MemoryManagement::create<BattleAnimationSprite>(m_pMap, pUnit, pUnit->getTerrain(), BattleAnimationSprite::standingAnimation, hp, false);
        pBattleAnimationSprite->setPosition(xOffset, yOffset);
        pBattleAnimationSprite->setPriority(3);
        m_pDetailedViewBox->addChild(pBattleAnimationSprite);
    }
    qint32 terrainDefense = 0;
    if (pUnit != nullptr)
    {
        terrainDefense = pUnit->getTerrainDefense();
    }
    else
    {
        terrainDefense = pTerrain->getBaseDefense();
    }
    float defenseY = pTerrainSprite->getY() + 5;
    float startDefenseX = pTerrainSprite->getX() + 5;
    float defenseX = startDefenseX;
    oxygine::ResAnim *pAnim = pGameManager->getResAnim("defenseStar");
    for (qint32 i = 1; i <= terrainDefense; i++)
    {
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setPosition(defenseX, defenseY);
        pSprite->setResAnim(pAnim);
        pSprite->setScale(1.5f);
        pSprite->setPriority(4);
        m_pDetailedViewBox->addChild(pSprite);
        if (pAnim != nullptr)
        {
            if (i % 4 == 0)
            {
                defenseY += pAnim->getHeight() + 2;
                defenseX = startDefenseX;
            }
            else
            {
                defenseX += pAnim->getWidth() + 2;
            }
        }
    }
}

void IngameInfoBar::syncMinimapPosition()
{
    if (m_pMap != nullptr &&
        m_pMinimapSlider != nullptr &&
        m_pMinimap != nullptr)
    {
        QPoint centeredPos = m_pMap->getCenteredPosition();
        auto bounds = m_pMinimapSlider->getDragBounds();
        auto size = m_pMinimapSlider->getSize();
        qint32 newX = size.width() / 2 - centeredPos.x() * Minimap::IMAGE_SIZE * m_pMinimap->getScaleX();
        qint32 newY = size.height() / 2 - centeredPos.y() * Minimap::IMAGE_SIZE * m_pMinimap->getScaleY();
        if (newX < bounds.left())
        {
            newX = bounds.left();
        }
        else if (newX > bounds.right())
        {
            newX = bounds.right();
        }
        if (newY < bounds.top())
        {
            newY = bounds.top();
        }
        else if (newY > bounds.bottom())
        {
            newY = bounds.bottom();
        }
        m_pMinimap->setPosition(newX, newY);
    }
}

GameMap *IngameInfoBar::getMap() const
{
    return m_pMap;
}

GameMenue* IngameInfoBar::getGameMenu()
{
    return m_pMenu;
}

oxygine::spBox9Sprite IngameInfoBar::getDetailedViewBox() const
{
    return m_pDetailedViewBox;
}
