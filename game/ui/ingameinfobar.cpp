#include "game/ui/ingameinfobar.h"

#include "coreengine/mainapp.h"

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
#include "game/battleanimationsprite.h"
#include "menue/gamemenue.h"

#include "objects/base/label.h"

IngameInfoBar::IngameInfoBar()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    qint32 width = 300;
    qint32 cursorInfoHeigth = 330;
    qint32 gameInfoHeigth = 290;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pMiniMapBox = new oxygine::Box9Sprite();
    pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setResAnim(pAnim);

    pMiniMapBox->setSize(width, Settings::getHeight() - cursorInfoHeigth - gameInfoHeigth);
    pMiniMapBox->setPosition(0, 0);
    if (Settings::getHeight() - cursorInfoHeigth - gameInfoHeigth < 50)
    {
        pMiniMapBox->setHeight(100);
        setScale(Settings::getHeight() / static_cast<float>(100 + cursorInfoHeigth + gameInfoHeigth));
    }
    setSize(width, Settings::getHeight());


    pMiniMapBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    m_pMinimap = new Minimap();
    m_pMinimap->setPosition(0, 0);
    m_pMinimap->updateMinimap(GameMap::getInstance());
    m_pMinimap->setScale(2.0f);
    m_pMinimapSlider = new oxygine::SlidingActor();
    m_pMinimapSlider->setPosition(10, 10);
    m_pMinimapSlider->setSize(pMiniMapBox->getWidth() - 20,
                              pMiniMapBox->getHeight() - 20);
    m_pMinimapSlider->setContent(m_pMinimap);
    pMiniMapBox->addChild(m_pMinimapSlider);
    addChild(pMiniMapBox);

    m_pGameInfoBox = new oxygine::Box9Sprite();
    m_pGameInfoBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pGameInfoBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pGameInfoBox->setResAnim(pAnim);
    m_pGameInfoBox->setPosition(0, pMiniMapBox->getHeight());
    m_pGameInfoBox->setSize(width, gameInfoHeigth);
    m_pGameInfoBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    addChild(m_pGameInfoBox);


    m_pCursorInfoBox = new oxygine::Box9Sprite();
    m_pCursorInfoBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pCursorInfoBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pCursorInfoBox->setResAnim(pAnim);
    m_pCursorInfoBox->setPosition(0, pMiniMapBox->getHeight() + gameInfoHeigth);
    m_pCursorInfoBox->setSize(width, cursorInfoHeigth);
    m_pCursorInfoBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));

    setX(Settings::getWidth() - getScaledWidth());
    addChild(m_pCursorInfoBox);
}

void IngameInfoBar::updatePlayerInfo()
{

    m_pGameInfoBox->removeChildren();

    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        spGameMenue pMenu = GameMenue::getInstance();
        if (pMenu.get() != nullptr)
        {
            Player* pPlayer = pMap->getCurrentPlayer();
            oxygine::spSprite pSprite = new oxygine::Sprite();
            CO* pCO = pPlayer->getCO(0);
            oxygine::ResAnim* pAnim = nullptr;
            if (pCO != nullptr)
            {
                pAnim = pCO->getResAnim((pCO->getCoID() + "+face"));
            }
            else
            {
                pAnim = pCOSpriteManager->getResAnim("no_co+face");
            }
            pSprite->setScale(1.8f);
            pSprite->setResAnim(pAnim);
            pSprite->setPosition(12, 10);
            m_pGameInfoBox->addChild(pSprite);
            pSprite = new oxygine::Sprite();
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
            pSprite->setPosition(109, 10);
            pSprite->setScale(1.8f);
            m_pGameInfoBox->addChild(pSprite);

            pSprite = new oxygine::Sprite();
            pAnim = pGameManager->getResAnim((pMap->getGameRules()->getCurrentWeather()->getWeatherSymbol()));
            if (pAnim != nullptr)
            {
                pSprite->setResAnim(pAnim);
                pSprite->setPosition(205, 12);
                pSprite->setScale(85 / pAnim->getWidth());
            }
            m_pGameInfoBox->addChild(pSprite);

            // boxes for co's and weather
            ObjectManager* pObjectManager = ObjectManager::getInstance();
            pAnim = pObjectManager->getResAnim("panel_transparent");
            oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
            pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setResAnim(pAnim);
            pBox->setSize(95, 95);
            pBox->setPosition(104, 8);
            m_pGameInfoBox->addChild(pBox);
            pBox = new oxygine::Box9Sprite();
            pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setResAnim(pAnim);
            pBox->setSize(95, 95);
            pBox->setPosition(7, 8);
            m_pGameInfoBox->addChild(pBox);
            // weather box
            pBox = new oxygine::Box9Sprite();
            pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
            pBox->setResAnim(pAnim);
            pBox->setSize(95, 95);
            pBox->setPosition(199, 8);
            m_pGameInfoBox->addChild(pBox);

            if (pMap->getGameRules()->getWeatherPrediction())
            {
                pSprite = new oxygine::Sprite();
                Weather* pWeather = pMap->getGameRules()->getWeatherAtDay(1, pMap->getCurrentPlayer()->getPlayerID());
                if (pWeather != nullptr)
                {
                    pAnim = pGameManager->getResAnim((pWeather->getWeatherSymbol()));
                    if (pAnim != nullptr)
                    {
                        pSprite->setResAnim(pAnim);
                        pSprite->setPosition(206, 108);
                        pSprite->setScale(37 / pAnim->getWidth());
                    }
                }
                m_pGameInfoBox->addChild(pSprite);
                pBox = new oxygine::Box9Sprite();
                pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                pAnim = pObjectManager->getResAnim("panel_transparent");
                pBox->setResAnim(pAnim);
                pBox->setSize(47, 47);
                pBox->setPosition(200, 104);
                m_pGameInfoBox->addChild(pBox);

                pSprite = new oxygine::Sprite();
                pWeather = pMap->getGameRules()->getWeatherAtDay(2, pMap->getCurrentPlayer()->getPlayerID());
                if (pWeather != nullptr)
                {
                    pAnim = pGameManager->getResAnim((pWeather->getWeatherSymbol()));
                    if (pAnim != nullptr)
                    {
                        pSprite->setResAnim(pAnim);
                        pSprite->setPosition(253, 108);
                        pSprite->setScale(37 / pAnim->getWidth());
                    }
                }
                m_pGameInfoBox->addChild(pSprite);
                pBox = new oxygine::Box9Sprite();
                pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                pAnim = pObjectManager->getResAnim("panel_transparent");
                pBox->setResAnim(pAnim);
                pBox->setSize(47, 47);
                pBox->setPosition(247, 104);
                m_pGameInfoBox->addChild(pBox);
            }


            oxygine::TextStyle style = FontManager::getMainFont16();
            style.color = FontManager::getFontColor();
            style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
            style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
            style.multiline = false;

            qint32 count = pPlayer->getBuildingCount();
            qint32 width = 95;
            qint32 y = 110;
            qint32 x1 = 10;
            qint32 x2 = 110;

            spLabel pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Day:"));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);

            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(pMap->getCurrentDay()));
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            Player* pViewPlayer = pMenu->getCurrentViewPlayer();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Buildings:"));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText("?");
            }
            else
            {
                pTextfield->setHtmlText(QString::number(count));
            }
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getUnitCount();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Units: "));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);

            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText("?");
            }
            else
            {
                pTextfield->setHtmlText(QString::number(count));
            }
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getFunds();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Funds: "));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText("?");
            }
            else
            {
                pTextfield->setHtmlText(QString::number(count));
            }
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getPlayerID();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Player: "));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(count + 1));
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getTeam();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(tr("Team: "));
            pTextfield->setPosition(x1, y);
            m_pGameInfoBox->addChild(pTextfield);
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(count + 1));
            pTextfield->setPosition(x2, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;
        }
    }
}

void IngameInfoBar::updateMinimap()
{
    m_pMinimap->updateMinimap(GameMap::getInstance(), true);
}

void IngameInfoBar::updateCursorInfo(qint32 x, qint32 y)
{
    updateTerrainInfo(x, y, false);
}

void IngameInfoBar::updateTerrainInfo(qint32 x, qint32 y, bool update)
{
    
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr && pMap->onMap(x, y) && (m_LastX != x || m_LastY != y || update))
    {
        m_LastX = x;
        m_LastY = y;
        m_pCursorInfoBox->removeChildren();
        spGameMenue pGamemenu = GameMenue::getInstance();
        if (pGamemenu.get() != nullptr)
        {
            Player* pPlayer = pGamemenu->getCurrentViewPlayer();
            GameEnums::VisionType visionHide = pPlayer->getFieldVisibleType(x, y);
            if (visionHide != GameEnums::VisionType_Shrouded)
            {
                updateDetailedView(x, y);
                createTerrainInfo(x, y);
                createUnitInfo(x, y);
            }
        }
    }
}

void IngameInfoBar::updateDetailedView(qint32 x, qint32 y)
{
    bool HpHidden = false;
    spGameMenue pGamemenu = GameMenue::getInstance();
    spGameMap pMap = GameMap::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    Terrain* pTerrain = pMap->getTerrain(x, y);
    spBuilding pBuilding = pTerrain->getBuilding();
    spUnit pUnit = pTerrain->getUnit();
    Player* pPlayer = pGamemenu->getCurrentViewPlayer();
    if (pUnit.get() != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    if (pUnit.get() != nullptr)
    {
        HpHidden = pUnit->getHpHidden(pGamemenu->getCurrentViewPlayer());
    }
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel_transparent");
    pBox->setResAnim(pAnim);
    pBox->setSize(136, 200);
    pBox->setPosition(-spriteWidth - 8, m_pCursorInfoBox->getHeight() - pBox->getHeight());
    m_pCursorInfoBox->addChild(pBox);

    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
    float speed = 0;
    if (pUnit.get() != nullptr)
    {
        pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase(), oxygine::ep_ignore_error);
        pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
        pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
        speed = pUnit->getTerrainAnimationMoveSpeed();
    }
    else if (pBuilding.get() != nullptr)
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
    oxygine::spSlidingSprite pTerrainSprite = new oxygine::SlidingSprite();
    pTerrainSprite->setPosition(6, 3);
    pTerrainSprite->setResAnim(pAnimBase);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeed(speed);
    pBox->addChild(pTerrainSprite);
    pTerrainSprite = new oxygine::SlidingSprite();
    pTerrainSprite->setPosition(6, 3);
    pTerrainSprite->setResAnim(pAnimBack);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeed(speed);
    pBox->addChild(pTerrainSprite);
    pTerrainSprite = new oxygine::SlidingSprite();
    pTerrainSprite->setPosition(6, 3);
    pTerrainSprite->setResAnim(pAnimFore);
    pTerrainSprite->setSize(spriteWidth, spriteHeigth);
    pTerrainSprite->setSpeed(speed);
    pBox->addChild(pTerrainSprite);
    if (pUnit.get() != nullptr)
    {
        BattleAnimationSprite* pBattleAnimationSprite = new BattleAnimationSprite(pUnit, pUnit->getTerrain(), BattleAnimationSprite::standingAnimation);
        pBattleAnimationSprite->setPosition(6, 3);
        if (HpHidden)
        {
            pBattleAnimationSprite->setHpRounded(Unit::MAX_UNIT_HP);
            pBattleAnimationSprite->loadAnimation(BattleAnimationSprite::standingAnimation);
        }
        pBattleAnimationSprite->setPriority(3);
        pBox->addChild(pBattleAnimationSprite);
    }
    qint32 TerrainDefense = 0;
    if (pUnit.get() != nullptr)
    {
        TerrainDefense = pUnit->getTerrainDefense();
    }
    else
    {
        TerrainDefense = pTerrain->getBaseDefense();
    }
    float defenseY = pTerrainSprite->getY() + 5;
    float startDefenseX = pTerrainSprite->getX() + 5;
    float defenseX = startDefenseX;
    pAnim = pGameManager->getResAnim("defenseStar");
    for (qint32 i = 1; i <= TerrainDefense; i++)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setPosition(defenseX, defenseY);
        pSprite->setResAnim(pAnim);
        pSprite->setScale(1.5f);
        pSprite->setPriority(4);
        pBox->addChild(pSprite);
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

void IngameInfoBar::createUnitInfo(qint32 x, qint32 y)
{
    oxygine::TextStyle smallStyle = FontManager::getMainFont16();
    smallStyle.color = FontManager::getFontColor();
    smallStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    smallStyle.multiline = false;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    spGameMenue pGamemenu = GameMenue::getInstance();
    spGameMap pMap = GameMap::getInstance();
    Terrain* pTerrain = pMap->getTerrain(x, y);
    spUnit pUnit = pTerrain->getUnit();
    Player* pPlayer = pGamemenu->getCurrentViewPlayer();
    if (pUnit.get() != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    if (pUnit.get() != nullptr)
    {
        qint32 posY = 80;
        qint32 posX = 10;
        spLabel pTextfield = new Label(m_pCursorInfoBox->getWidth() - 20);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(pUnit->getName());
        m_pCursorInfoBox->addChild(pTextfield);
        smallStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        posY += 22;
        float hp = pUnit->getHp();
        float divider = static_cast<float>(hp) / static_cast<float>(Unit::MAX_UNIT_HP);
        bool HpHidden = pUnit->getHpHidden(pGamemenu->getCurrentViewPlayer());
        bool perfectHpVision = pUnit->getPerfectHpView(pGamemenu->getCurrentViewPlayer());
        if (HpHidden)
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
        pTextfield = new Label(pAnim->getWidth() - 10);
        pTextfield->setStyle(smallStyle);
        if (HpHidden)
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
        divider = 0.0f;
        pTextfield = new Label(pAnim->getWidth() - 10);
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
        addColorbar(divider, posX + pAnim->getWidth() + 10, posY, QColor(0, 0, 255, 255));
        pTextfield->setPosition(posX + pAnim->getWidth() + 15, posY);
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pAnim->getHeight();

        qint32 ammo1 = pUnit->getAmmo1();
        qint32 maxAmmo1 = pUnit->getMaxAmmo1();
        pTextfield = new Label(pAnim->getWidth() - 10);
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
        pTextfield = new Label(pAnim->getWidth() - 10);
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
        addColorbar(divider, posX + pAnim->getWidth() + 10, posY, QColor(0, 255, 0, 255));
        pTextfield->setPosition(posX + pAnim->getWidth() + 15, posY);
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pAnim->getHeight();

        pTextfield = new Label(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Owner:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = new Label(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Player ") + QString::number(pUnit->getOwner()->getPlayerID() + 1));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pTextfield->getTextRect().getHeight();

        if (!pUnit->getWeapon1ID().isEmpty())
        {
            pTextfield = new Label(spriteWidth - 5);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("Weapon 1:"));
            m_pCursorInfoBox->addChild(pTextfield);
            pTextfield = new Label(spriteWidth + 10);
            pTextfield->setPosition(posX + spriteWidth, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(WeaponManager::getInstance()->getName(pUnit->getWeapon1ID()));
            m_pCursorInfoBox->addChild(pTextfield);
            posY += 5 + pTextfield->getTextRect().getHeight();
        }
        if (!pUnit->getWeapon2ID().isEmpty())
        {
            pTextfield = new Label(spriteWidth - 5);
            pTextfield->setPosition(posX, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(tr("Weapon 2:"));
            m_pCursorInfoBox->addChild(pTextfield);
            pTextfield = new Label(spriteWidth + 10);
            pTextfield->setPosition(posX + spriteWidth, posY);
            pTextfield->setStyle(smallStyle);
            pTextfield->setHtmlText(WeaponManager::getInstance()->getName(pUnit->getWeapon2ID()));
            m_pCursorInfoBox->addChild(pTextfield);
            posY += 5 + pTextfield->getTextRect().getHeight();
        }

        pTextfield = new Label(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Move:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = new Label(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(MovementTableManager::getInstance()->getName(pUnit->getMovementType()));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pTextfield->getTextRect().getHeight();

        pTextfield = new Label(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Movepoints:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = new Label(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(QString::number(pUnit->getMovementpoints(QPoint(x, y))));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pTextfield->getTextRect().getHeight();

        pTextfield = new Label(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Vision:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = new Label(spriteWidth + 10);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(QString::number(pUnit->getVision(QPoint(pUnit->getX(), pUnit->getY()))));
        m_pCursorInfoBox->addChild(pTextfield);
        posY += 5 + pTextfield->getTextRect().getHeight();

        qint32 loadingPlace = pUnit->getLoadingPlace();
        if (loadingPlace > 0)
        {
            bool transportHidden = pUnit->getTransportHidden(pPlayer);
            for (qint32 i = 0; i < loadingPlace; ++i)
            {
                oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
                pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel_transparent");
                pBox->setResAnim(pAnim);
                pBox->setSize(GameMap::defaultImageSize + 11, GameMap::defaultImageSize + 12);
                pBox->setPosition(posX, posY);
                m_pCursorInfoBox->addChild(pBox);
                if (!transportHidden)
                {
                    Unit* pTransportUnit = pUnit->getLoadedUnit(i);
                    if (pTransportUnit != nullptr)
                    {
                        spUnit pCopypTransportUnit = new Unit(pTransportUnit->getUnitID(), pTransportUnit->getOwner(), false);
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
}

void IngameInfoBar::createTerrainInfo(qint32 x, qint32 y)
{
    oxygine::TextStyle smallStyle = FontManager::getMainFont16();
    smallStyle.color = FontManager::getFontColor();
    smallStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    smallStyle.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    smallStyle.multiline = false;
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    spGameMenue pGamemenu = GameMenue::getInstance();
    spGameMap pMap = GameMap::getInstance();
    Terrain* pTerrain = pMap->getTerrain(x, y);
    spBuilding pBuilding = pTerrain->getBuilding();
    spUnit pUnit = pTerrain->getUnit();
    Player* pPlayer = pGamemenu->getCurrentViewPlayer();
    if (pUnit.get() != nullptr && pUnit->isStealthed(pPlayer))
    {
        pUnit = nullptr;
    }
    qint32 posY = 5;
    qint32 posX = 10;
    spLabel pTextfield = new Label(m_pCursorInfoBox->getWidth() - 20);
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
        addColorbar(divider, posX + 10 + pAnim->getWidth() + 5, posY, color);
        pTextfield = new Label(pAnim->getWidth() - 10);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("HP: ") + QString::number(hp));
        pTextfield->setPosition(posX + 10 + pAnim->getWidth() + 10, posY);
        m_pCursorInfoBox->addChild(pTextfield);
        barAdded = true;
    }
    if (pBuilding.get() != nullptr)
    {
        constexpr qint32 maxCapturepoints = 20;
        qint32 resistance = maxCapturepoints;
        if (pUnit.get() != nullptr)
        {
            resistance = maxCapturepoints - pUnit->getCapturePoints();
        }
        addColorbar(static_cast<float>(resistance) / static_cast<float>(maxCapturepoints), posX, posY, Qt::cyan);
        pTextfield = new Label(pAnim->getWidth() - 10);
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
        pTextfield = new Label(spriteWidth - 5);
        pTextfield->setPosition(posX, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(tr("Owner:"));
        m_pCursorInfoBox->addChild(pTextfield);
        pTextfield = new Label(spriteWidth);
        pTextfield->setPosition(posX + spriteWidth, posY);
        pTextfield->setStyle(smallStyle);
        pTextfield->setHtmlText(ownerText);
        m_pCursorInfoBox->addChild(pTextfield);
    }
}

void IngameInfoBar::addColorbar(float divider, qint32 posX, qint32 posY, QColor color)
{
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("barforeground");
    oxygine::spColorRectSprite pColorBar = new oxygine::ColorRectSprite();
    pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
    pColorBar->setPosition(posX, posY);
    pColorBar->setColor(color);
    m_pCursorInfoBox->addChild(pColorBar);
    pColorBar = new oxygine::ColorRectSprite();
    pColorBar->setColor(127, 127, 127, 255);
    pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
    pColorBar->setPosition(posX + pAnim->getWidth() - pColorBar->getWidth(), posY);
    m_pCursorInfoBox->addChild(pColorBar);
    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPosition(posX, posY);
    m_pCursorInfoBox->addChild(pSprite);
}
