#include "wiki/unitinfo.h"
#include "wiki/wikidatabase.h"

#include "resource_management/fontmanager.h"
#include "resource_management/movementtablemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/gamemanager.h"

#include "game/terrain.h"
#include "game/building.h"
#include "game/gamemap.h"
#include "game/battleanimationsprite.h"
#include "game/gameaction.h"

UnitInfo::UnitInfo(Unit* pUnit, qint32 width)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);


    this->setWidth(width);

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    oxygine::TextStyle headerStyle = FontManager::getMainFont48();
    headerStyle.color = FontManager::getFontColor();
    headerStyle.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = true;

    QString name = pUnit->getName();
    QString description = pUnit->getDescription();
    // no the fun begins create checkboxes and stuff and a panel down here
    qint32 y = 0;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText((tr("Unit Information ") + name));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, 0);
    addChild(pLabel);
    y += 60;

    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(description);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    y += 20 + pLabel->getTextRect().getHeight();

    qint32 yStart = y;
    qint32 xOffset = 230;

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel_transparent");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setPosition(width - 210, y);
    this->addChild(pSpriteBox);

    spBattleAnimationSprite pBattleAnimationSprite = new BattleAnimationSprite(pUnit, nullptr, BattleAnimationSprite::standingAnimation);
    pBattleAnimationSprite->setPosition(pSpriteBox->getX() + 7, pSpriteBox->getY() + 5);
    pSpriteBox->setSize(pBattleAnimationSprite->getWidth() + 14, pBattleAnimationSprite->getHeight() + 12);
    addChild(pBattleAnimationSprite);

    // movement
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Unit Type:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(UnitSpriteManager::getUnitTypeText(pUnit->getUnitType()));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // fire range
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if ((pUnit->getWeapon1ID() != "" && pWeaponManager->exists(pUnit->getWeapon1ID())) ||
        (pUnit->getWeapon2ID() != "" && pWeaponManager->exists(pUnit->getWeapon2ID())))
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Firerange:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getBaseMinRange()) + " - " + QString::number(pUnit->getBaseMaxRange())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
    }

    // movement
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Movepoints: "));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseMovementPoints())));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // vision
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Vision:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseVision())));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // fuel
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Fuel:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    if (pUnit->getMaxFuel() > 0)
    {
        pLabel->setHtmlText((QString::number(pUnit->getFuel()) + " / " + QString::number(pUnit->getMaxFuel())));
    }
    else
    {
        pLabel->setHtmlText("- / -");
    }
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // ammo 1
    if (pUnit->getMaxAmmo1() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 1:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo1()) + " / " + QString::number(pUnit->getMaxAmmo1())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
    }
    // ammo 2
    if (pUnit->getMaxAmmo2() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 2:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo2()) + " / " + QString::number(pUnit->getMaxAmmo2())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
    }

    // loading place 2
    if (pUnit->getLoadingPlace() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Loading Place:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getLoadedUnitCount()) + " / " + QString::number(pUnit->getLoadingPlace())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
        pLabel = new oxygine::TextField();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText(tr("Loadable Units"));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
        addChild(pLabel);
        y += 80;
        QStringList loadingUnits = pUnit->getTransportUnits();
        createLoadingTable(pUnit, loadingUnits, y, width);
        y += 40;

        if (pUnit->getLoadedUnitCount() > 0)
        {
            pLabel = new oxygine::TextField();
            pLabel->setStyle(headerStyle);
            pLabel->setHtmlText(tr("Loaded Units"));
            pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
            addChild(pLabel);
            y += 80;
            createLoadedUnits(pUnit, y, width);
            y += 40;
        }
    }

    pLabel = new oxygine::TextField();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Actions"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
    addChild(pLabel);
    y += 80;
    createActionTable(pUnit, y, width);
    y += 40;

    pLabel = new oxygine::TextField();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Transporters"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
    addChild(pLabel);
    y += 80;
    createTransportTable(pUnit, y, width);
    y += 40;

    if (y - yStart < 210)
    {
        y = yStart + 210;
    }
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    QString id = pUnit->getMovementType();
    name = pMovementTableManager->getName(id);
    pLabel = new oxygine::TextField();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText((tr("Movement ") + name));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
    addChild(pLabel);
    y += 80;
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    qint32 x = 0;
    for (const auto& terrainId : sortedTerrains)
    {
        spTerrain pTerrain = Terrain::createTerrain(terrainId, -1, -1, "");
        pTerrain->loadSprites();
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get(), pTerrain.get(), pUnit);
        pTerrain->setPosition(x, y);
        pTerrain->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(terrainId);
        });
        addChild(pTerrain);

        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs >= 0)
        {
            pLabel->setHtmlText(QString::number(static_cast<qint32>(costs)));
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 80;
        if (x + 90 > width)
        {
            x = 0;
            y += 40;
        }
    }
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
        pTerrain->loadSprites();
        Building* pBuilding = new Building(pBuildingSpriteManager->getID(i));
        // pBuilding->setOwner(pUnit->getOwner());
        pBuilding->updateBuildingSprites(false);
        qint32 buildingWidth = pBuilding->getBuildingWidth();
        qint32 buildingHeigth = pBuilding->getBuildingHeigth();
        pBuilding->setScaleX(1.0f / static_cast<float>(buildingWidth));
        pBuilding->setScaleY(1.0f / static_cast<float>(buildingHeigth));
        if (buildingWidth > 1)
        {
            pBuilding->setX(GameMap::getImageSize() * (buildingWidth - 1) / (buildingWidth));
        }
        if (buildingHeigth > 1)
        {
            pBuilding->setY(GameMap::getImageSize() * (buildingHeigth - 1) / (buildingHeigth));
        }
        pTerrain->setBuilding(pBuilding);
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get(), pTerrain.get(), pUnit);
        pTerrain->setPosition(x, y);
        pTerrain->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(pBuildingSpriteManager->getID(i));
        });
        addChild(pTerrain);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs > 0)
        {
            pLabel->setHtmlText(QString::number(static_cast<qint32>(costs)));
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 80;
        if (x + 90 > width && i < pBuildingSpriteManager->getCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
    y += 40;

    if (pUnit->getWeapon1ID() != "" && pWeaponManager->exists(pUnit->getWeapon1ID()))
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText((tr("Weapon 1 ") + pWeaponManager->getName(pUnit->getWeapon1ID())));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
        addChild(pLabel);
        y += 80;
        createWeaponTable(pUnit, pUnit->getWeapon1ID(), y, width);
        y += 40;
    }
    if (pUnit->getWeapon2ID() != "" && pWeaponManager->exists(pUnit->getWeapon2ID()))
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText((tr("Weapon 2 ") + pWeaponManager->getName(pUnit->getWeapon2ID())));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth() / 2, y);
        addChild(pLabel);
        y += 80;
        createWeaponTable(pUnit, pUnit->getWeapon2ID(), y, width);
        y += 40;
    }
    setHeight(y);
    connect(this, &UnitInfo::sigShowLink, this, &UnitInfo::showLink, Qt::QueuedConnection);
}

void UnitInfo::createWeaponTable(Unit* pUnit, QString weaponID, qint32& y, qint32 width)
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 x = 0;
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (qint32 i = 0; i < sortedUnits.size(); i++)
    {
        spUnit pDummy = new Unit(sortedUnits[i], pUnit->getOwner(), false);
        pDummy->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(sortedUnits[i]);
        });
        float damage = pWeaponManager->getBaseDamage(weaponID, pDummy.get());
        pDummy->setPosition(x, y);
        addChild(pDummy);
        oxygine::spTextField pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (damage > 0)
        {
            pLabel->setHtmlText((QString::number(static_cast<qint32>(damage))  + " %"));
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 140;
        if (x + 150 > width && i < pUnitSpriteManager->getCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
}

void UnitInfo::createLoadingTable(Unit* pUnit, QStringList loadables, qint32& y, qint32 width)
{
    qint32 x = 0;
    for (const auto& unitID : loadables)
    {
        spUnit pDummy = new Unit(unitID, pUnit->getOwner(), false);
        pDummy->setPosition(x, y);
        pDummy->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(unitID);
        });
        addChild(pDummy);
        x += GameMap::getImageSize() * 1.5f;
        if (x + GameMap::getImageSize() * 1.5f > width)
        {
            x = 0;
            y += 40;
        }
    }
}

void UnitInfo::createLoadedUnits(Unit* pUnit, qint32& y, qint32 width)
{
    qint32 x = 0;
    for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
    {
        spUnit loadedUnit = pUnit->getLoadedUnit(i);
        spUnit pDummy = new Unit(loadedUnit->getUnitID(), pUnit->getOwner(), false);
        pDummy->setPosition(x, y);
        pDummy->setHasMoved(loadedUnit->getHasMoved());
        pDummy->setHp(loadedUnit->getHp());
        pDummy->setAmmo1(loadedUnit->getAmmo1());
        pDummy->setAmmo2(loadedUnit->getAmmo2());
        pDummy->setFuel(loadedUnit->getFuel());
        pDummy->setUnitRank(loadedUnit->getUnitRank());
        pDummy->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(loadedUnit->getUnitID());
        });
        addChild(pDummy);
        x += GameMap::getImageSize() * 1.5f;
        if (x + GameMap::getImageSize() * 1.5f > width)
        {
            x = 0;
            y += 40;
        }
    }
}

void UnitInfo::createTransportTable(Unit* pUnit, qint32& y, qint32 width)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    qint32 x = 0;
    for (const auto& unitID : sortedUnits)
    {
        spUnit pDummy = new Unit(unitID, pUnit->getOwner(), false);
        if (pDummy->canTransportUnit(pUnit, true))
        {
            pDummy->setPosition(x, y);
            pDummy->addClickListener([=](oxygine::Event*)
            {
                emit sigShowLink(unitID);
            });
            addChild(pDummy);
            x += GameMap::getImageSize() * 1.5f;
            if (x + GameMap::getImageSize() * 1.5f > width)
            {
                x = 0;
                y += 40;
            }
        }
    }
}

void UnitInfo::createActionTable(Unit* pUnit, qint32& y, qint32 width)
{
    qint32 x = 0;
    QStringList actions = pUnit->getActionList();
    for (const auto & action : actions)
    {
        // QString text = GameAction::getActionText(action);
        QString icon = GameAction::getActionIcon(action);
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        oxygine::spSprite pSprite = pWikiDatabase->getIcon(icon, GameMap::getImageSize());
        pSprite->setPosition(x, y);
        pSprite->addClickListener([=](oxygine::Event*)
        {
            emit sigShowLink(action);
        });
        addChild(pSprite);
        x += GameMap::getImageSize() * 1.5f;
        if (x + GameMap::getImageSize() * 1.5f > width)
        {
            x = 0;
            y += 40;
        }
    }
}

void UnitInfo::showLink(QString pageID)
{
    WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
    oxygine::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(pageID)));
    
}
