#include "unitinfo.h"

#include "resource_management/fontmanager.h"

#include "resource_management/movementtablemanager.h"

#include "resource_management/weaponmanager.h"

#include "resource_management/unitspritemanager.h"

#include "resource_management/terrainmanager.h"

#include "resource_management/buildingspritemanager.h"

#include "game/terrain.h"
#include "game/building.h"
#include "game/gamemap.h"

UnitInfo::UnitInfo(Unit* pUnit, qint32 width)
    : QObject()
{
    this->setWidth(width);

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    QString name = pUnit->getName();
    QString description = pUnit->getDescription();
    // no the fun begins create checkboxes and stuff and a panel down here
    qint32 y = 0;
    oxygine::spTextField pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText((tr("Unit Information ") + name).toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), 0);
    addChild(pLabel);
    y += 60;

    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(description.toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    y += 20 + pLabel->getTextRect().getHeight();

    qint32 xOffset = 200;

    // fire range
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if ((pUnit->getWeapon1ID() != "" && pWeaponManager->existsWeapon(pUnit->getWeapon1ID())) ||
        (pUnit->getWeapon2ID() != "" && pWeaponManager->existsWeapon(pUnit->getWeapon2ID())))
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Firerange:").toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getMinRange()) + " - " + QString::number(pUnit->getBaseMaxRange())).toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
    }

    // movement
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Movementpoints:").toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseMovementPoints())).toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // vision
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Vision:").toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseVision())).toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // fuel
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Fuel:").toStdString().c_str());
    pLabel->setScale(1.0f);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = new oxygine::TextField();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    if (pUnit->getMaxFuel() > 0)
    {
        pLabel->setHtmlText((QString::number(pUnit->getFuel()) + " / " + QString::number(pUnit->getMaxFuel())).toStdString().c_str());
    }
    else
    {
        pLabel->setHtmlText("- / -");
    }
    pLabel->setScale(1.0f);
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += 40;

    // ammo 1
    if (pUnit->getMaxAmmo1() > 0)
    {
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 1:").toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo1()) + " / " + QString::number(pUnit->getMaxAmmo1())).toStdString().c_str());
        pLabel->setScale(1.0f);
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
        pLabel->setHtmlText(tr("Ammo 2:").toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo2()) + " / " + QString::number(pUnit->getMaxAmmo2())).toStdString().c_str());
        pLabel->setScale(1.0f);
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
        pLabel->setHtmlText(tr("Loading Place:").toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getLoadedUnitCount()) + " / " + QString::number(pUnit->getLoadingPlace())).toStdString().c_str());
        pLabel->setScale(1.0f);
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += 40;
    }

    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    QString id = pUnit->getMovementType();
    name = pMovementTableManager->getMovementName(id);
    pLabel = new oxygine::TextField();
    pLabel->setStyle(style);
    pLabel->setHtmlText((tr("Movement ") + name).toStdString().c_str());
    pLabel->setScale(2.0f);
    pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), y);
    addChild(pLabel);
    y += 80;
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    qint32 x = 0;
    for (qint32 i = 0; i < pTerrainManager->getTerrainCount(); i++)
    {
        spTerrain pTerrain = Terrain::createTerrain(pTerrainManager->getTerrainID(i), -1, -1, "");
        pTerrain->loadSprites();
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get());
        pTerrain->setPosition(x, y);
        addChild(pTerrain);

        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs > 0)
        {
            pLabel->setHtmlText(QString::number(static_cast<qint32>(costs)).toStdString().c_str());
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setScale(1.0f);
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 80;
        if (x + 90 > width)
        {
            x = 0;
            y += 40;
        }
    }
    for (qint32 i = 0; i < pBuildingSpriteManager->getBuildingCount(); i++)
    {
        spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
        pTerrain->loadSprites();
        Building* pBuilding = new Building(pBuildingSpriteManager->getBuildingID(i));
        // pBuilding->setOwner(pUnit->getOwner());
        pBuilding->updateBuildingSprites(false);
        qint32 buildingWidth = pBuilding->getBuildingWidth();
        qint32 buildingHeigth = pBuilding->getBuildingHeigth();
        pBuilding->setScaleX(1.0f / static_cast<float>(buildingWidth));
        pBuilding->setScaleY(1.0f / static_cast<float>(buildingHeigth));
        if (buildingWidth > 1)
        {
            pBuilding->setX(GameMap::Imagesize * (buildingWidth - 1) / (buildingWidth));
        }
        if (buildingHeigth > 1)
        {
            pBuilding->setY(GameMap::Imagesize * (buildingHeigth - 1) / (buildingHeigth));
        }
        pTerrain->setBuilding(pBuilding);
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get());
        pTerrain->setPosition(x, y);
        addChild(pTerrain);
        pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (costs > 0)
        {
            pLabel->setHtmlText(QString::number(static_cast<qint32>(costs)).toStdString().c_str());
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setScale(1.0f);
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 80;
        if (x + 90 > width && i < pBuildingSpriteManager->getBuildingCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
    y += 40;

    if (pUnit->getWeapon1ID() != "" && pWeaponManager->existsWeapon(pUnit->getWeapon1ID()))
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setHtmlText((tr("Weapon 1 ") + pWeaponManager->getWeaponName(pUnit->getWeapon1ID())).toStdString().c_str());
        pLabel->setScale(2.0f);
        pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), y);
        addChild(pLabel);
        y += 80;
        createWeaponTable(pUnit, pUnit->getWeapon1ID(), y, width);
        y += 40;
    }
    if (pUnit->getWeapon2ID() != "" && pWeaponManager->existsWeapon(pUnit->getWeapon2ID()))
    {
        pLabel = new oxygine::TextField();
        pLabel->setStyle(style);
        pLabel->setHtmlText((tr("Weapon 2 ") + pWeaponManager->getWeaponName(pUnit->getWeapon2ID())).toStdString().c_str());
        pLabel->setScale(2.0f);
        pLabel->setPosition(width / 2 - pLabel->getTextRect().getWidth(), y);
        addChild(pLabel);
        y += 80;
        createWeaponTable(pUnit, pUnit->getWeapon2ID(), y, width);
        y += 40;
    }
    setHeight(y);
}

void UnitInfo::createWeaponTable(Unit* pUnit, QString weaponID, qint32& y, qint32 width)
{
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 x = 0;
    for (qint32 i = 0; i < pUnitSpriteManager->getUnitCount(); i++)
    {
        spUnit pDummy = new Unit(pUnitSpriteManager->getUnitID(i), pUnit->getOwner(), false);
        float damage = pWeaponManager->getBaseDamage(weaponID, pDummy.get());
        pDummy->setPosition(x, y);
        addChild(pDummy);
        oxygine::spTextField pLabel = new oxygine::TextField();
        pLabel->setWidth(width);
        pLabel->setStyle(style);
        if (damage > 0)
        {
            pLabel->setHtmlText((QString::number(static_cast<qint32>(damage))  + " %").toStdString().c_str());
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setScale(1.0f);
        pLabel->setPosition(x + 30, y - 5);
        addChild(pLabel);
        x += 100;
        if (x + 110 > width && i < pUnitSpriteManager->getUnitCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
}
