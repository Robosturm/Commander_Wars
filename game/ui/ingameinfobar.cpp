#include "ingameinfobar.h"

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


    pMiniMapBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
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
    m_pGameInfoBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(m_pGameInfoBox);


    m_pCursorInfoBox = new oxygine::Box9Sprite();
    m_pCursorInfoBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    m_pCursorInfoBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    m_pCursorInfoBox->setResAnim(pAnim);
    m_pCursorInfoBox->setPosition(0, pMiniMapBox->getHeight() + gameInfoHeigth);
    m_pCursorInfoBox->setSize(width, cursorInfoHeigth);
    m_pCursorInfoBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));

    setX(Settings::getWidth() - getScaledWidth());
    addChild(m_pCursorInfoBox);
}

void IngameInfoBar::updatePlayerInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
                pAnim = pCOSpriteManager->getResAnim((pCO->getCoID() + "+face"));
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
                pAnim = pCOSpriteManager->getResAnim((pCO->getCoID() + "+face"));
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
            qint32 width = 285;
            qint32 y = 110;

            spLabel pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText((tr("Day: ") + QString::number(pMap->getCurrentDay())));
            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            Player* pViewPlayer = pMenu->getCurrentViewPlayer();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText((tr("Buildings: ") + "?"));
            }
            else
            {
                pTextfield->setHtmlText((tr("Buildings: ") + QString::number(count)));
            }
            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getUnitCount();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText((tr("Units: ") + "?"));
            }
            else
            {
                pTextfield->setHtmlText((tr("Units: ") + QString::number(count)));
            }
            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getFunds();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);

            if (pViewPlayer->getTeam() != pPlayer->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                pTextfield->setHtmlText((tr("Funds: ") + "?"));
            }
            else
            {
                pTextfield->setHtmlText((tr("Funds: ") + QString::number(count)));
            }

            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getPlayerID();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText((tr("Player: ") + QString::number(count + 1)));
            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            y += 25;

            count = pPlayer->getTeam();
            pTextfield = new Label(width);
            pTextfield->setStyle(style);
            pTextfield->setHtmlText((tr("Team: ") + QString::number(count + 1)));
            pTextfield->setPosition(10, y);
            m_pGameInfoBox->addChild(pTextfield);
            pApp->continueThread();
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
                Terrain* pTerrain = pMap->getTerrain(x, y);
                spBuilding pBuilding = pTerrain->getBuilding();
                spUnit pUnit = pTerrain->getUnit();
                if (pUnit.get() != nullptr && pUnit->isStealthed(pPlayer))
                {
                    pUnit = nullptr;
                }
                GameManager* pGameManager = GameManager::getInstance();
                ObjectManager* pObjectManager = ObjectManager::getInstance();
                oxygine::spSprite pSprite = new oxygine::Sprite();
                oxygine::ResAnim* pAnim = nullptr;

                oxygine::TextStyle smallStyle = FontManager::getMainFont16();
                smallStyle.color = FontManager::getFontColor();
                smallStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
                smallStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
                smallStyle.multiline = false;


                // draw building hp
                qint32 width = 145;
                spLabel pTextfield = new Label(width);
                qint32 hp = 0;
                if ((pBuilding.get() != nullptr) && (pBuilding->getHp() > 0))
                {
                    hp = pBuilding->getHp();
                }
                else if ((pTerrain->getHp() > 0))
                {
                    hp = pTerrain->getHp();
                }
                if (hp > 0 && pUnit.get() == nullptr)
                {
                    pAnim = pObjectManager->getResAnim("barforeground");
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText(tr("HP: ") + QString::number(hp));
                    pTextfield->setPosition(10, 10);
                    m_pCursorInfoBox->addChild(pTextfield);
                    qint32 hpMax = 100;
                    if (hp > 100)
                    {
                        hpMax = hp;
                    }
                    oxygine::spColorRectSprite pColorBar = new oxygine::ColorRectSprite();
                    float divider = static_cast<float>(hp) / static_cast<float>(hpMax);
                    if (divider > 2.0f / 3.0f)
                    {
                        pColorBar->setColor(0, 128, 0, 255);
                    }
                    else if (divider > 1.0f / 3.0f)
                    {
                        pColorBar->setColor(255, 128, 0, 255);
                    }
                    else
                    {
                        pColorBar->setColor(255, 0, 0, 255);
                    }
                    pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
                    pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 12);
                    m_pCursorInfoBox->addChild(pColorBar);
                    pColorBar = new oxygine::ColorRectSprite();
                    pColorBar->setColor(127, 127, 127, 255);
                    pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
                    pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 12);
                    m_pCursorInfoBox->addChild(pColorBar);
                    pSprite = new oxygine::Sprite();
                    pSprite->setResAnim(pAnim);
                    pSprite->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 12);
                    m_pCursorInfoBox->addChild(pSprite);
                }


                pAnim = pObjectManager->getResAnim("barforeground");
                bool HpHidden = false;
                bool perfectHpVision = false;
                if (pUnit.get() != nullptr)
                {
                    HpHidden = pUnit->getHpHidden(pGamemenu->getCurrentViewPlayer());
                    perfectHpVision = pUnit->getPerfectHpView(pGamemenu->getCurrentViewPlayer());
                    float count = pUnit->getHp();
                    qint32 hpRounded = pUnit->getHpRounded();
                    float countMax = 10.0f;
                    pTextfield = new Label(width);
                    pTextfield->setStyle(smallStyle);
                    if (HpHidden)
                    {
                        pTextfield->setHtmlText((tr("HP: ") + "?/10"));
                    }
                    else
                    {
                        if (perfectHpVision)
                        {
                            pTextfield->setHtmlText((tr("HP: ") + QString::number(count * 10, 'f', 0) + "/100"));
                        }
                        else
                        {
                            pTextfield->setHtmlText((tr("HP: ") + QString::number(hpRounded) + "/" + QString::number(countMax, 'f', 0)));
                        }
                    }
                    pTextfield->setPosition(10, 10);
                    m_pCursorInfoBox->addChild(pTextfield);

                    oxygine::spColorRectSprite pColorBar = new oxygine::ColorRectSprite();
                    float divider = 0;
                    if (!HpHidden)
                    {
                        divider = count / countMax;
                        if (divider > 2.0f / 3.0f)
                        {
                            pColorBar->setColor(0, 128, 0, 255);
                        }
                        else if (divider > 1.0f / 3.0f)
                        {
                            pColorBar->setColor(255, 128, 0, 255);
                        }
                        else
                        {
                            pColorBar->setColor(255, 0, 0, 255);
                        }
                        pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 12);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    pColorBar = new oxygine::ColorRectSprite();
                    pColorBar->setColor(127, 127, 127, 255);
                    pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
                    pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 12);
                    m_pCursorInfoBox->addChild(pColorBar);



                    qint32 countInt = pUnit->getAmmo1();
                    qint32 countMaxInt = pUnit->getMaxAmmo1();
                    pTextfield = new Label(width);
                    pTextfield->setStyle(smallStyle);
                    if (countMaxInt > 0)
                    {
                        pTextfield->setHtmlText((tr("Ammo1: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)));
                        pColorBar = new oxygine::ColorRectSprite();
                        divider = static_cast<float>(countInt) / static_cast<float>(countMaxInt);
                        pColorBar->setColor(Qt::yellow);
                        pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 37);
                        m_pCursorInfoBox->addChild(pColorBar);

                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 37);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    else
                    {
                        pTextfield->setHtmlText(tr("Ammo1: -/-"));
                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize(pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 37);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    pTextfield->setPosition(10, 35);
                    m_pCursorInfoBox->addChild(pTextfield);

                    countInt = pUnit->getAmmo2();
                    countMaxInt = pUnit->getMaxAmmo2();
                    pTextfield = new Label(width);
                    pTextfield->setStyle(smallStyle);
                    if (countMaxInt > 0)
                    {
                        pTextfield->setHtmlText((tr("Ammo2: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)));
                        pColorBar = new oxygine::ColorRectSprite();
                        divider = static_cast<float>(countInt) / static_cast<float>(countMaxInt);
                        pColorBar->setColor(0, 255, 0, 255);
                        pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 62);
                        m_pCursorInfoBox->addChild(pColorBar);

                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 62);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    else
                    {
                        pTextfield->setHtmlText(tr("Ammo2: -/-"));
                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize(pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 62);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    pTextfield->setPosition(10, 60);
                    m_pCursorInfoBox->addChild(pTextfield);


                    countInt = pUnit->getFuel();
                    countMaxInt = pUnit->getMaxFuel();
                    pTextfield = new Label(width);
                    pTextfield->setStyle(smallStyle);
                    if (countMaxInt > 0)
                    {
                        pTextfield->setHtmlText((tr("Fuel: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)));

                        pColorBar = new oxygine::ColorRectSprite();
                        divider = static_cast<float>(countInt) / static_cast<float>(countMaxInt);
                        pColorBar->setColor(0, 0, 255, 255);
                        pColorBar->setSize(divider * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 87);
                        m_pCursorInfoBox->addChild(pColorBar);

                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize((1 - divider) * pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 87);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    else
                    {
                        pTextfield->setHtmlText(tr("Fuel: -/-"));
                        pColorBar = new oxygine::ColorRectSprite();
                        pColorBar->setColor(127, 127, 127, 255);
                        pColorBar->setSize(pAnim->getWidth(), pAnim->getHeight());
                        pColorBar->setPosition(m_pCursorInfoBox->getWidth() - 10 - pColorBar->getWidth(), 87);
                        m_pCursorInfoBox->addChild(pColorBar);
                    }
                    pTextfield->setPosition(10, 85);
                    m_pCursorInfoBox->addChild(pTextfield);

                    // draw unit overlay
                    for (qint32 i = 0; i < 4; i++)
                    {
                        pSprite = new oxygine::Sprite();
                        pSprite->setResAnim(pAnim);
                        pSprite->setPosition(m_pCursorInfoBox->getWidth() - 10 - pAnim->getWidth(), 10 + i * 25 + 2);
                        m_pCursorInfoBox->addChild(pSprite);
                    }
                }

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
                pTerrainSprite->setPosition(m_pCursorInfoBox->getWidth() - spriteWidth - 10, 120);
                pTerrainSprite->setResAnim(pAnimBase);
                pTerrainSprite->setSize(spriteWidth, spriteHeigth);
                pTerrainSprite->setSpeed(speed);
                m_pCursorInfoBox->addChild(pTerrainSprite);
                pTerrainSprite = new oxygine::SlidingSprite();
                pTerrainSprite->setPosition(m_pCursorInfoBox->getWidth() - spriteWidth - 10, 120);
                pTerrainSprite->setResAnim(pAnimBack);
                pTerrainSprite->setSize(spriteWidth, spriteHeigth);
                pTerrainSprite->setSpeed(speed);
                m_pCursorInfoBox->addChild(pTerrainSprite);
                pTerrainSprite = new oxygine::SlidingSprite();
                pTerrainSprite->setPosition(m_pCursorInfoBox->getWidth() - spriteWidth - 10, 120);
                pTerrainSprite->setResAnim(pAnimFore);
                pTerrainSprite->setSize(spriteWidth, spriteHeigth);
                pTerrainSprite->setSpeed(speed);
                m_pCursorInfoBox->addChild(pTerrainSprite);


                oxygine::spBox9Sprite pBox = new oxygine::Box9Sprite();
                pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
                pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
                pAnim = pObjectManager->getResAnim("panel_transparent");
                pBox->setResAnim(pAnim);
                pBox->setSize(136, 200);
                pBox->setPosition(m_pCursorInfoBox->getWidth() - spriteWidth - 17, 116);
                m_pCursorInfoBox->addChild(pBox);

                if (pUnit.get() != nullptr)
                {
                    BattleAnimationSprite* pBattleAnimationSprite = new BattleAnimationSprite(pUnit, pUnit->getTerrain(), BattleAnimationSprite::standingAnimation);
                    pBattleAnimationSprite->setPosition(m_pCursorInfoBox->getWidth() - spriteWidth - 10, 120);
                    if (HpHidden)
                    {
                        pBattleAnimationSprite->setHpRounded(10);
                        pBattleAnimationSprite->loadAnimation(BattleAnimationSprite::standingAnimation);
                    }
                    pBattleAnimationSprite->setPriority(3);
                    m_pCursorInfoBox->addChild(pBattleAnimationSprite);
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
                    pSprite = new oxygine::Sprite();
                    pSprite->setPosition(defenseX, defenseY);
                    pSprite->setResAnim(pAnim);
                    pSprite->setScale(1.5f);
                    pSprite->setPriority(4);
                    m_pCursorInfoBox->addChild(pSprite);
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

                qint32 y2 = 120;

                qint32 yAdvance = 2;

                pTextfield = new Label(width);
                pTextfield->setPosition(10, y2);
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
                y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                // show building information
                if (pBuilding.get() != nullptr)
                {
                    if (pBuilding->getOwner() != nullptr &&
                        visionHide == GameEnums::VisionType_Clear)
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText((tr("Owner: Player ") + QString::number(pBuilding->getOwner()->getPlayerID() + 1)));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }
                    else
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText(tr("Owner: Neutral"));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }
                    if (pUnit.get() == nullptr)
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText((tr("Resistance: ") + QString::number(20)));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }
                    else
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText((tr("Resistance: ") + QString::number(20 - pUnit->getCapturePoints())));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }
                }
                // show unit information
                if (pUnit.get() != nullptr)
                {
                    pTextfield = new Label(width);
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText(pUnit->getName());
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() + yAdvance;

                    pTextfield = new Label(width);
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText((tr("Owner: Player ") + QString::number(pUnit->getOwner()->getPlayerID() + 1)));
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() + yAdvance;

                    if (!pUnit->getWeapon1ID().isEmpty())
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText((tr("Weapon 1: ") +
                                                 WeaponManager::getInstance()->getName(pUnit->getWeapon1ID())));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }

                    if (!pUnit->getWeapon2ID().isEmpty())
                    {
                        pTextfield = new Label(width);
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(smallStyle);
                        pTextfield->setHtmlText((tr("Weapon 2: ") +
                                                 WeaponManager::getInstance()->getName(pUnit->getWeapon2ID())));
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                    }

                    pTextfield = new Label(width);
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText((tr("Move: ") +
                                             MovementTableManager::getInstance()->getName(pUnit->getMovementType())));
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() + yAdvance;

                    pTextfield = new Label(width);
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText((tr("Movepoints: ") +
                                             QString::number(pUnit->getMovementpoints(QPoint(x, y)))));
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() + yAdvance;

                    pTextfield = new Label(width);
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(smallStyle);
                    pTextfield->setHtmlText((tr("Vision: ") +
                                             QString::number(pUnit->getVision(QPoint(pUnit->getX(), pUnit->getY())))));
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() + yAdvance;

                    if (pUnit->getLoadingPlace() > 0)
                    {
                        if (!pUnit->getTransportHidden(pPlayer))
                        {
                            pTextfield = new Label(width);
                            pTextfield->setPosition(10, y2);
                            pTextfield->setStyle(smallStyle);
                            pTextfield->setHtmlText(tr("Loaded Units"));
                            m_pCursorInfoBox->addChild(pTextfield);
                            y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                            for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
                            {
                                pTextfield = new Label(width);
                                pTextfield->setPosition(10, y2);
                                pTextfield->setStyle(smallStyle);
                                pTextfield->setHtmlText((pUnit->getLoadedUnit(i)->getName()));
                                m_pCursorInfoBox->addChild(pTextfield);
                                y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                            }
                        }
                        else
                        {
                            pTextfield = new Label(width);
                            pTextfield->setPosition(10, y2);
                            pTextfield->setStyle(smallStyle);
                            pTextfield->setHtmlText(tr("Loaded Units: ?"));
                            m_pCursorInfoBox->addChild(pTextfield);
                            y2 += pTextfield->getTextRect().getHeight() + yAdvance;
                        }
                    }
                }
            }
        }
    }
    pApp->continueThread();
}
