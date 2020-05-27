#include "objects/dialogunitinfo.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/gamemap.h"
#include "gameinput/humanplayerinput.h"

#include "objects/panel.h"

#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

DialogUnitInfo::DialogUnitInfo(Player* pPlayer)
    : QObject()
{
    GameMap* pMap = GameMap::getInstance();

    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

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
        detach();
        emit sigFinished();
    });
    spPanel pPanel = new Panel(true, QSize(Settings::getWidth() - 60, Settings::getHeight() - 110),
                               QSize(Settings::getWidth() - 60, Settings::getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);

    qint32 y = 10;

    oxygine::spTextField pText = new oxygine::TextField();
    pText->setHtmlText("Fuel");
    pText->setStyle(style);
    pText->setPosition(150, y);
    pPanel->addItem(pText);

    pText = new oxygine::TextField();
    pText->setHtmlText("Ammo 1");
    pText->setStyle(style);
    pText->setPosition(300, y);
    pPanel->addItem(pText);

    pText = new oxygine::TextField();
    pText->setHtmlText("Ammo 2");
    pText->setStyle(style);
    pText->setPosition(450, y);
    pPanel->addItem(pText);

    y += 40;
    QmlVectorUnit* pUnits = pPlayer->getUnits();

    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        pText = new oxygine::TextField();
        pText->setHtmlText(QString::number(i + 1));
        pText->setStyle(style);
        pText->setPosition(10, y);
        pPanel->addItem(pText);
        Unit* pUnit = pUnits->at(i);
        Terrain* pTerrain = pMap->getTerrain(pUnit->getX(), pUnit->getY());
        spTerrain pActor = Terrain::createTerrain(pTerrain->getTerrainID(), -10, -10, "");
        pActor->loadSprites();
        Unit* pDummy = new Unit(pUnit->getUnitID(), pUnit->getOwner(), false);
        pDummy->setHasMoved(pUnit->getHasMoved());
        pActor->addChild(pDummy);
        pActor->setPosition(100, y + 8);
        pPanel->addItem(pActor);

        pText = new oxygine::TextField();
        if (pUnit->getMaxFuel() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getFuel()) + "/" + QString::number(pUnit->getMaxFuel()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setStyle(style);
        pText->setPosition(150, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        if (pUnit->getMaxAmmo1() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getAmmo1()) + "/" + QString::number(pUnit->getMaxAmmo1()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setStyle(style);
        pText->setPosition(300, y);
        pPanel->addItem(pText);

        pText = new oxygine::TextField();
        if (pUnit->getMaxAmmo2() > 0)
        {
            pText->setHtmlText(QString::number(pUnit->getAmmo2()) + "/" + QString::number(pUnit->getMaxAmmo2()));
        }
        else
        {
            pText->setHtmlText("-/-");
        }
        pText->setStyle(style);
        pText->setPosition(450, y);
        pPanel->addItem(pText);

        oxygine::spButton pButton = ObjectManager::createButton(tr("Go to Unit"));
        pButton->setPosition(600, y);
        qint32 posX = pUnit->getX();
        qint32 posY = pUnit->getY();
        pButton->addClickListener([=](oxygine::Event*)
        {
            emit sigMoveToUnit(posX, posY);
        });
        pPanel->addItem(pButton);
        y += 40;
    }
    delete pUnits;
    pPanel->setContentWidth(800);
    pPanel->setContentHeigth(y + 40);

    connect(this, &DialogUnitInfo::sigMoveToUnit, this, &DialogUnitInfo::moveToUnit, Qt::QueuedConnection);
}

void DialogUnitInfo::moveToUnit(qint32 posX, qint32 posY)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMenue::getInstance()->MoveMap(posX, posY);
    GameMenue::getInstance()->calcNewMousePosition(posX, posY);
    addRef();
    oxygine::Actor::detach();
    deleteLater();
    emit sigFinished();
    pApp->continueThread();
}
