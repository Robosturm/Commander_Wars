#include "3rd_party/oxygine-framework/oxygine/actor/slidingsprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "game/ui/ingameinfobar.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
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

#include "objects/base/label.h"

static const char* const FONT = "infoBar16";

IngameInfoBar::IngameInfoBar(GameMenue* pMenu, GameMap* pMap)
    : m_pMap(pMap),
      m_pMenu(pMenu)
{
#ifdef GRAPHICSUPPORT
    setObjectName("IngameInfoBar");
#endif
    CONSOLE_PRINT("Creating IngameInfoBar", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);

    qint32 width = 300;
    qint32 cursorInfoHeigth = 330;
    qint32 gameInfoHeigth = 290;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pMiniMapBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pMiniMapBox->setResAnim(pAnim);

    pMiniMapBox->setSize(width, oxygine::Stage::getStage()->getHeight() - cursorInfoHeigth - gameInfoHeigth);
    pMiniMapBox->setPosition(0, 0);
    if (oxygine::Stage::getStage()->getHeight() - cursorInfoHeigth - gameInfoHeigth < 50)
    {
        pMiniMapBox->setHeight(100);
        setScale(static_cast<float>(oxygine::Stage::getStage()->getHeight()) / static_cast<float>(100.0f + cursorInfoHeigth + gameInfoHeigth));
    }
    setSize(width, oxygine::Stage::getStage()->getHeight());


    pMiniMapBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pMinimap = MemoryManagement::create<Minimap>();
    m_pMinimap->setMenu(pMenu);
    m_pMinimap->setPosition(0, 0);
    updateMinimap();
    if (getScaleY() < 1.0f)
    {
        m_pMinimap->setScale(1.0f / getScaleY());
    }
    else
    {
        m_pMinimap->setScale(2.0f);
    }
    m_pMinimapSlider = MemoryManagement::create<oxygine::SlidingActor>();
    m_pMinimapSlider->setPosition(10, 10);
    m_pMinimapSlider->setSize(pMiniMapBox->getScaledWidth() - 20,
                              pMiniMapBox->getScaledHeight() - 20);
    m_pMinimapSlider->setContent(m_pMinimap);
    pMiniMapBox->addChild(m_pMinimapSlider);
    addChild(pMiniMapBox);

    m_pGameInfoBox = MemoryManagement::create<oxygine::Box9Sprite>();
    m_pGameInfoBox->setResAnim(pAnim);
    m_pGameInfoBox->setPosition(0, pMiniMapBox->getScaledHeight());
    m_pGameInfoBox->setSize(width, gameInfoHeigth);
    m_pGameInfoBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(m_pGameInfoBox);

    m_pCursorInfoBox = MemoryManagement::create<oxygine::Box9Sprite>();
    m_pCursorInfoBox->setResAnim(pAnim);
    m_pCursorInfoBox->setPosition(0, pMiniMapBox->getScaledHeight() + gameInfoHeigth);
    m_pCursorInfoBox->setSize(width, cursorInfoHeigth);
    m_pCursorInfoBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    m_pDetailedViewBox = MemoryManagement::create<oxygine::Box9Sprite>();
    pAnim = pObjectManager->getResAnim("panel_transparent+mask");
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
    m_pDetailedViewBox->setPosition(-m_pDetailedViewBox->getScaledWidth(),
                                    m_pCursorInfoBox->getScaledHeight() - m_pDetailedViewBox->getScaledHeight());

    setX(oxygine::Stage::getStage()->getWidth() - getScaledWidth());
    addChild(m_pCursorInfoBox);
}

void IngameInfoBar::updatePlayerInfo()
{
    Mainapp::getInstance()->pauseRendering();
    m_pGameInfoBox->removeChildren();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    
    if (m_pMap != nullptr &&
        m_pMap->getCurrentPlayer() != nullptr)
    {
        m_pDetailedViewBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
        if (m_pMenu != nullptr)
        {
            Player* pPlayer = m_pMap->getCurrentPlayer();
            if (pPlayer != nullptr)
            {
                qint32 y = 12;
                qint32 x1 = 10;
                qint32 x2 = 110;
                spLabel pName = MemoryManagement::create<Label>(260);
                auto style = pName->getStyle();
                style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
                pName->setStyle(style);
                pName->setPosition(x1, y - 5);
                pName->setHtmlText(pPlayer->getPlayerNameId());
                m_pGameInfoBox->addChild(pName);
                y += pName->getScaledHeight();

                oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
                const CO* pCO = pPlayer->getCO(0);
                oxygine::ResAnim* pAnim = nullptr;
                if (pCO != nullptr)
                {
                    pAnim = pCO->getResAnim((pCO->getCoID() + "+face"));
                }
                else
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+face");
                }
                if (pAnim != nullptr)
                {
                    pSprite->setScale(87.0f / static_cast<float>(pAnim->getHeight()));
                }
                pSprite->setResAnim(pAnim);
                pSprite->setPosition(12, y);
                m_pGameInfoBox->addChild(pSprite);
                pSprite = MemoryManagement::create<oxygine::Sprite>();
                pCO = pPlayer->getCO(1);
                if (pCO != nullptr)
                {
                    pAnim = pCO->getResAnim((pCO->getCoID() + "+face"));
                }
                else
                {
                    pAnim = pCOSpriteManager->getResAnim("no_co+face");
                }
                pSprite->setResAnim(pAnim);
                pSprite->setPosition(103, y);
                if (pAnim != nullptr)
                {
                    pSprite->setScale(87.0f / static_cast<float>(pAnim->getHeight()));
                }
                m_pGameInfoBox->addChild(pSprite);

                pSprite = MemoryManagement::create<oxygine::Sprite>();
                pAnim = pGameManager->getResAnim((m_pMap->getGameRules()->getCurrentWeather()->getWeatherSymbol()));
                if (pAnim != nullptr)
                {
                    pSprite->setResAnim(pAnim);
                    pSprite->setPosition(202, y);
                    pSprite->setScale(85.0f / static_cast<float>(pAnim->getWidth()));
                }
                m_pGameInfoBox->addChild(pSprite);

                y -= 4;
                // boxes for co's and weather
                ObjectManager* pObjectManager = ObjectManager::getInstance();
                pAnim = pObjectManager->getResAnim("panel_transparent+mask");
                oxygine::spBox9Sprite pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                pBox->setResAnim(pAnim);
                pBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
                pBox->setSize(91, 95);
                pBox->setPosition(101, y);
                m_pGameInfoBox->addChild(pBox);
                pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                pBox->setResAnim(pAnim);
                pBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
                pBox->setSize(91, 95);
                pBox->setPosition(10, y);
                m_pGameInfoBox->addChild(pBox);
                // weather box
                pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                pBox->setResAnim(pAnim);
                pBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
                pBox->setSize(91, 95);
                pBox->setPosition(198, y);
                m_pGameInfoBox->addChild(pBox);
                y += 100;

                if (m_pMap->getGameRules()->getWeatherPrediction())
                {
                    pSprite = MemoryManagement::create<oxygine::Sprite>();
                    Weather* pWeather = m_pMap->getGameRules()->getWeatherAtDay(1, m_pMap->getCurrentPlayer()->getPlayerID());
                    if (pWeather != nullptr)
                    {
                        pAnim = pGameManager->getResAnim((pWeather->getWeatherSymbol()));
                        if (pAnim != nullptr)
                        {
                            pSprite->setResAnim(pAnim);
                            pSprite->setPosition(202, y);
                            pSprite->setScale(37.0f / static_cast<float>(pAnim->getWidth()));
                        }
                    }
                    m_pGameInfoBox->addChild(pSprite);
                    pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                    pAnim = pObjectManager->getResAnim("panel_transparent+mask");
                    pBox->setResAnim(pAnim);
                    pBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
                    pBox->setSize(45, 47);
                    pBox->setPosition(198, y - 4);
                    m_pGameInfoBox->addChild(pBox);

                    pSprite = MemoryManagement::create<oxygine::Sprite>();
                    pWeather = m_pMap->getGameRules()->getWeatherAtDay(2, m_pMap->getCurrentPlayer()->getPlayerID());
                    if (pWeather != nullptr)
                    {
                        pAnim = pGameManager->getResAnim((pWeather->getWeatherSymbol()));
                        if (pAnim != nullptr)
                        {
                            pSprite->setResAnim(pAnim);
                            pSprite->setPosition(247, y);
                            pSprite->setScale(37.0f / static_cast<float>(pAnim->getWidth()));
                        }
                    }
                    m_pGameInfoBox->addChild(pSprite);
                    pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                    pAnim = pObjectManager->getResAnim("panel_transparent+mask");
                    pBox->setResAnim(pAnim);
                    pBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
                    pBox->setSize(45, 47);
                    pBox->setPosition(243, y - 4);
                    m_pGameInfoBox->addChild(pBox);
                }

                style = oxygine::TextStyle(FontManager::getFont(FONT));
                style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
                style.multiline = false;

                qint32 count = pPlayer->getBuildingCount();
                qint32 width = 95;
                y += 2;

                spLabel pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(tr("Day:"));
                pTextfield->setPosition(x1, y);
                m_pGameInfoBox->addChild(pTextfield);

                constexpr qint32 yAdvance = 20;

                pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(QString::number(m_pMap->getCurrentDay()));
                pTextfield->setPosition(x2, y);
                m_pGameInfoBox->addChild(pTextfield);
                y += yAdvance;

                const Player* pViewPlayer = m_pMenu->getCurrentViewPlayer();
                if (pViewPlayer != nullptr)
                {
                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    pTextfield->setHtmlText(tr("Buildings:"));
                    pTextfield->setPosition(x1, y);
                    m_pGameInfoBox->addChild(pTextfield);
                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_OfMist)
                    {
                        pTextfield->setHtmlText(tr("?"));
                    }
                    else
                    {
                        pTextfield->setHtmlText(QString::number(count));
                    }
                    pTextfield->setPosition(x2, y);
                    m_pGameInfoBox->addChild(pTextfield);
                    y += yAdvance;

                    count = pPlayer->getUnitCount();
                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    pTextfield->setHtmlText(tr("Units: "));
                    pTextfield->setPosition(x1, y);
                    m_pGameInfoBox->addChild(pTextfield);

                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_OfMist)
                    {
                        pTextfield->setHtmlText(tr("?"));
                    }
                    else
                    {
                        pTextfield->setHtmlText(QString::number(count));
                    }
                    pTextfield->setPosition(x2, y);
                    m_pGameInfoBox->addChild(pTextfield);
                    y += yAdvance;

                    count = pPlayer->getFunds();
                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    pTextfield->setHtmlText(tr("Funds: "));
                    pTextfield->setPosition(x1, y);
                    m_pGameInfoBox->addChild(pTextfield);
                    pTextfield = MemoryManagement::create<Label>(width);
                    pTextfield->setStyle(style);
                    if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off &&
                        m_pMap->getGameRules()->getFogMode() != GameEnums::Fog_OfMist)
                    {
                        pTextfield->setHtmlText(tr("?"));
                    }
                    else
                    {
                        pTextfield->setHtmlText(QString::number(count));
                    }
                    pTextfield->setPosition(x2, y);
                    m_pGameInfoBox->addChild(pTextfield);
                    y += yAdvance;
                }

                count = pPlayer->getPlayerID();
                pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(tr("Player: "));
                pTextfield->setPosition(x1, y);
                m_pGameInfoBox->addChild(pTextfield);
                pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(QString::number(count + 1));
                pTextfield->setPosition(x2, y);
                m_pGameInfoBox->addChild(pTextfield);
                y += yAdvance;

                count = pPlayer->getTeam();
                pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(tr("Team: "));
                pTextfield->setPosition(x1, y);
                m_pGameInfoBox->addChild(pTextfield);
                pTextfield = MemoryManagement::create<Label>(width);
                pTextfield->setStyle(style);
                pTextfield->setHtmlText(QString::number(count + 1));
                pTextfield->setPosition(x2, y);
                m_pGameInfoBox->addChild(pTextfield);
                y += yAdvance;
            }
        }
    }
    Mainapp::getInstance()->continueRendering();
}

void IngameInfoBar::updateMinimap()
{
    m_pMinimap->updateMinimap(m_pMap, true);
}

void IngameInfoBar::updateCursorInfo(qint32 x, qint32 y)
{
    updateTerrainInfo(x, y, false);
}

void IngameInfoBar::updateTerrainInfo(qint32 x, qint32 y, bool update)
{
    if (m_pMap != nullptr && m_pMap->onMap(x, y) && (m_LastX != x || m_LastY != y || update))
    {
        m_pDetailedViewBox->setColorTable(m_pMap->getCurrentPlayer()->getColorTableAnim(), true);
        m_LastX = x;
        m_LastY = y;
        m_pCursorInfoBox->removeChildren();
        if (m_pMenu != nullptr)
        {
            const Player* pPlayer = m_pMenu->getCurrentViewPlayer();
            if (pPlayer != nullptr)
            {
                GameEnums::VisionType visionHide = pPlayer->getFieldVisibleType(x, y);
                if (visionHide != GameEnums::VisionType_Shrouded)
                {
                    updateDetailedView(x, y);
                    createTerrainInfo(x, y);
                    if (!createUnitInfo(x, y))
                    {
                        createMovementInfo(x, y);
                    }
                }
            }
        }
    }
}

void IngameInfoBar::createMovementInfo(qint32 x, qint32 y)
{
    constexpr qint32 yAdvance = 20;
    oxygine::TextStyle smallStyle = oxygine::TextStyle(FontManager::getFont(FONT));
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    smallStyle.multiline = false;

    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();

    Terrain* pTerrain = m_pMap->getTerrain(x, y);
    qint32 posY = 80;
    qint32 posX = 10;
    auto movements = pMovementTableManager->getLoadedRessources();
    for (const auto & movement : qAsConst(movements))
    {
        qint32 cost = pMovementTableManager->getBaseMovementPoints(movement, pTerrain, pTerrain, nullptr);
        if (cost >= 0)
        {
            QString name = pMovementTableManager->getName(movement) + " : " ;
            name += QString::number(cost);
            spLabel pTextfield = MemoryManagement::create<Label>(m_pCursorInfoBox->getScaledWidth() / 2 - 30);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(name);
            m_pCursorInfoBox->addChild(pTextfield);
            posY += yAdvance;
            if (posY + yAdvance >= m_pCursorInfoBox->getScaledHeight())
            {
                if (posX > yAdvance)
                {
                    break;
                }
                else
                {
                    posX += m_pCursorInfoBox->getScaledWidth() / 2 + 5;
                }
            }
        }
    }
}

void IngameInfoBar::updateDetailedView(qint32 x, qint32 y)
{
    static constexpr qint32 xOffset = 2;
    static constexpr qint32 yOffset = 4;
    bool hpHidden = false;
    Terrain* pTerrain = m_pMap->getTerrain(x, y);
    Building* pBuilding = pTerrain->getBuilding();
    Unit* pUnit = pTerrain->getUnit();
    Player* pPlayer = m_pMenu->getCurrentViewPlayer();
    if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    if (pUnit != nullptr)
    {
        hpHidden = pUnit->getHpHidden(m_pMenu->getCurrentViewPlayer());
    }
    GameManager* pGameManager = GameManager::getInstance();
    m_pDetailedViewBox->removeChildren();
    m_pCursorInfoBox->addChild(m_pDetailedViewBox);

    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
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

    oxygine::ResAnim* pAnimWeather = pGameManager->getResAnim(pTerrain->getWeatherOverlayId(), oxygine::ep_ignore_error);
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
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("defenseStar");
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

bool IngameInfoBar::createUnitInfo(qint32 x, qint32 y)
{
    bool created = false;
    oxygine::TextStyle smallStyle = oxygine::TextStyle(FontManager::getFont(FONT));
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    smallStyle.multiline = false;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    Terrain* pTerrain = m_pMap->getTerrain(x, y);
    Unit* pUnit = pTerrain->getUnit();
    Player* pPlayer = m_pMenu->getCurrentViewPlayer();
    if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    if (pUnit != nullptr &&
        pAnim != nullptr)
    {
        created = true;
        qint32 posY = 80;
        qint32 posX = 10;
        spLabel pTextfield = MemoryManagement::create<Label>(m_pCursorInfoBox->getScaledWidth() - 20);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(pUnit->getName());
        m_pCursorInfoBox->addChild(pTextfield);
        smallStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        posY += 22;
        float hp = pUnit->getHp();
        float divider = static_cast<float>(hp) / static_cast<float>(Unit::MAX_UNIT_HP);
        bool hpHidden = pUnit->getHpHidden(m_pMenu->getCurrentViewPlayer());
        bool perfectHpVision = pUnit->getPerfectHpView(m_pMenu->getCurrentViewPlayer());
        if (hpHidden)
        {
            divider = 0.0f;
        }
        QColor color;
        if (divider > 2.0f / 3.0f)
        {
            color = QColor(0, 128, 0, 255);
        }
        else if (divider > 1.0f / 3.0f)
        {
            color = QColor(255, 128, 0, 255);
        }
        else
        {
            color = QColor(255, 0, 0, 255);
        }
        addColorbar(divider, posX, posY, color);
        qint32 hpRounded = pUnit->getHpRounded();
        float countMax = Unit::MAX_UNIT_HP;
        pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 5);
        pTextfield->setStyle(smallStyle);
        if (hpHidden)
        {
            pTextfield->setHtmlText((tr("HP: ") + "?/10"));
        }
        else
        {
            if (perfectHpVision)
            {
                pTextfield->setHtmlText((tr("HP: ") + QString::number(hp * Unit::MAX_UNIT_HP, 'f', 0) + "/100"));
            }
            else
            {
                pTextfield->setHtmlText((tr("HP: ") + QString::number(hpRounded) + "/" + QString::number(countMax, 'f', 0)));
            }
        }
        pTextfield->setPosition(posX + 5, posY);
        m_pCursorInfoBox->addChild(pTextfield);

        qint32 fuel = pUnit->getFuel();
        qint32 maxFuel = pUnit->getMaxFuel();
        pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 5);
        pTextfield->setStyle(smallStyle);
        if (maxFuel > 0.0f)
        {
            pTextfield->setHtmlText((tr("Fuel: ") + QString::number(fuel) + "/" + QString::number(maxFuel)));
            divider = static_cast<float>(fuel) / static_cast<float>(maxFuel);
        }
        else
        {
            divider = 0.0f;
            pTextfield->setHtmlText(tr("Fuel: -/-"));
        }
        addColorbar(divider, posX + pAnim->getWidth() + 5, posY, QColor(0, 0, 255, 255));
        pTextfield->setPosition(posX + pAnim->getWidth() + 10, posY);
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pAnim->getHeight();

        qint32 ammo1 = pUnit->getAmmo1();
        qint32 maxAmmo1 = pUnit->getMaxAmmo1();
        pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 10);
        pTextfield->setStyle(smallStyle);
        if (maxAmmo1 > 0)
        {
            pTextfield->setHtmlText((tr("Ammo 1: ") + QString::number(ammo1) + "/" + QString::number(maxAmmo1)));
            divider = static_cast<float>(ammo1) / static_cast<float>(maxAmmo1);
        }
        else
        {
            divider = 0.0f;
            pTextfield->setHtmlText(tr("Ammo 1: -/-"));
        }
        addColorbar(divider, posX, posY, QColor(255, 150, 0));
        pTextfield->setPosition(posX + 5, posY);
        m_pCursorInfoBox->addChild(pTextfield);

        qint32 ammo2 = pUnit->getAmmo2();
        qint32 maxAmmo2 = pUnit->getMaxAmmo2();
        pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 5);
        pTextfield->setStyle(smallStyle);
        if (maxAmmo2 > 0)
        {
            pTextfield->setHtmlText((tr("Ammo 2: ") + QString::number(ammo2) + "/" + QString::number(maxAmmo2)));
            divider = static_cast<float>(ammo2) / static_cast<float>(maxAmmo2);
        }
        else
        {
            pTextfield->setHtmlText(tr("Ammo 2: -/-"));
            divider = 0.0f;
        }
        addColorbar(divider, posX + pAnim->getWidth() + 5, posY, QColor(0, 255, 0, 255));
        pTextfield->setPosition(posX + pAnim->getWidth() + 10, posY);
        m_pCursorInfoBox->addChild(pTextfield);

        constexpr qint32 yAdvance = 2;
        posY += yAdvance + pAnim->getHeight();
        pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Owner:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Player ") + QString::number(pUnit->getOwner()->getPlayerID() + 1));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += yAdvance + pTextfield->getTextRect().height();

        if (!pUnit->getWeapon1ID().isEmpty())
        {
            pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("Weapon 1:"));
            m_pCursorInfoBox->addChild(pTextfield);
            pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
            pTextfield->setPosition(posX + spriteWidth, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(WeaponManager::getInstance()->getName(pUnit->getWeapon1ID()));
            m_pCursorInfoBox->addChild(pTextfield);
            posY += yAdvance + pTextfield->getTextRect().height();
        }
        if (!pUnit->getWeapon2ID().isEmpty())
        {
            pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("Weapon 2:"));
            m_pCursorInfoBox->addChild(pTextfield);
            pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
            pTextfield->setPosition(posX + spriteWidth, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(WeaponManager::getInstance()->getName(pUnit->getWeapon2ID()));
            m_pCursorInfoBox->addChild(pTextfield);
            posY += yAdvance + pTextfield->getTextRect().height();
        }

        pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Move:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(MovementTableManager::getInstance()->getName(pUnit->getMovementType()));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += yAdvance + pTextfield->getTextRect().height();

        pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Movepoints:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(QString::number(pUnit->getMovementpoints(QPoint(x, y))));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += yAdvance + pTextfield->getTextRect().height();

        pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Vision:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = MemoryManagement::create<Label>(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(QString::number(pUnit->getVision(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()))));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += yAdvance + pTextfield->getTextRect().height();

        qint32 loadingPlace = pUnit->getLoadingPlace();
        qint32 loadingCount = pUnit->getLoadedUnitCount();
        if (loadingCount > loadingPlace)
        {
            loadingPlace = loadingCount;
        }
        if (loadingPlace > 0)
        {
            bool transportHidden = pUnit->getTransportHidden(pPlayer);
            for (qint32 i = 0; i < loadingPlace; ++i)
            {
                oxygine::spBox9Sprite pBox = MemoryManagement::create<oxygine::Box9Sprite>();
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel_transparent+mask");
                pBox->setResAnim(pAnim);
                pBox->setSize(GameMap::defaultImageSize + 11, GameMap::defaultImageSize + 12);
                pBox->setColorTable(pUnit->getOwner()->getColorTableAnim(), true);
                pBox->setPosition(posX, posY);
                m_pCursorInfoBox->addChild(pBox);
                if (!transportHidden)
                {
                    Unit* pTransportUnit = pUnit->getLoadedUnit(i);
                    if (pTransportUnit != nullptr)
                    {
                        spUnit pCopypTransportUnit = MemoryManagement::create<Unit>(pTransportUnit->getUnitID(), pTransportUnit->getOwner(), false, m_pMap);
                        pCopypTransportUnit->setHasMoved(pTransportUnit->getHasMoved());
                        pCopypTransportUnit->setHp(pTransportUnit->getHp());
                        pCopypTransportUnit->setAmmo1(pTransportUnit->getAmmo1());
                        pCopypTransportUnit->setAmmo2(pTransportUnit->getAmmo2());
                        pCopypTransportUnit->setFuel(pTransportUnit->getFuel());
                        pCopypTransportUnit->setUnitRank(pTransportUnit->getUnitRank());
                        pCopypTransportUnit->setPosition(6, 5);
                        pBox->addChild(pCopypTransportUnit);
                    }
                }
                posX += GameMap::defaultImageSize + 15;
            }
        }
    }
    return created;
}

void IngameInfoBar::createTerrainInfo(qint32 x, qint32 y)
{
    oxygine::TextStyle smallStyle = oxygine::TextStyle(FontManager::getFont(FONT));
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    smallStyle.multiline = false;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    if (pAnim != nullptr)
    {
        Terrain* pTerrain = m_pMap->getTerrain(x, y);
        spBuilding pBuilding = pTerrain->getSpBuilding();
        spUnit pUnit = pTerrain->getSpUnit();
        Player* pPlayer = m_pMenu->getCurrentViewPlayer();
        if (pUnit.get() != nullptr && pUnit->isStealthed(pPlayer))
        {
            pUnit.reset();
        }
        qint32 posY = 5;
        qint32 posX = 10;
        spLabel pTextfield = MemoryManagement::create<Label>(m_pCursorInfoBox->getScaledWidth() - 20);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        QString name = "";
        if (pBuilding.get() != nullptr)
        {
            name = pBuilding->getName();
        }
        else
        {
            name = pTerrain->getTerrainName();
        }
        pTextfield->setHtmlText(name);
        m_pCursorInfoBox->addChild(pTextfield);
        smallStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        posY += 22;
        // draw building hp
        qint32 hp = 0;
        if ((pBuilding.get() != nullptr) && (pBuilding->getHp() > 0))
        {
            hp = pBuilding->getHp();
        }
        else if ((pTerrain->getHp() > 0))
        {
            hp = pTerrain->getHp();
        }
        bool barAdded = false;
        if (hp > 0)
        {
            qint32 hpMax = 100;
            if (hp > 100)
            {
                hpMax = hp;
            }
            float divider = static_cast<float>(hp) / static_cast<float>(hpMax);
            QColor color;
            if (divider > 2.0f / 3.0f)
            {
                color = QColor(0, 128, 0, 255);
            }
            else if (divider > 1.0f / 3.0f)
            {
                color = QColor(255, 128, 0, 255);
            }
            else
            {
                color = QColor(255, 0, 0, 255);
            }
            addColorbar(divider, posX + pAnim->getWidth() + 10, posY, color);
            pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 5);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("HP: ") + QString::number(hp));
            pTextfield->setPosition(posX + 10 + pAnim->getWidth() + 5, posY);
            m_pCursorInfoBox->addChild(pTextfield);
            barAdded = true;
        }
        if (pUnit->getCapturePoints() > 0)
        {
            constexpr qint32 maxCapturepoints = 20;
            qint32 resistance = maxCapturepoints;
            if (pUnit != nullptr)
            {
                resistance = maxCapturepoints - pUnit->getCapturePoints();
            }
            addColorbar(static_cast<float>(resistance) / static_cast<float>(maxCapturepoints), posX, posY, Qt::cyan);
            pTextfield = MemoryManagement::create<Label>(pAnim->getWidth() - 5);
            pTextfield->setPosition(posX + 5, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText((tr("Resistance: ") + QString::number(resistance)));
            m_pCursorInfoBox->addChild(pTextfield);
            barAdded = true;
        }
        if (barAdded)
        {
            posY += 5 + pAnim->getHeight();
        }

        // show building information
        if (pBuilding.get() != nullptr)
        {
            GameEnums::VisionType visionHide = pPlayer->getFieldVisibleType(x, y);
            QString ownerText = tr("Neutral");
            if (pBuilding->getOwner() != nullptr &&
                visionHide == GameEnums::VisionType_Clear)
            {
                ownerText = tr("Player ") + QString::number(pBuilding->getOwner()->getPlayerID() + 1);
            }
            pTextfield = MemoryManagement::create<Label>(spriteWidth - 5);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("Owner:"));
            m_pCursorInfoBox->addChild(pTextfield);
            pTextfield = MemoryManagement::create<Label>(spriteWidth);
            pTextfield->setPosition(posX + spriteWidth, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(ownerText);
            m_pCursorInfoBox->addChild(pTextfield);
        }
    }
}

void IngameInfoBar::addColorbar(float divider, qint32 posX, qint32 posY, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    if (pAnim != nullptr)
    {
        oxygine::spColorRectSprite pColorBar = MemoryManagement::create<oxygine::ColorRectSprite>();
        pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
        pColorBar->setPosition(posX, posY);
        pColorBar->setColor(color);
        m_pCursorInfoBox->addChild(pColorBar);
        pColorBar = MemoryManagement::create<oxygine::ColorRectSprite>();
        pColorBar->setColor(127, 127, 127, 255);
        pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
        pColorBar->setPosition(posX + pAnim->getWidth() - pColorBar->getScaledWidth(), posY);
        m_pCursorInfoBox->addChild(pColorBar);
        oxygine::spSprite pSprite = MemoryManagement::create<oxygine::Sprite>();
        pSprite->setResAnim(pAnim);
        pSprite->setPosition(posX, posY);
        m_pCursorInfoBox->addChild(pSprite);
    }
}

void IngameInfoBar::syncMinimapPosition()
{
    
    if (m_pMap != nullptr)
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
        m_pMinimapSlider->getContent()->setPosition(newX, newY);
    }
}

GameMap *IngameInfoBar::getMap() const
{
    return m_pMap;
}

void IngameInfoBar::setMap(GameMap *newMap)
{
    m_pMap = newMap;
}

oxygine::spBox9Sprite IngameInfoBar::getDetailedViewBox() const
{
    return m_pDetailedViewBox;
}
