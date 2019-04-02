#include "ingameinfobar.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

IngameInfoBar::IngameInfoBar()
    : QObject()
{
    qint32 width = 300;
    qint32 cursorInfoHeigth = 300;
    qint32 gameInfoHeigth = 300;
    Mainapp* pApp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pMiniMapBox = new oxygine::Box9Sprite();
    pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setResAnim(pAnim);
    pMiniMapBox->setPosition(pApp->getSettings()->getWidth() - width, 0);
    pMiniMapBox->setSize(width, pApp->getSettings()->getHeight() - cursorInfoHeigth - gameInfoHeigth);
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
    m_pGameInfoBox->setPosition(pApp->getSettings()->getWidth() - width, pApp->getSettings()->getHeight() - cursorInfoHeigth - gameInfoHeigth);
    m_pGameInfoBox->setSize(width, gameInfoHeigth);
    m_pGameInfoBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(m_pGameInfoBox);


    oxygine::spBox9Sprite pCursorInfoBox = new oxygine::Box9Sprite();
    pCursorInfoBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pCursorInfoBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pCursorInfoBox->setResAnim(pAnim);
    pCursorInfoBox->setPosition(pApp->getSettings()->getWidth() - width, pApp->getSettings()->getHeight() - cursorInfoHeigth);
    pCursorInfoBox->setSize(width, cursorInfoHeigth);
    pCursorInfoBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pCursorInfoBox);
}

void IngameInfoBar::updatePlayerInfo()
{
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
    pTextfield->setText((tr("Buildings: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 100);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getUnitCount();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText((tr("Units: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 135);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getFonds();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText((tr("Fonds: ") + QString::number(count)).toStdString().c_str());
    pTextfield->setPosition(10, 170);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getPlayerID();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText((tr("Player: ") + QString::number(count + 1)).toStdString().c_str());
    pTextfield->setPosition(10, 205);
    m_pGameInfoBox->addChild(pTextfield);

    count = pPlayer->getTeam();
    pTextfield = new oxygine::TextField();
    pTextfield->setStyle(style);
    pTextfield->setText((tr("Team: ") + QString::number(count + 1)).toStdString().c_str());
    pTextfield->setPosition(10, 240);
    m_pGameInfoBox->addChild(pTextfield);
}

void IngameInfoBar::updateMinimap()
{
    m_pMinimap->updateMinimap(GameMap::getInstance(), true);
}

void IngameInfoBar::updateCursorInfo(qint32 x, qint32 y)
{
}
