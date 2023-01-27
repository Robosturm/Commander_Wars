#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "wiki/unitinfo.h"
#include "wiki/wikidatabase.h"
#include "wiki/wikipage.h"

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
#include "game/gameaction.h"

#include "objects/base/label.h"
#include "objects/base/dropdownmenusprite.h"

UnitInfo::UnitInfo(spUnit pUnit, qint32 width)
    : m_pUnit(pUnit)
{
#ifdef GRAPHICSUPPORT
    setObjectName("UnitInfo");
#endif
    Interpreter::setCppOwnerShip(this);

    setWidth(width);

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    oxygine::TextStyle headerStyle = oxygine::TextStyle(FontManager::getMainFont48());
    headerStyle.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    headerStyle.multiline = true;

    QString name = pUnit->getName();
    QString description = pUnit->getDescription();
    // no the fun begins create checkboxes and stuff and a panel down here
    qint32 y = 0;
    oxygine::spTextField pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText((tr("Unit Information ") + name));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, 0);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;

    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(description);
    pLabel->setPosition(0, y);
    addChild(pLabel);
    y += 20 + pLabel->getTextRect().height();

    qint32 yStart = y;
    qint32 xOffset = 230;

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel_transparent+mask");
    m_pSpriteBox = oxygine::spBox9Sprite::create();
    m_pSpriteBox->setResAnim(pAnim);
    m_pSpriteBox->setPosition(width - 210, y);
    m_pSpriteBox->setColorTable(pUnit->getOwner()->getColorTableAnim(), true);
    addChild(m_pSpriteBox);

    // load battle sprite data
    loadBattleSprite(pUnit.get());
    loadArmySelector(m_pSpriteBox->getX(), m_pSpriteBox->getY() + m_pSpriteBox->getScaledHeight() + 10);

    // movement
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Unit Type:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(UnitSpriteManager::getUnitTypeText(pUnit->getUnitType()));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;

    // fire range
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    if ((pUnit->getWeapon1ID() != "" && pWeaponManager->exists(pUnit->getWeapon1ID())) ||
        (pUnit->getWeapon2ID() != "" && pWeaponManager->exists(pUnit->getWeapon2ID())))
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Firerange:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getBaseMinRange()) + " - " + QString::number(pUnit->getBaseMaxRange())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
    }

    // movement
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Movepoints: "));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseMovementPoints())));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;

    // vision
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Vision:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getBaseVision())));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;

    // fuel
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Fuel:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
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
    y += pLabel->getTextRect().height() + 10;

    // ammo 1
    if (pUnit->getMaxAmmo1() > 0)
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 1:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo1()) + " / " + QString::number(pUnit->getMaxAmmo1())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
    }
    // ammo 2
    if (pUnit->getMaxAmmo2() > 0)
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Ammo 2:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getAmmo2()) + " / " + QString::number(pUnit->getMaxAmmo2())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
    }

    // cpsts
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText(tr("Costs:"));
    pLabel->setPosition(0, y);
    addChild(pLabel);
    pLabel = oxygine::spTextField::create();
    pLabel->setWidth(width - 10);
    pLabel->setStyle(style);
    pLabel->setHtmlText((QString::number(pUnit->getUnitCosts())));
    pLabel->setPosition(xOffset, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;

    // loading place 2
    if (pUnit->getLoadingPlace() > 0)
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText(tr("Loading Place:"));
        pLabel->setPosition(0, y);
        addChild(pLabel);
        pLabel = oxygine::spTextField::create();
        pLabel->setWidth(width - 10);
        pLabel->setStyle(style);
        pLabel->setHtmlText((QString::number(pUnit->getLoadedUnitCount()) + " / " + QString::number(pUnit->getLoadingPlace())));
        pLabel->setPosition(xOffset, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
        pLabel = oxygine::spTextField::create();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText(tr("Loadable Units"));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
        QStringList loadingUnits = pUnit->getTransportUnits();
        createLoadingTable(pUnit.get(), loadingUnits, y, width);
        y += pLabel->getTextRect().height() + 10;

        if (pUnit->getLoadedUnitCount() > 0)
        {
            pLabel = oxygine::spTextField::create();
            pLabel->setStyle(headerStyle);
            pLabel->setHtmlText(tr("Loaded Units"));
            pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
            addChild(pLabel);
            y += pLabel->getTextRect().height() + 10;
            createLoadedUnits(pUnit.get(), y, width);
            y += pLabel->getTextRect().height() + 10;
        }
    }

    y += 10;
    pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Actions"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;
    createActionTable(pUnit.get(), y, width);
    y += 50;

    pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText(tr("Transporters"));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10;
    createTransportTable(pUnit.get(), y, width);
    y += 50;

    if (y - yStart < 210)
    {
        y = yStart + 210;
    }
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    QString id = pUnit->getMovementType();
    name = pMovementTableManager->getName(id);
    pLabel = oxygine::spTextField::create();
    pLabel->setStyle(headerStyle);
    pLabel->setHtmlText((tr("Movement ") + name));
    pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
    addChild(pLabel);
    y += pLabel->getTextRect().height() + 10 + GameMap::getImageSize();
    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    QStringList sortedTerrains = pTerrainManager->getTerrainsSorted();
    qint32 x = 0;
    for (const auto& terrainId : sortedTerrains)
    {
        spTerrain pTerrain = Terrain::createTerrain(terrainId, -1, -1, "", pUnit->getMap());
        pTerrain->loadSprites();
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get(), pTerrain.get(), pUnit.get());
        pTerrain->setPosition(x, y);
        pTerrain->addClickListener([this, terrainId](oxygine::Event*)
        {
            emit sigShowLink(terrainId);
        });
        addChild(pTerrain);

        pLabel = oxygine::spTextField::create();
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
        pLabel->setPosition(x + GameMap::getImageSize() + 6, y - 5);
        addChild(pLabel);
        x += 90;
        if (x + 100 > width)
        {
            x = 0;
            y += GameMap::getImageSize() * 2 + 6;
        }
    }
    for (qint32 i = 0; i < pBuildingSpriteManager->getCount(); i++)
    {
        QString buildingId = pBuildingSpriteManager->getID(i);
        spBuilding pBuilding = spBuilding::create(buildingId, pUnit->getMap());
        spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", pUnit->getMap());
        pTerrain->setBuilding(pBuilding);
        qint32 costs = pMovementTableManager->getBaseMovementPoints(id, pTerrain.get(), pTerrain.get(), pUnit.get());
        pTerrain->removeBuilding();

        qint32 buildingWidth = pBuilding->getBuildingWidth();
        qint32 buildingHeigth = pBuilding->getBuildingHeigth();
        pBuilding->setScaleX(1.0f / static_cast<float>(buildingWidth));
        pBuilding->setScaleY(1.0f / static_cast<float>(buildingHeigth));
        pBuilding->updateBuildingSprites(false);

        pTerrain->loadSprites();
        pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain));
        pTerrain->setScaleX(1.0f / pBuilding->getScaleX());
        pTerrain->setScaleY(1.0f / pBuilding->getScaleY());
        if (buildingWidth > 1)
        {
            pTerrain->oxygine::Actor::setX(-GameMap::getImageSize() * (buildingWidth - 1));
        }
        if (buildingHeigth > 1)
        {
            pTerrain->oxygine::Actor::setY(-GameMap::getImageSize() * (buildingHeigth - 1));
        }
        pBuilding->addChild(pTerrain);
        pBuilding->oxygine::Actor::setX(x + GameMap::getImageSize() * (buildingWidth - 1) / (buildingWidth));
        pBuilding->oxygine::Actor::setY(y + GameMap::getImageSize() * (buildingHeigth - 1) / (buildingHeigth));

        pBuilding->addClickListener([this, pBuildingSpriteManager, i](oxygine::Event*)
        {
            emit sigShowLink(pBuildingSpriteManager->getID(i));
        });
        addChild(pBuilding);
        pLabel = oxygine::spTextField::create();
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
        pLabel->setPosition(x + GameMap::getImageSize() + 6, y - 5);
        addChild(pLabel);
        x += 90;
        if (x + 100 > width && i < pBuildingSpriteManager->getCount() - 1)
        {
            x = 0;
            y += GameMap::getImageSize() * 2 + 6;
        }
    }
    y += 60;

    if (pUnit->getWeapon1ID() != "" && pWeaponManager->exists(pUnit->getWeapon1ID()))
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText((tr("Weapon 1 ") + pWeaponManager->getName(pUnit->getWeapon1ID())));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
        createWeaponTable(pUnit.get(), pUnit->getWeapon1ID(), y, width);
        y += 60;
    }
    if (pUnit->getWeapon2ID() != "" && pWeaponManager->exists(pUnit->getWeapon2ID()))
    {
        pLabel = oxygine::spTextField::create();
        pLabel->setStyle(headerStyle);
        pLabel->setHtmlText((tr("Weapon 2 ") + pWeaponManager->getName(pUnit->getWeapon2ID())));
        pLabel->setPosition(width / 2 - pLabel->getTextRect().width() / 2, y);
        addChild(pLabel);
        y += pLabel->getTextRect().height() + 10;
        createWeaponTable(pUnit.get(), pUnit->getWeapon2ID(), y, width);
        y += 60;
    }
    setHeight(y);
    connect(this, &UnitInfo::sigShowLink, this, &UnitInfo::showLink, Qt::QueuedConnection);
}

void UnitInfo::loadBattleSprite(Unit* pUnit)
{
    if (m_pBattleAnimationSprite.get() != nullptr)
    {
        m_pBattleAnimationSprite->detach();
    }
    m_pBattleAnimationSprite = spBattleAnimationSprite::create(pUnit->getMap(), spUnit(pUnit), nullptr, BattleAnimationSprite::standingAnimation, -1, false);
    m_pBattleAnimationSprite->setPosition(m_pSpriteBox->getX() + 7, m_pSpriteBox->getY() + 5);
    m_pSpriteBox->setSize(m_pBattleAnimationSprite->getScaledWidth() + 14, m_pBattleAnimationSprite->getScaledHeight() + 12);
    addChild(m_pBattleAnimationSprite);
}

void UnitInfo::loadArmySelector(qint32 x, qint32 y)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("PLAYER", "getArmies");
    QStringList army = erg.toVariant().toStringList();
    GameManager* pGameManager = GameManager::getInstance();
    auto creator = [pGameManager](QString army)
    {
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("icon_" + army.toLower());
        oxygine::spSprite ret = oxygine::spSprite::create();
        ret->setResAnim(pAnim);
        return ret;
    };
    spDropDownmenuSprite pDropDown = spDropDownmenuSprite::create(105, army, creator);
    pDropDown->setPosition(x, y);
    pDropDown->setCurrentItem(m_pUnit->getOwner()->getArmy());
    connect(pDropDown.get(), &DropDownmenuSprite::sigItemString, this, &UnitInfo::showNewBattlesprite, Qt::QueuedConnection);
    addChild(pDropDown);
}

void UnitInfo::showNewBattlesprite(QString newArmy)
{
    spPlayer pPlayer = spPlayer::create(nullptr);
    pPlayer->setPlayerArmy(newArmy);
    pPlayer->init();
    spUnit pUnit = spUnit::create(m_pUnit->getUnitID(), pPlayer.get(), false, nullptr);
    loadBattleSprite(pUnit.get());
}

void UnitInfo::createWeaponTable(Unit* pUnit, const QString & weaponID, qint32& y, qint32 width)
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 x = 0;
    QStringList sortedUnits = pUnitSpriteManager->getUnitsSorted();
    for (qint32 i = 0; i < sortedUnits.size(); i++)
    {
        spUnit pDummy = spUnit::create(sortedUnits[i], pUnit->getOwner(), false, pUnit->getMap());
        QString unitId = sortedUnits[i];
        pDummy->addClickListener([this, unitId](oxygine::Event*)
        {
            emit sigShowLink(unitId);
        });
        float damage = pWeaponManager->getBaseDamage(weaponID, pDummy.get());
        pDummy->setPosition(x, y);
        addChild(pDummy);
        spLabel pLabel = spLabel::create(width);
        pLabel->setStyle(style);
        if (damage > 0)
        {
            pLabel->setHtmlText((QString::number(static_cast<qint32>(damage))  + " %"));
        }
        else
        {
            pLabel->setHtmlText("-");
        }
        pLabel->setPosition(x + GameMap::getImageSize() + 6, y - 5);
        addChild(pLabel);
        x += 150;
        if (x + 160 > width && i < pUnitSpriteManager->getCount() - 1)
        {
            x = 0;
            y += 40;
        }
    }
}

void UnitInfo::createLoadingTable(Unit* pUnit, const QStringList & loadables, qint32& y, qint32 width)
{
    qint32 x = 0;
    for (const auto& unitID : loadables)
    {
        spUnit pDummy = spUnit::create(unitID, pUnit->getOwner(), false, pUnit->getMap());
        pDummy->setPosition(x, y);
        pDummy->addClickListener([this, unitID](oxygine::Event*)
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
    for (auto & loadedUnit : pUnit->getLoadedUnits())
    {
        spUnit pDummy = spUnit::create(loadedUnit->getUnitID(), pUnit->getOwner(), false, pUnit->getMap());
        pDummy->setPosition(x, y);
        pDummy->setHasMoved(loadedUnit->getHasMoved());
        pDummy->setHp(loadedUnit->getHp());
        pDummy->setAmmo1(loadedUnit->getAmmo1());
        pDummy->setAmmo2(loadedUnit->getAmmo2());
        pDummy->setFuel(loadedUnit->getFuel());
        pDummy->setUnitRank(loadedUnit->getUnitRank());
        QString loadedUnitId = loadedUnit->getUnitID();
        pDummy->addClickListener([this, loadedUnitId](oxygine::Event*)
        {
            emit sigShowLink(loadedUnitId);
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
        spUnit pDummy = spUnit::create(unitID, pUnit->getOwner(), false, pUnit->getMap());
        if (pDummy->canTransportUnit(pUnit, true))
        {
            pDummy->setPosition(x, y);
            pDummy->addClickListener([this, unitID](oxygine::Event*)
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
    GameMap* pMap = pUnit->getMap();
    QStringList allowedActions;
    if (pMap != nullptr)
    {
        auto* pRules = pMap->getGameRules();
        allowedActions = pRules->getAllowedActions();
    }
    for (const auto & action : actions)
    {
        if (allowedActions.isEmpty() ||
            allowedActions.contains(action))
        {
            // QString text = GameAction::getActionText(action);
            QString icon = GameAction::getActionIcon(pUnit->getMap(), action);
            WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
            oxygine::spSprite pSprite = pWikiDatabase->getIcon(pUnit->getMap(), icon, GameMap::getImageSize());
            pSprite->setPosition(x, y);
            pSprite->addClickListener([this, action](oxygine::Event*)
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
}

void UnitInfo::showLink(QString pageID)
{
    if (!Wikipage::getPageStack().contains(pageID))
    {
        WikiDatabase* pWikiDatabase = WikiDatabase::getInstance();
        oxygine::Stage::getStage()->addChild(pWikiDatabase->getPage(pWikiDatabase->getEntry(pageID)));
    }
}
