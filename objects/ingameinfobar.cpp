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

    pMiniMapBox->setSize(width, pApp->getSettings()->getHeight() - cursorInfoHeigth - gameInfoHeigth);
    pMiniMapBox->setPosition(0, 0);
    if (pApp->getSettings()->getHeight() - cursorInfoHeigth - gameInfoHeigth < 100)
    {
        pMiniMapBox->setHeight(100);
        setScale(pApp->getSettings()->getHeight() / static_cast<float>(100 + cursorInfoHeigth + gameInfoHeigth));
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

    setX(pApp->getSettings()->getWidth() - getScaledWidth());
    addChild(m_pCursorInfoBox);
}

void IngameInfoBar::updatePlayerInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pGameInfoBox->removeChildren();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    GameMap* pMap = GameMap::getInstance();
    Player* pPlayer = pMap->getCurrentPlayer();
    oxygine::spSprite pSprite = new oxygine::Sprite();
    CO* pCO = pPlayer->getCO(0);
    oxygine::ResAnim* pAnim = nullptr;
    if (pCO != nullptr)
    {
        pAnim = pCOSpriteManager->getResAnim((pCO->getCoID() + "+face").toStdString().c_str());
    }
    else
    {
        pAnim = pCOSpriteManager->getResAnim("no_co+face");
    }
    pSprite->setScale(1.8f);
    pSprite->setResAnim(pAnim);
    pSprite->setPosition(10, 10);
    m_pGameInfoBox->addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        pAnim = pCOSpriteManager->getResAnim((pCO->getCoID() + "+face").toStdString().c_str());
    }
    else
    {
        pAnim = pCOSpriteManager->getResAnim("no_co+face");
    }
    pSprite->setResAnim(pAnim);
    pSprite->setPosition(210, 10);
    pSprite->setScale(1.8f);
    m_pGameInfoBox->addChild(pSprite);

    pSprite = new oxygine::Sprite();
    pAnim = pGameManager->getResAnim((pMap->getGameRules()->getCurrentWeather()->getWeatherSymbol()).toStdString().c_str());
    pSprite->setResAnim(pAnim);
    pSprite->setPosition(100, 10);
    pSprite->setScale(1.4f);
    m_pGameInfoBox->addChild(pSprite);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    qint32 count = pPlayer->getBuildingCount();
    oxygine::spTextField pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText((tr("Buildings: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 100);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getUnitCount();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText((tr("Units: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 135);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getFunds();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText((tr("Funds: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 170);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getPlayerID();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText((tr("Player: ") + QString::number(count + 1)).toStdString().c_str());
    pTextfield->setPosition(10, 205);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getTeam();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setHtmlText((tr("Team: ") + QString::number(count + 1)).toStdString().c_str());
    pTextfield->setPosition(10, 240);
    m_pGameInfoBox->addChild(pTextfield);
    pApp->continueThread();
}

void IngameInfoBar::updateMinimap()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pMinimap->updateMinimap(GameMap::getInstance(), true);
    pApp->continueThread();

}

void IngameInfoBar::updateCursorInfo(qint32 x, qint32 y)
{
    updateTerrainInfo(x, y, false);
}

void IngameInfoBar::updateTerrainInfo(qint32 x, qint32 y, bool update)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y) && (m_LastX != x || m_LastY != y || update))
    {
        m_LastX = x;
        m_LastY = y;
        m_pCursorInfoBox->removeChildren();
        Player* pPlayer = pMap->getCurrentViewPlayer();
        Terrain* pTerrain = pMap->getTerrain(x, y);
        Building* pBuilding = pTerrain->getBuilding();
        Unit* pUnit = pTerrain->getUnit();
        if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
        {
            pUnit = nullptr;
        }
        GameManager* pGameManager = GameManager::getInstance();
        ObjectManager* pObjectManager = ObjectManager::getInstance();
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::ResAnim* pAnim = nullptr;
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = false;


        // draw building hp
        oxygine::spTextField pTextfield = new oxygine::TextField();
        qint32 hp = 0;
        if ((pBuilding != nullptr) && (pBuilding->getHp() > 0))
        {
            hp = pBuilding->getHp();
        }
        else if ((pTerrain->getHp() > 0))
        {
            hp = pTerrain->getHp();
        }
        if (hp > 0)
        {
            pAnim = pObjectManager->getResAnim("barforeground");
            pTextfield->setStyle(style);
            qint32 hpMax = 100;
            if (hp > 100)
            {
                hpMax = hp;
            }
            pTextfield->setHtmlText((tr("HP: ") + QString::number(hp) + "/" + QString::number(hpMax)).toStdString().c_str());
            pTextfield->setPosition(10, 10);
            m_pCursorInfoBox->addChild(pTextfield);
            oxygine::spColorRectSprite pColorBar = new oxygine::ColorRectSprite();
            float divider = static_cast<float>(hp) / static_cast<float>(hpMax);
            if (divider > 2.0f / 3.0f)
            {
                pColorBar->setColor(0, 255, 0, 255);
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
        if (pUnit != nullptr)
        {
            bool HpHidden = pUnit->getHpHidden(pMap->getCurrentViewPlayer());
            float count = pUnit->getHp();
            qint32 hpRounded = pUnit->getHpRounded();
            float countMax = 10.0f;
            pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            if (HpHidden)
            {
                pTextfield->setHtmlText((tr("HP: ") + "?/10").toStdString().c_str());
            }
            else
            {
                pTextfield->setHtmlText((tr("HP: ") + QString::number(hpRounded) + "/" + QString::number(countMax, 'f', 0)).toStdString().c_str());
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
                    pColorBar->setColor(0, 255, 0, 255);
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
            pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            if (countMaxInt > 0)
            {
                pTextfield->setHtmlText((tr("Ammo1: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)).toStdString().c_str());
                pColorBar = new oxygine::ColorRectSprite();
                divider = static_cast<float>(countInt) / static_cast<float>(countMaxInt);
                pColorBar->setColor(139, 69, 19, 255);
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
                pTextfield->setHtmlText(tr("Ammo1: -/-").toStdString().c_str());
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
            pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            if (countMaxInt > 0)
            {
                pTextfield->setHtmlText((tr("Ammo2: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)).toStdString().c_str());
                pColorBar = new oxygine::ColorRectSprite();
                divider = static_cast<float>(countInt) / static_cast<float>(countMaxInt);
                pColorBar->setColor(255, 255, 0, 255);
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
                pTextfield->setHtmlText(tr("Ammo2: -/-").toStdString().c_str());
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
            pTextfield = new oxygine::TextField();
            pTextfield->setStyle(style);
            if (countMaxInt > 0)
            {
                pTextfield->setHtmlText((tr("Fuel: ") + QString::number(countInt) + "/" + QString::number(countMaxInt)).toStdString().c_str());

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
                pTextfield->setHtmlText(tr("Fuel: -/-").toStdString().c_str());
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
        if (pUnit != nullptr)
        {
            pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground().toStdString().c_str(), oxygine::ep_ignore_error);
        }
        else if (pBuilding != nullptr)
        {
            pAnimBase = pGameManager->getResAnim(pBuilding->getTerrainAnimationBase().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimFore = pGameManager->getResAnim(pBuilding->getTerrainAnimationForeground().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimBack = pGameManager->getResAnim(pBuilding->getTerrainAnimationBackground().toStdString().c_str(), oxygine::ep_ignore_error);
        }
        else
        {
            pAnimBase = pGameManager->getResAnim(pTerrain->getTerrainAnimationBase().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimFore = pGameManager->getResAnim(pTerrain->getTerrainAnimationForeground().toStdString().c_str(), oxygine::ep_ignore_error);
            pAnimBack = pGameManager->getResAnim(pTerrain->getTerrainAnimationBackground().toStdString().c_str(), oxygine::ep_ignore_error);
        }
        pSprite = new oxygine::Sprite();
        pSprite->setPosition(m_pCursorInfoBox->getWidth() - pAnimBase->getWidth() - 10, 110);
        pSprite->setResAnim(pAnimBase);
        m_pCursorInfoBox->addChild(pSprite);
        pSprite = new oxygine::Sprite();
        pSprite->setPosition(m_pCursorInfoBox->getWidth() - pAnimBase->getWidth() - 10, 110);
        pSprite->setResAnim(pAnimFore);
        pSprite->setPriority(1);
        m_pCursorInfoBox->addChild(pSprite);
        pSprite = new oxygine::Sprite();
        pSprite->setPosition(m_pCursorInfoBox->getWidth() - pAnimBase->getWidth() - 10, 110);
        pSprite->setResAnim(pAnimBack);
        pSprite->setPriority(2);
        m_pCursorInfoBox->addChild(pSprite);

        if (pUnit != nullptr)
        {
            BattleAnimationSprite* pBattleAnimationSprite = new BattleAnimationSprite(pUnit, pUnit->getTerrain(), BattleAnimationSprite::standingAnimation);
            pBattleAnimationSprite->setPosition(m_pCursorInfoBox->getWidth() - pAnimBase->getWidth() - 10, 110);
            pBattleAnimationSprite->setPriority(3);
            m_pCursorInfoBox->addChild(pBattleAnimationSprite);
        }

        qint32 TerrainDefense = 0;
        if (pUnit != nullptr)
        {
            TerrainDefense = pUnit->getTerrainDefense();
        }
        else
        {
            TerrainDefense = pTerrain->getBaseDefense();
        }
        float defenseY = pSprite->getY() + 5;
        float startDefenseX = pSprite->getX() + 5;
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

        float unitInfoScale = 0.7f;
        qint32 yAdvance = 2;

        pTextfield = new oxygine::TextField();
        pTextfield->setPosition(10, y2);
        pTextfield->setStyle(style);
        pTextfield->setScale(unitInfoScale);
        QString name = "";
        if (pBuilding != nullptr)
        {
            name = pBuilding->getName();
        }
        else
        {
            name = pTerrain->getTerrainName();
        }
        pTextfield->setHtmlText(name.toStdString().c_str());
        m_pCursorInfoBox->addChild(pTextfield);



        y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

        // show building information
        if (pBuilding != nullptr)
        {
            if (pBuilding->getOwner() != nullptr)
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText((tr("Owner: Player ") + QString::number(pBuilding->getOwner()->getPlayerID() + 1)).toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }
            else
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText(tr("Owner: Neutral").toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }
            if (pUnit == nullptr)
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText((tr("Resistance: ") +
                                     QString::number(20)).toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }
            else
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText((tr("Resistance: ") +
                                     QString::number(20 - pUnit->getCapturePoints())).toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }
        }
        // show unit information
        if (pUnit != nullptr)
        {
            pTextfield = new oxygine::TextField();
            pTextfield->setPosition(10, y2);
            pTextfield->setStyle(style);
            pTextfield->setScale(unitInfoScale);
            pTextfield->setHtmlText(pUnit->getName().toStdString().c_str());
            m_pCursorInfoBox->addChild(pTextfield);
            y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

            pTextfield = new oxygine::TextField();
            pTextfield->setPosition(10, y2);
            pTextfield->setStyle(style);
            pTextfield->setScale(unitInfoScale);
            pTextfield->setHtmlText((tr("Owner: Player ") + QString::number(pUnit->getOwner()->getPlayerID() + 1)).toStdString().c_str());
            m_pCursorInfoBox->addChild(pTextfield);
            y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

            if (!pUnit->getWeapon1ID().isEmpty())
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText((tr("Weapon 1: ") +
                                     WeaponManager::getInstance()->getWeaponName(pUnit->getWeapon1ID())).toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }

            if (!pUnit->getWeapon2ID().isEmpty())
            {
                pTextfield = new oxygine::TextField();
                pTextfield->setPosition(10, y2);
                pTextfield->setStyle(style);
                pTextfield->setScale(unitInfoScale);
                pTextfield->setHtmlText((tr("Weapon 2: ") +
                                     WeaponManager::getInstance()->getWeaponName(pUnit->getWeapon2ID())).toStdString().c_str());
                m_pCursorInfoBox->addChild(pTextfield);
                y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
            }

            pTextfield = new oxygine::TextField();
            pTextfield->setPosition(10, y2);
            pTextfield->setStyle(style);
            pTextfield->setScale(unitInfoScale);
            pTextfield->setHtmlText((tr("Move: ") +
                                 MovementTableManager::getInstance()->getMovementName(pUnit->getMovementType())).toStdString().c_str());
            m_pCursorInfoBox->addChild(pTextfield);
            y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

            pTextfield = new oxygine::TextField();
            pTextfield->setPosition(10, y2);
            pTextfield->setStyle(style);
            pTextfield->setScale(unitInfoScale);
            pTextfield->setHtmlText((tr("Movementpoints: ") +
                                 QString::number(pUnit->getMovementpoints(QPoint(x, y)))).toStdString().c_str());
            m_pCursorInfoBox->addChild(pTextfield);
            y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

            pTextfield = new oxygine::TextField();
            pTextfield->setPosition(10, y2);
            pTextfield->setStyle(style);
            pTextfield->setScale(unitInfoScale);
            pTextfield->setHtmlText((tr("Vision: ") +
                                 QString::number(pUnit->getVision(QPoint(pUnit->getX(), pUnit->getY())))).toStdString().c_str());
            m_pCursorInfoBox->addChild(pTextfield);
            y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;

            if (pUnit->getLoadingPlace() > 0)
            {
                if (!pUnit->getTransportHidden(pPlayer))
                {
                    pTextfield = new oxygine::TextField();
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(style);
                    pTextfield->setScale(unitInfoScale);
                    pTextfield->setHtmlText(tr("Loaded Units").toStdString().c_str());
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
                    for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
                    {
                        pTextfield = new oxygine::TextField();
                        pTextfield->setPosition(10, y2);
                        pTextfield->setStyle(style);
                        pTextfield->setScale(unitInfoScale);
                        pTextfield->setHtmlText((pUnit->getLoadedUnit(i)->getName()).toStdString().c_str());
                        m_pCursorInfoBox->addChild(pTextfield);
                        y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
                    }
                }
                else
                {
                    pTextfield = new oxygine::TextField();
                    pTextfield->setPosition(10, y2);
                    pTextfield->setStyle(style);
                    pTextfield->setScale(unitInfoScale);
                    pTextfield->setHtmlText(tr("Loaded Units: ?").toStdString().c_str());
                    m_pCursorInfoBox->addChild(pTextfield);
                    y2 += pTextfield->getTextRect().getHeight() * unitInfoScale + yAdvance;
                }
            }

        }
    }
    pApp->continueThread();
}
