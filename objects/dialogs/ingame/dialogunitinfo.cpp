#include "objects/dialogs/ingame/dialogunitinfo.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/gamemap.h"
#include "gameinput/humanplayerinput.h"

#include "objects/base/panel.h"

#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

DialogUnitInfo::DialogUnitInfo(Player* pPlayer)
    : QObject()
{
    setObjectName("DialogUnitInfo");
    spGameMap pMap = GameMap::getInstance();

    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    // ok button
    oxygine::spButton pOkButton = pObjectManager->createButton(tr("Ok"), 150);
    pOkButton->setPosition(Settings::getWidth() / 2 - pOkButton->getWidth() / 2, Settings::getHeight() - 30 - pOkButton->getHeight());
    pSpriteBox->addChild(pOkButton);
    pOkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        detach();
    });
    spPanel pPanel = spPanel::create(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 150),
                               QSize(Settings::getWidth() - 60, Settings::getHeight() - 150));
    pPanel->setPosition(30, 70);
    pSpriteBox->addChild(pPanel);

    qint32 y = 30;

    spLabel pText = spLabel::create(140);
    pText->setStyle(style);
    pText->setHtmlText("HP");
    pText->setPosition(160 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);


    pText = spLabel::create(140);
    pText->setStyle(style);
    pText->setHtmlText("Fuel");
    pText->setPosition(310 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(140);
    pText->setStyle(style);
    pText->setHtmlText("Ammo 1");
    pText->setPosition(460 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    pText = spLabel::create(140);
    pText->setStyle(style);
    pText->setHtmlText("Ammo 2");
    pText->setPosition(610 + pPanel->getX(), y);
    pSpriteBox->addChild(pText);

    y += 40;
    spQmlVectorUnit pUnits = pPlayer->getUnits();

    y = 10;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        pText = spLabel::create(140);
        pText->setStyle(style);
        pText->setHtmlText(QString::number(i + 1));
        pText->setPosition(10, y);
        pPanel->addItem(pText);
        Unit* pUnit = pUnits->at(i);
        Terrain* pTerrain = pMap->getTerrain(pUnit->Unit::getX(), pUnit->Unit::getY());
        spTerrain pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pActor->loadSprites();
        Building* pBuilding = pTerrain->getBuilding();
        if (pBuilding != nullptr)
        {
            spBuilding pTerrainBuilding = spBuilding::create(pBuilding->getBuildingID());
            pTerrainBuilding->setOwner(pBuilding->getOwner());
            pTerrainBuilding->scaleAndShowOnSingleTile();
            pActor->addChild(pTerrainBuilding);
        }

        spUnit pDummy = spUnit::create(pUnit->getUnitID(), pUnit->getOwner(), false);
        pDummy->setHasMoved(pUnit->getHasMoved());
        pActor->addChild(pDummy);
        pActor->setPosition(100, y + 8);
        pPanel->addItem(pActor);

        pText = spLabel::create(140);
        pText->setStyle(style);
        pText->setHtmlText(QString::number(pUnit->getHpRounded()));
        pText->setPosition(150, y);
        pPanel->addItem(pText);

        pText = spLabel::create(140);
        pText->setStyle(style);
        if (pUnit->getMaxFuel() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getFuel()) + "/" + QString::number(pUnit->getMaxFuel()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setPosition(300, y);
        pPanel->addItem(pText);

        pText = spLabel::create(140);
        pText->setStyle(style);
        if (pUnit->getMaxAmmo1() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getAmmo1()) + "/" + QString::number(pUnit->getMaxAmmo1()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setPosition(450, y);
        pPanel->addItem(pText);

        pText = spLabel::create(140);
        pText->setStyle(style);
        if (pUnit->getMaxAmmo2() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getAmmo2()) + "/" + QString::number(pUnit->getMaxAmmo2()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setPosition(600, y);
        pPanel->addItem(pText);

        oxygine::spButton pButton = ObjectManager::createButton(tr("Go to Unit"));
        pButton->setPosition(750, y);
        qint32 posX = pUnit->Unit::getX();
        qint32 posY = pUnit->Unit::getY();
        pButton->addClickListener([=](oxygine::Event*)
        {
            emit sigMoveToUnit(posX, posY);
        });
        pPanel->addItem(pButton);
        y += 40;
    }
    pPanel->setContentWidth(950);
    pPanel->setContentHeigth(y + 40);

    connect(this, &DialogUnitInfo::sigMoveToUnit, this, &DialogUnitInfo::moveToUnit, Qt::QueuedConnection);
}

void DialogUnitInfo::moveToUnit(qint32 posX, qint32 posY)
{
    
    spGameMenue pGamemenu = GameMenue::getInstance();
    if (pGamemenu.get() != nullptr)
    {
        pGamemenu->MoveMap(posX, posY);
        pGamemenu->calcNewMousePosition(posX, posY);
    }
    emit sigFinished();
    oxygine::Actor::detach();
}
