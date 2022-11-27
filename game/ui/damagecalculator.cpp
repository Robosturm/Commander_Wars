#include "game/ui/damagecalculator.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/terrainmanager.h"
#include "resource_management/buildingspritemanager.h"
#include "resource_management/gamemanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "ai/coreai.h"

#include "coreengine/settings.h"

static constexpr qint32 xAdvance = 5;
static constexpr qint32 maxBuildingCount = 40;

DamageCalculator::DamageCalculator()
    : CloseablePopUp(800, 540),
      m_map(2, 2, 2)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    setPosition(Settings::getWidth() / 2 - getScaledWidth() / 2,
                Settings::getHeight() / 2 - getScaledHeight() / 2);
    if (getX() < 0)
    {
        setX(20);
    }
    if (getY() < 0)
    {
        setY(20);
    }
    qint32 x = 10;
    qint32 y = 10;
    spLabel pLabel = spLabel::create(300);
    pLabel->setPosition(getScaledWidth() / 2 - pLabel->getScaledWidth() / 2, y);
    pLabel->setHtmlText(tr("Damage calculator"));
    addItem(pLabel);

    QStringList coIds;
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    pCOSpriteManager->getCoGroups(coIds);
    coIds.push_front("");

    m_dropDownPlayer = spPlayer::create(&m_map);
    m_dropDownPlayer->init();

    QStringList rankItems;
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    Unit pUnit(pUnitSpriteManager->getID(0), m_dropDownPlayer.get(), false, &m_map);
    qint32 maxRang = pUnit.getMaxUnitRang();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getIcon";
    for (qint32 i = GameEnums::UnitRank_CO1; i <= maxRang; i++)
    {
        QJSValueList args({i,
                           pInterpreter->newQObject(&m_map)});
        QJSValue ret = pInterpreter->doFunction("UNITRANKINGSYSTEM", function1, args);
        rankItems.append(ret.toString());
    }

    TerrainManager* pTerrainManager = TerrainManager::getInstance();
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    auto unitIds = pUnitSpriteManager->getUnitsSorted();
    QStringList terrainIds = pTerrainManager->getTerrainsSorted();
    QStringList buildingIds = pBuildingSpriteManager->getLoadedBuildings();    
    terrainIds.append(buildingIds);

    y = 50;
    loadCoData(x, y, m_atkCos, coIds);
    qint32 startX = x + 10;
    x = 10;
    loadUnitData(x, y, m_atkUnit, rankItems, unitIds, terrainIds, buildingIds);
    y = 50;
    x = startX;
    loadCoData(x, y, m_defCos, coIds);
    x = startX;
    loadUnitData(x, y, m_defUnit, rankItems, unitIds, terrainIds, buildingIds);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spButton pCalculateButton = pObjectManager->createButton(tr("Calculate"), 150);
    pCalculateButton->setPosition(getScaledWidth() / 2 - pCalculateButton->getScaledWidth() / 2, y + 5);

    pCalculateButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigCalculateDamage();
    });
    connect(this, &DamageCalculator::sigCalculateDamage, this, &DamageCalculator::calculateDamage, Qt::QueuedConnection);
    addItem(pCalculateButton);
    pApp->continueRendering();
}

void DamageCalculator::loadCoData(qint32 & x, qint32 & y, CosData & cosData,
                                  const QStringList & coIds)
{
    qint32 startX = x;
    GameManager* pGameManager = GameManager::getInstance();
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    QStringList powers;
    GameAction dummy(&m_map);
    powers.append("no_power");
    powers.append(dummy.getActionIcon(&m_map, CoreAI::ACTION_ACTIVATE_POWER_CO_0));
    powers.append(dummy.getActionIcon(&m_map, CoreAI::ACTION_ACTIVATE_SUPERPOWER_CO_0));
    powers.append(dummy.getActionIcon(&m_map, CoreAI::ACTION_ACTIVATE_TAGPOWER));

    auto coCreator = [this, pCOSpriteManager](QString id)
    {
        oxygine::ResAnim* pAnim = nullptr;
        if (id.isEmpty())
        {
            pAnim = pCOSpriteManager->getResAnim("no_co+info");
        }
        else
        {
            pAnim = pCOSpriteManager->getResAnim(id + "+info");
        }
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim != nullptr)
        {
            pSprite->setResAnim(pAnim);
            pSprite->setScale(pAnim->getWidth() / 32.0f);
            pSprite->setSize(pAnim->getSize());
        }
        return pSprite;
    };
    auto powerCreator = [this, pGameManager](QString id)
    {
        oxygine::spSprite pSprite = pGameManager->getIcon(&m_map, id);
        const oxygine::ResAnim* pAnim = pSprite->getResAnim();
        if (pAnim != nullptr)
        {
            pSprite->setScale(1.0f);
            pSprite->setSize(pAnim->getSize());
        }
        return pSprite;
    };
    for (auto & coData : cosData)
    {
        x = startX;

        coData.m_co = spDropDownmenuSprite::create(150, coIds, coCreator);
        coData.m_co->setTooltipText(tr("CO for the player."));
        coData.m_co->setPosition(x, y);
        addItem(coData.m_co);
        x += coData.m_co->getScaledWidth() + xAdvance;
        coData.m_powerMode = spDropDownmenuSprite::create(110, powers, powerCreator);
        coData.m_powerMode->setTooltipText(tr("Power level of the co."));
        coData.m_powerMode->setPosition(x, y);
        addItem(coData.m_powerMode);
        x += coData.m_powerMode->getScaledWidth() + xAdvance;
        spLabel pLabel = spLabel::create(60);
        pLabel->setPosition(x, y);
        pLabel->setHtmlText(tr("Zone"));
        addItem(pLabel);
        x += pLabel->getScaledWidth();
        coData.m_inCoRange = spCheckbox::create();
        coData.m_inCoRange->setPosition(x, y);
        coData.m_inCoRange->setTooltipText(tr("If the unit is in the co-zone of this co."));
        addItem(coData.m_inCoRange);
        x += coData.m_inCoRange->getScaledWidth() + xAdvance;
        y += pLabel->getScaledHeight() + 5;
    }
}

void DamageCalculator::loadUnitData(qint32 & x, qint32 & y, UnitData & unitData,
                                    const QStringList & rankItems, const QStringList & unitIds,
                                    const QStringList & terrainIds, const QStringList & buildingIds)
{
    qint32 startX = x;
    Player* pPlayer = m_dropDownPlayer.get();
    auto unitCreator = [this, pPlayer](QString id)
    {
        spUnit pSprite = spUnit::create(id, pPlayer, false, &m_map);
        return pSprite;
    };
    unitData.m_unit = spDropDownmenuSprite::create(105, unitIds, unitCreator, 30);
    unitData.m_unit->setTooltipText(tr("Unit to be used."));
    unitData.m_unit->setPosition(x, y);
    addItem(unitData.m_unit);
    x += unitData.m_unit->getScaledWidth() + xAdvance;
    auto terrainCreator = [this, buildingIds](QString id)
    {
        QString terrainId = GameMap::PLAINS;
        bool isBuilding = buildingIds.contains(id);
        if (!isBuilding)
        {
            terrainId = id;
        }
        spTerrain pTerrain = Terrain::createTerrain(terrainId, -1, -1, "", &m_map);
        pTerrain->loadSprites();
        pTerrain->setPriority(-100);
        oxygine::spSprite pRet = pTerrain;
        if (isBuilding)
        {
            spBuilding building = spBuilding::create(id, &m_map);
            building->setTooltipText(building->getName());
            building->updateBuildingSprites(false);
            building->addChild(pTerrain);
            pRet = building;
        }
        return pRet;
    };
    unitData.m_Terrain = spDropDownmenuSprite::create(105, terrainIds, terrainCreator, -1, false);
    unitData.m_Terrain->setTooltipText(tr("Terrain the unit is currently sitting on."));
    unitData.m_Terrain->setPosition(x, y);
    addItem(unitData.m_Terrain);
    x += unitData.m_unit->getScaledWidth() + xAdvance;
    auto unitRankCreator = [=](QString id)
    {
        oxygine::ResAnim* pAnim = nullptr;
        if (id.isEmpty())
        {
            pAnim = UnitSpriteManager::getInstance()->getResAnim("no_rank");
        }
        else
        {
            pAnim = UnitSpriteManager::getInstance()->getResAnim(id);
        }
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (pAnim != nullptr)
        {
            pSprite->setResAnim(pAnim);
            pSprite->setScale(pAnim->getWidth() / 30.0f);
            pSprite->setSize(pAnim->getSize());
        }
        return pSprite;
    };

    unitData.m_unitRank = spDropDownmenuSprite::create(105, rankItems, unitRankCreator, 30);
    unitData.m_unitRank->setTooltipText(tr("Selects the Rank of this Unit. CO Ranks may be replaced with highest rang. This is immediatly applied."));
    unitData.m_unitRank->setCurrentItem(-GameEnums::UnitRank_CO1);
    unitData.m_unitRank->setPosition(x, y);
    addItem(unitData.m_unitRank);
    x = startX;
    y += unitData.m_unitRank->getScaledHeight() + 5;
    spLabel pLabel = spLabel::create(150);
    pLabel->setPosition(x, y);
    pLabel->setHtmlText(tr("HP:"));
    addItem(pLabel);
    unitData.m_hp = spSpinBox::create(200, 1, 10);
    unitData.m_hp->setCurrentValue(10);
    unitData.m_hp->setTooltipText(tr("HP of the unit."));
    unitData.m_hp->setPosition(x + pLabel->getScaledWidth() + xAdvance, y);
    addItem(unitData.m_hp);
    y += pLabel->getScaledHeight() + 5;
    pLabel = spLabel::create(150);
    pLabel->setPosition(x, y);
    pLabel->setHtmlText(tr("Towers:"));
    addItem(pLabel);
    unitData.m_towers = spSpinBox::create(200, 0, maxBuildingCount);
    unitData.m_towers->setCurrentValue(0);
    unitData.m_towers->setTooltipText(tr("Amount of towers owned by the player."));
    unitData.m_towers->setPosition(x + pLabel->getScaledWidth() + xAdvance, y);
    addItem(unitData.m_towers);
    y += pLabel->getScaledHeight() + 5;
    pLabel = spLabel::create(150);
    pLabel->setPosition(x, y);
    pLabel->setHtmlText(tr("Towns:"));
    addItem(pLabel);
    unitData.m_buildings = spSpinBox::create(200, 0, maxBuildingCount);
    unitData.m_buildings->setCurrentValue(0);
    unitData.m_buildings->setTooltipText(tr("Amount of towns owned by the player."));
    unitData.m_buildings->setPosition(x + pLabel->getScaledWidth() + xAdvance, y);
    addItem(unitData.m_buildings);
    y += pLabel->getScaledHeight() + 5;
    pLabel = spLabel::create(150);
    pLabel->setPosition(x, y);
    pLabel->setHtmlText(tr("Funds:"));
    addItem(pLabel);
    unitData.m_funds = spSpinBox::create(200, 0, 9999999);
    unitData.m_funds->setCurrentValue(0);
    unitData.m_funds->setTooltipText(tr("Amount of funds owned by the player."));
    unitData.m_funds->setPosition(x + pLabel->getScaledWidth() + xAdvance, y);
    addItem(unitData.m_funds);
    y += pLabel->getScaledHeight() + 5;

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.multiline = false;

    pLabel = spLabel::create(150);
    pLabel->setHtmlText(tr("Battle"));
    pLabel->setPosition(x + 60, y + 50);
    pLabel->setStyle(style);
    addItem(pLabel);

    pLabel = spLabel::create(59);
    pLabel->setHtmlText(tr("Min:"));
    pLabel->setPosition(x, y + 80);
    addItem(pLabel);
    pLabel = spLabel::create(59);
    pLabel->setHtmlText(tr("Avg:"));
    pLabel->setPosition(x, y + 110);
    addItem(pLabel);
    pLabel = spLabel::create(59);
    pLabel->setHtmlText(tr("Max:"));
    pLabel->setPosition(x, y + 140);
    addItem(pLabel);

    unitData.m_minBattleDamage = spLabel::create(150);
    unitData.m_minBattleDamage->setHtmlText("");
    unitData.m_minBattleDamage->setPosition(x + 60, y + 80);
    unitData.m_minBattleDamage->setStyle(style);
    addItem(unitData.m_minBattleDamage);
    unitData.m_avgBattleDamage = spLabel::create(150);
    unitData.m_avgBattleDamage->setHtmlText("");
    unitData.m_avgBattleDamage->setPosition(x + 60, y + 110);
    unitData.m_avgBattleDamage->setStyle(style);
    addItem(unitData.m_avgBattleDamage);
    unitData.m_maxBattleDamage = spLabel::create(150);
    unitData.m_maxBattleDamage->setHtmlText("");
    unitData.m_maxBattleDamage->setPosition(x + 60, y + 140);
    unitData.m_maxBattleDamage->setStyle(style);
    addItem(unitData.m_maxBattleDamage);

    pLabel = spLabel::create(150);
    pLabel->setHtmlText(tr("Post battle"));
    pLabel->setPosition(x + 210, y + 50);
    pLabel->setStyle(style);
    addItem(pLabel);

    unitData.m_minPostbattleDamage = spLabel::create(150);
    unitData.m_minPostbattleDamage->setHtmlText("");
    unitData.m_minPostbattleDamage->setPosition(x + 210, y + 80);
    unitData.m_minPostbattleDamage->setStyle(style);
    addItem(unitData.m_minPostbattleDamage);
    unitData.m_avgPostbattleDamage = spLabel::create(150);
    unitData.m_avgPostbattleDamage->setHtmlText("");
    unitData.m_avgPostbattleDamage->setPosition(x + 210, y + 110);
    unitData.m_avgPostbattleDamage->setStyle(style);
    addItem(unitData.m_avgPostbattleDamage);
    unitData.m_maxPostbattleDamage = spLabel::create(150);
    unitData.m_maxPostbattleDamage->setHtmlText("");
    unitData.m_maxPostbattleDamage->setPosition(x + 210, y + 140);
    unitData.m_maxPostbattleDamage->setStyle(style);
    addItem(unitData.m_maxPostbattleDamage);
}

void DamageCalculator::calculateDamage()
{
    QPoint atkPos = QPoint(0, 0);
    QPoint defPos = QPoint(0, 0);
    updateMapData(defPos);
    spUnit pAttacker = m_map.getTerrain(atkPos.x(), atkPos.y())->getSpUnit();
    spUnit pDefender = m_map.getTerrain(defPos.x(), defPos.y())->getSpUnit();

    QRectF minDamage = CoreAI::calcVirtuelUnitDamage(&m_map,
                                                    pAttacker.get(), 0.0f, atkPos, GameEnums::LuckDamageMode_Min,
                                                    pDefender.get(), 0.0f, defPos, GameEnums::LuckDamageMode_Max,
                                                    true, false);
    if (minDamage.x() < 0)
    {
        if (pAttacker->hasDirectWeapon())
        {
            defPos = QPoint(1, 0);
        }
        pDefender->getTerrain()->setUnit(spUnit());
        m_map.getTerrain(defPos.x(), defPos.y())->setUnit(pDefender);
        spUnit pAttacker = m_map.getTerrain(atkPos.x(), atkPos.y())->getSpUnit();
        spUnit pDefender = m_map.getTerrain(defPos.x(), defPos.y())->getSpUnit();
        minDamage = CoreAI::calcVirtuelUnitDamage(&m_map,
                                                  pAttacker.get(), 0.0f, atkPos, GameEnums::LuckDamageMode_Min,
                                                  pDefender.get(), 0.0f, defPos, GameEnums::LuckDamageMode_Max,
                                                  true, false);
    }
    QRectF avgDamage = CoreAI::calcVirtuelUnitDamage(&m_map,
                                                    pAttacker.get(), 0.0f, atkPos, GameEnums::LuckDamageMode_Average,
                                                    pDefender.get(), 0.0f, defPos, GameEnums::LuckDamageMode_Average,
                                                    true, false);
    QRectF maxDamage = CoreAI::calcVirtuelUnitDamage(&m_map,
                                                    pAttacker.get(), 0.0f, atkPos, GameEnums::LuckDamageMode_Max,
                                                    pDefender.get(), 0.0f, defPos, GameEnums::LuckDamageMode_Min,
                                                    true, false);
    createDamageText(m_atkUnit.m_minBattleDamage, m_atkUnit.m_avgBattleDamage, m_atkUnit.m_maxBattleDamage,
                     minDamage.x(), avgDamage.x(), maxDamage.x());
    createDamageText(m_defUnit.m_minBattleDamage, m_defUnit.m_avgBattleDamage, m_defUnit.m_maxBattleDamage,
                     maxDamage.width(), avgDamage.width(), minDamage.width());

    QRectF minPostDamage = calculatePostBattleDamage(pAttacker, pDefender, minDamage);
    QRectF avgPostDamage = calculatePostBattleDamage(pAttacker, pDefender, avgDamage);
    QRectF maxPostDamage = calculatePostBattleDamage(pAttacker, pDefender, maxDamage);
    createDamageText(m_atkUnit.m_minPostbattleDamage, m_atkUnit.m_avgPostbattleDamage, m_atkUnit.m_maxPostbattleDamage,
                     minPostDamage.x(), avgPostDamage.x(), maxPostDamage.x());
    createDamageText(m_defUnit.m_minPostbattleDamage, m_defUnit.m_avgPostbattleDamage, m_defUnit.m_maxPostbattleDamage,
                     maxPostDamage.width(), avgPostDamage.width(), minPostDamage.width());
}

void DamageCalculator::updateMapData(QPoint & defPos, bool forceDirect)
{
    m_map.newMap(1, 1, 2);
    BuildingSpriteManager* pBuildingSpriteManager = BuildingSpriteManager::getInstance();
    QStringList buildingIds = pBuildingSpriteManager->getLoadedBuildings();
    // attacker
    Player* pPlayer = m_map.getPlayer(0);
    pPlayer->setFunds(m_atkUnit.m_funds->getCurrentValue());
    for (qint32 i = 0; i < pPlayer->getMaxCoCount(); ++i)
    {
        pPlayer->setCO(m_atkCos[i].m_co->getCurrentItemText(), i);
        CO* pCO = pPlayer->getCO(i);
        if (pCO != nullptr)
        {
            pCO->setGlobalCoZone(m_atkCos[i].m_inCoRange->getChecked());
            pCO->setPowerMode(static_cast<GameEnums::PowerMode>(m_atkCos[i].m_powerMode->getCurrentItem()));
        }
    }
    spUnit pAttacker = spUnit::create(m_atkUnit.m_unit->getCurrentItemText(), pPlayer, false, &m_map);
    pAttacker->setHp(m_atkUnit.m_hp->getCurrentValue());
    pAttacker->setUnitRank(m_atkUnit.m_unitRank->getCurrentItem() + GameEnums::UnitRank_CO1);
    // defender
    pPlayer = m_map.getPlayer(1);
    pPlayer->setFunds(m_defUnit.m_funds->getCurrentValue());
    for (qint32 i = 0; i < pPlayer->getMaxCoCount(); ++i)
    {
        pPlayer->setCO(m_defCos[i].m_co->getCurrentItemText(), i);
        CO* pCO = pPlayer->getCO(i);
        if (pCO != nullptr)
        {
            pCO->setGlobalCoZone(m_defCos[i].m_inCoRange->getChecked());
            pCO->setPowerMode(static_cast<GameEnums::PowerMode>(m_defCos[i].m_powerMode->getCurrentItem()));
        }
    }
    spUnit pDefender = spUnit::create(m_defUnit.m_unit->getCurrentItemText(), pPlayer, false, &m_map);
    pDefender->setHp(m_defUnit.m_hp->getCurrentValue());
    pDefender->setUnitRank(m_defUnit.m_unitRank->getCurrentItem() + GameEnums::UnitRank_CO1);

    if (buildingIds.contains(m_atkUnit.m_Terrain->getCurrentItemText()))
    {
        m_map.replaceTerrain(GameMap::PLAINS, 0, 0, false, false, false);
        m_map.replaceBuilding(m_atkUnit.m_Terrain->getCurrentItemText(), 0, 0);
    }
    else
    {
        m_map.replaceTerrain(m_atkUnit.m_Terrain->getCurrentItemText(), 0, 0, false, false, false);
    }
    m_map.getTerrain(0, 0)->setUnit(pAttacker);
    qint32 minRange = pAttacker->getMinRange(QPoint(0, 0));
    m_map.changeMap(4 + minRange, 1 + maxBuildingCount, 2);
    defPos = QPoint(minRange, 0);

    if (buildingIds.contains(m_defUnit.m_Terrain->getCurrentItemText()))
    {
        m_map.replaceTerrain(GameMap::PLAINS, defPos.x(), defPos.y(), false, false, false);
        m_map.replaceBuilding(m_defUnit.m_Terrain->getCurrentItemText(), defPos.x(), defPos.y());
    }
    else
    {
        m_map.replaceTerrain(m_defUnit.m_Terrain->getCurrentItemText(), defPos.x(), defPos.y(), false, false, false);
    }
    m_map.getTerrain(defPos.x(), defPos.y())->setUnit(pDefender);

    // place towers and towns
    QString tower = "TOWER";
    QString town = "TOWN";
    placeBuilding(tower, 0, m_atkUnit.m_towers->getCurrentValue(), 0, m_atkUnit.m_Terrain->getCurrentItemText());
    placeBuilding(town, 0, m_atkUnit.m_buildings->getCurrentValue(), 1, m_atkUnit.m_Terrain->getCurrentItemText());
    placeBuilding(tower, 1, m_defUnit.m_towers->getCurrentValue(), 2, m_defUnit.m_Terrain->getCurrentItemText());
    placeBuilding(town, 1, m_defUnit.m_buildings->getCurrentValue(), 3, m_defUnit.m_Terrain->getCurrentItemText());
}

void DamageCalculator::placeBuilding(QString & buildingId, qint32 player, qint32 count, qint32 x, QString unitBuilding)
{
    Player* pPlayer = m_map.getPlayer(player);
    for (qint32 y = 1; y <= count; ++y)
    {
        m_map.replaceBuilding(buildingId, x, y);
        m_map.getTerrain(x, y)->getBuilding()->setOwner(pPlayer);
    }
}

void DamageCalculator::createDamageText(spLabel & minText, spLabel & avgText, spLabel & maxText,
                                        qint32 minDmg, qint32 avgDmg, qint32 maxDmg)
{
    QString dmgText;
    if (minDmg < 0)
    {
        dmgText = "-";
    }
    else
    {
        dmgText = QString::number(static_cast<qint32>(minDmg));
    }
    dmgText += "%";
    minText->setHtmlText(dmgText);

    if (avgDmg < 0)
    {
        dmgText = "-";
    }
    else
    {
        dmgText = QString::number(static_cast<qint32>(avgDmg));
    }
    dmgText += "%";
    avgText->setHtmlText(dmgText);

    if (maxDmg < 0)
    {
        dmgText = "-";
    }
    else
    {
        dmgText = QString::number(static_cast<qint32>(maxDmg));
    }
    dmgText += "%";
    maxText->setHtmlText(dmgText);
}

QRectF DamageCalculator::calculatePostBattleDamage(spUnit pAttacker, spUnit pDefender, QRectF & info)
{
    QRectF ret = info;
    auto attackerPos = pAttacker->Unit::getPosition();
    auto defenderPos = pDefender->Unit::getPosition();

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(&m_map),
                       pInterpreter->newQObject(pAttacker.get()),
                       info.x(),
                       info.y(),
                       defenderPos.x(),
                       defenderPos.y(),
                       info.width(),
                       info.height(),
                       true,
                       true});
    pInterpreter->doFunction(CoreAI::ACTION_FIRE, "battle", args);

    float counterDmg = (m_atkUnit.m_hp->getCurrentValue() - pAttacker->getHp()) * Unit::MAX_UNIT_HP;
    float dmg = (m_defUnit.m_hp->getCurrentValue() - pDefender->getHp()) * Unit::MAX_UNIT_HP;
    ret = QRectF(dmg, info.y(), counterDmg, info.height());
    pAttacker->setHp(m_atkUnit.m_hp->getCurrentValue());
    m_map.getTerrain(attackerPos.x(), attackerPos.y())->setUnit(pAttacker);
    pDefender->setHp(m_defUnit.m_hp->getCurrentValue());
    m_map.getTerrain(defenderPos.x(), defenderPos.y())->setUnit(pDefender);
    return ret;
}
