#include "ai/heavyai.h"

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/weaponmanager.h"

// code for building units is here

bool HeavyAi::buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                         spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    scoreUnitBuildings(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings);
    double bestScore = std::numeric_limits<float>::min();
    qint32 index = -1;
    for (qint32 i = 0; i < m_BuildingData.size(); ++i)
    {
        if (m_BuildingData[i].m_action.get() != nullptr &&
            m_BuildingData[i].m_score > bestScore)
        {
            bestScore = m_BuildingData[i].m_score;
            index = i;
        }
    }
    if (index >= 0)
    {
        auto & item = m_BuildingData[index];
        Console::print("HeavyAi::buildUnits " + item.buildingDataInput[item.m_selectedData].unitId + " with scored value " + QString::number(bestScore), Console::eDEBUG);
        m_updatePoints.append(item.m_action->getTarget());
        emit performAction(item.m_action);
        item.m_action = nullptr;
        item.m_score = 0;
        if (item.buildingDataInput[item.m_selectedData].unitBuildingDataInput[BuildingEntry::CanAttackImmuneUnitRatio] > 0 ||
            item.buildingDataInput[item.m_selectedData].unitBuildingDataInput[BuildingEntry::UnitsToTransportRatio] > 0)
        {
            // reset data
            m_BuildingData.clear();
        }
        return true;
    }
    else
    {
        Console::print("HeavyAi::buildUnits build no units highest value was: " + QString::number(bestScore), Console::eDEBUG);
    }
    return false;
}

void HeavyAi::scoreUnitBuildings(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                 spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    QVector<std::tuple<Unit*, Unit*>> transportTargets;
    QVector<double> data = getGlobalBuildInfo(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings, transportTargets);
    QVector<Unit*> immuneUnits;
    getImmuneUnits(pUnits, pEnemyUnits, immuneUnits);


    // do the actual scoring
    qint32 funds = m_pPlayer->getFunds();
    for (auto & building : m_BuildingData)
    {
        if (building.buildingDataInput.size() == 0)
        {
            createUnitBuildData(building, data, funds, immuneUnits, transportTargets, pEnemyBuildings);
        }
        else
        {
            updateUnitBuildData(building, data, funds);
        }
        scoreBuildingProductionData(building);
    }
}

void HeavyAi::scoreBuildingProductionData(HeavyAi::BuildingData & building)
{
    double bestScore = 0.0;
    QVector<qint32> bestItems;
    QVector<double> scores;
    building.m_action = nullptr;
    building.m_score = 0;
    for (qint32 i = 0; i < building.buildingDataInput.size(); ++i)
    {
        if (building.buildingDataInput[i].enabled)
        {
            auto score = m_neuralNetworks[NeuralNetworks::Production]->predict(building.buildingDataInput[i].unitBuildingDataInput);
            double value = score[0] * BaseGameInputIF::getUnitBuildValue(building.buildingDataInput[i].unitId);
            if (value > m_maxScore)
            {
                value = m_maxScore;
            }
            if (value > bestScore && value >= m_minActionScore)
            {
                bestScore = value;
                qint32 i2 = 0;
                while (i2 < bestItems.size())
                {
                    if (scores[i2] < bestScore - m_actionScoreVariant)
                    {
                        scores.removeAt(i2);
                        bestItems.removeAt(i2);
                    }
                    else
                    {
                        ++i2;
                    }
                }
                scores.append(bestScore);
                bestItems.append(i);
            }
            else if (bestScore - m_actionScoreVariant <= value)
            {
                bestItems.append(i);
                scores.append(value);
            }
        }
    }
    if (bestItems.size() > 0)
    {
        qint32 item = GlobalUtils::randInt(0, bestItems.size() - 1);
        building.m_score = scores[item];
        building.m_selectedData = bestItems[item];
        building.m_action = spGameAction::create(CoreAI::ACTION_BUILD_UNITS);
        building.m_action->setTarget(QPoint(building.m_pBuilding->Building::getX(), building.m_pBuilding->Building::getY()));
        CoreAI::addMenuItemData(building.m_action, building.buildingDataInput[building.m_selectedData].unitId, building.buildingDataInput[building.m_selectedData].cost);
    }
}

void HeavyAi::createUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds, const QVector<Unit*> & immuneUnits,
                                  const QVector<std::tuple<Unit*, Unit*>> & transportTargets, const spQmlVectorBuilding & pEnemyBuildings)
{
    // create new
    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS);
    qint32 x = building.m_pBuilding->Building::getX();
    qint32 y = building.m_pBuilding->Building::getY();
    pAction->setTarget(QPoint(x, y));
    if (pAction->canBePerformed())
    {
        // we're allowed to build units here
        spMenuData pData = pAction->getMenuStepData();
        if (pData->validData())
        {
            auto enableList = pData->getEnabledList();
            auto actionIds = pData->getActionIDs();
            auto costs = pData->getCostList();
            for (qint32 i = 0; i < pData->getActionIDs().size(); i++)
            {
                if (enableList[i])
                {
                    UnitBuildData unitData;
                    unitData.unitId = actionIds[i];
                    unitData.cost = costs[i];
                    Unit dummy(unitData.unitId, m_pPlayer, false);
                    dummy.setVirtuellX(x);
                    dummy.setVirtuellY(y);
                    qint32 movementPoints = dummy.getMovementpoints(QPoint(x, y));
                    getProductionInputVector(building.m_pBuilding, &dummy, unitData, immuneUnits, movementPoints);
                    getTransportInputVector(building.m_pBuilding, &dummy, transportTargets, pEnemyBuildings, movementPoints, unitData);
                    updateUnitBuildData(unitData, data, funds);
                    building.buildingDataInput.append(unitData);
                }
            }
        }
    }
}

void HeavyAi::updateUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds)
{
    GameAction action = GameAction(ACTION_BUILD_UNITS);
    action.setTarget(QPoint(building.m_pBuilding->Building::getX(), building.m_pBuilding->Building::getY()));
    if (action.canBePerformed())
    {
        spMenuData pData = action.getMenuStepData();
        if (pData->validData())
        {
            // update
            auto enableList = pData->getEnabledList();
            auto actionIds = pData->getActionIDs();
            for (auto & unitData : building.buildingDataInput)
            {
                for (qint32 i = 0; i < enableList.size(); ++i)
                {
                    if (unitData.unitId == actionIds[i])
                    {
                        unitData.enabled = enableList[i];
                        break;
                    }
                }
                updateUnitBuildData(unitData, data, funds);
            }
        }
        else
        {
            building.buildingDataInput.clear();
        }
    }
    else
    {
        building.buildingDataInput.clear();
    }
}

void HeavyAi::updateUnitBuildData(UnitBuildData &unitData, QVector<double> &data, qint32 funds)
{
    for (qint32 i = 0; i < BuildingEntry::LocalUnitData; ++i)
    {
        unitData.unitBuildingDataInput[i] = data[i];
    }
    if (funds > 0)
    {
        unitData.unitBuildingDataInput[BuildingEntry::FondsUsage] = static_cast<double>(unitData.cost) / static_cast<double>(funds);
    }
    else
    {
        unitData.unitBuildingDataInput[BuildingEntry::FondsUsage] = 0;
    }
}

const QString &HeavyAi::getAiName() const
{
    return m_aiName;
}

void HeavyAi::setAiName(const QString &newAiName)
{
    m_aiName = newAiName;
}

void HeavyAi::getProductionInputVector(Building* pBuilding, Unit* pUnit, UnitBuildData & data, const QVector<Unit*> & immuneUnits, qint32 movementPoints)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        QStringList actionList = pUnit->getActionList();
        if (pUnit->getBaseMaxRange() > 1)
        {
            data.unitBuildingDataInput[BuildingEntry::BasicAttackRange] = 1;
        }
        else
        {
            data.unitBuildingDataInput[BuildingEntry::BasicAttackRange] = -1;
        }
        if (actionList.contains(ACTION_CAPTURE))
        {
            data.unitBuildingDataInput[BuildingEntry::CaptureUnit] = 1;
        }
        else
        {
            data.unitBuildingDataInput[BuildingEntry::CaptureUnit] = -1;
        }
        QPoint position(pBuilding->Building::getX(), pBuilding->Building::getY());
        auto & influenceInfo = m_InfluenceFrontMap.getInfluenceInfo(position.x(), position.y());
        double highestInfluence = m_InfluenceFrontMap.getTotalHighestInfluence();
        double value = getAiCoUnitMultiplier(m_pPlayer->getCO(0), pUnit);
        value += getAiCoUnitMultiplier(m_pPlayer->getCO(1), pUnit);
        data.unitBuildingDataInput[BuildingEntry::CoUnitValue] = value / (CO::MAX_CO_UNIT_VALUE * 2);
        data.unitBuildingDataInput[BuildingEntry::Movementpoints] = static_cast<double>(movementPoints) / static_cast<double>(m_maxMovementpoints);
        data.unitBuildingDataInput[BuildingEntry::VisionPotential] = pUnit->getVision(position) / m_maxVision;
        data.unitBuildingDataInput[BuildingEntry::MapMovementpoints] = movementPoints / static_cast<double>(pMap->getMapHeight() * pMap->getMapWidth());
        data.unitBuildingDataInput[BuildingEntry::FireRange] = static_cast<double>(pUnit->getMaxRange(position)) / static_cast<double>(m_maxFirerange);
        data.unitBuildingDataInput[BuildingEntry::Flying] = (pUnit->useTerrainDefense() == false) ? 1 : -1;
        data.unitBuildingDataInput[BuildingEntry::LoadingPotential] = static_cast<double>(pUnit->getLoadingPlace()) / 4.0;
        data.unitBuildingDataInput[BuildingEntry::OwnInfluence] = static_cast<double>(influenceInfo.ownInfluence) / highestInfluence;
        data.unitBuildingDataInput[BuildingEntry::HighestEnemyInfluence] = static_cast<double>(influenceInfo.highestEnemyInfluence) / highestInfluence;
        qint32 islandIdx = getIslandIndex(pUnit);
        qint32 island = m_IslandMaps[islandIdx]->getIsland(position.x(), position.y());
        qint32 islandSize =  m_IslandMaps[islandIdx]->getIslandSize(island);
        double maxIslandSize = movementPoints * movementPoints;
        if (islandSize > maxIslandSize)
        {
            data.unitBuildingDataInput[BuildingEntry::MovementPotential] = 1;
        }
        else if (maxIslandSize > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::MovementPotential] = islandSize / maxIslandSize;
        }
        calculateUnitProductionDamage(pBuilding, pUnit, movementPoints, position, data, immuneUnits);
    }
}

void HeavyAi::getTransportInputVector(Building* pBuilding, Unit* pUnit, const QVector<std::tuple<Unit*, Unit*>> transportTargets,
                                      const spQmlVectorBuilding & pEnemyBuildings, qint32 movementPoints, UnitBuildData & data)
{
    if (data.unitBuildingDataInput[BuildingEntry::LoadingPotential] > 0)
    {
        QPoint position(pBuilding->Building::getX(), pBuilding->Building::getY());
        QVector<QVector3D> targets;
        QVector<Unit*> loadingUnits = appendLoadingTargets(pUnit, m_pUnits, m_pEnemyUnits, pEnemyBuildings, false, true, targets, true);
        QVector<Unit*> transporterUnits;
        for (qint32 i2 = 0; i2 < transportTargets.size(); i2++)
        {
            if (!transporterUnits.contains(std::get<0>(transportTargets[i2])))
            {
                transporterUnits.append(std::get<0>(transportTargets[i2]));
            }
        }
        double transporterCount = 0;
        for (const auto & unit : qAsConst(loadingUnits))
        {
            if (canTransportToEnemy(pUnit, unit, m_pEnemyUnits, pEnemyBuildings))
            {
                double scoreMultiplier = getProductionScoreMultiplier(position, unit->Unit::getMapPosition(), movementPoints);
                transporterCount += scoreMultiplier;
            }
        }
        double requiredTransporterCount = 0;
        for (const auto & unit : qAsConst(transporterUnits))
        {
            double scoreMultiplier = getProductionScoreMultiplier(position, unit->Unit::getMapPosition(), movementPoints);
            requiredTransporterCount += scoreMultiplier;
        }
        if (transportTargets.size() > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::RequiredUnitsToTransportRatio] = requiredTransporterCount / static_cast<double>(transportTargets.size());
        }
        if (m_pUnits->size() > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::UnitsToTransportRatio] = transporterCount / static_cast<double>(m_pUnits->size());
        }
    }
}

void HeavyAi::calculateUnitProductionDamage(Building* pBuilding, Unit* pUnit, qint32 movementPoints, QPoint position, UnitBuildData & data, const QVector<Unit*> & immuneUnits)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 mapWidth = pMap->getMapWidth();
        qint32 mapHeight = pMap->getMapHeight();
        qint32 unitValue = pUnit->getUnitValue();
        bool canCapture = (data.unitBuildingDataInput[BuildingEntry::CaptureUnit] > 0);
        double fundsDamage = 0.0;
        double possibleFundsDamage = 0.0;
        double hpDamage = 0.0;
        double possibleHpDamage = 0.0;
        double captureRate = 0.0;
        double maxCaptureRate = 0.0;
        double counterFundsDamage = 0.0;
        double counterhpDamage = 0.0;
        double counterAttackCount = 0.0;
        double immuneUnit = 0;
        for (qint32 x = 0; x < mapWidth; ++x)
        {
            for (qint32 y = 0; y < mapHeight; ++y)
            {
                Terrain* pTerrain = pMap->getTerrain(x, y);
                Unit* pEnemyUnit = pTerrain->getUnit();
                Building* pBuilding = pTerrain->getBuilding();
                double scoreMultiplier = getProductionScoreMultiplier(position, QPoint(x, y), movementPoints);
                if (pEnemyUnit != nullptr &&
                    m_pPlayer->isEnemyUnit(pEnemyUnit))
                {
                    double unitMulitpler = scoreMultiplier;
                    if (isPrimaryEnemy(pEnemyUnit))
                    {
                        unitMulitpler *= m_primaryEnemyMultiplier;
                    }
                    double dmg = getBaseDamage(pUnit, pEnemyUnit);
                    double enemyHp = pEnemyUnit->getHp();
                    double enemyHpRatio = enemyHp / Unit::MAX_UNIT_HP;
                    if (dmg >= 0)
                    {
                        double enemyUnitValue = pEnemyUnit->getUnitValue();
                        double damageMultiplier = dmg / (enemyHp * Unit::MAX_UNIT_HP);
                        fundsDamage         += unitMulitpler * enemyUnitValue * damageMultiplier;
                        possibleFundsDamage += unitMulitpler * enemyUnitValue;
                        hpDamage            += unitMulitpler * damageMultiplier;
                        possibleHpDamage    += unitMulitpler * enemyHp / Unit::MAX_UNIT_HP;
                        if (immuneUnits.contains(pEnemyUnit))
                        {
                            ++immuneUnit;
                        }
                    }
                    dmg = getBaseDamage(pEnemyUnit, pUnit) * enemyHpRatio;
                    if (dmg >= 0)
                    {
                        static constexpr double maxDamage = Unit::MAX_UNIT_HP  * Unit::MAX_UNIT_HP;
                        counterFundsDamage  += unitMulitpler * unitValue * dmg / maxDamage;
                        counterhpDamage     += unitMulitpler * dmg / maxDamage;
                        ++counterAttackCount;
                    }
                }
                if (canCapture &&
                    pBuilding != nullptr &&
                    m_pPlayer->isEnemy(pBuilding->getOwner()) &&
                    pBuilding->isCaptureOrMissileBuilding(true))
                {
                    double buildingMulitpler = 1;
                    if (isPrimaryEnemy(pBuilding))
                    {
                        buildingMulitpler = m_primaryEnemyMultiplier;
                        scoreMultiplier *= m_primaryEnemyMultiplier;
                    }
                    if (pBuilding->getActionList().contains(ACTION_BUILD_UNITS))
                    {
                        qint32 buildSize = pBuilding->getConstructionList().size();
                        captureRate += scoreMultiplier * buildSize;
                        maxCaptureRate += buildSize * buildingMulitpler;
                    }
                    else
                    {
                        captureRate += buildingMulitpler;
                        maxCaptureRate += buildingMulitpler;
                    }
                }
            }
        }
        if (possibleHpDamage > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::DealingHpDamage] = hpDamage / possibleHpDamage;
        }
        if (possibleFundsDamage > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::DealingFundsDamage] = fundsDamage / possibleFundsDamage;
        }
        if (counterAttackCount > 0 && unitValue > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::ReceivingFundsDamge]  = counterFundsDamage / (counterAttackCount * unitValue);
            data.unitBuildingDataInput[BuildingEntry::ReceivingHpDamage]    = counterhpDamage / counterAttackCount;
        }
        if (maxCaptureRate > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::CapturePotential] = captureRate / maxCaptureRate;
        }
        if (immuneUnits.size() > 0)
        {
            data.unitBuildingDataInput[BuildingEntry::CanAttackImmuneUnitRatio] = immuneUnit / static_cast<double>(immuneUnits.size());
        }
        data.unitBuildingDataInput[BuildingEntry::MaxUnitValue] = unitValue / m_maxUnitValue;
    }
}

float HeavyAi::getProductionScoreMultiplier(QPoint position, QPoint target, qint32 movementPoints)
{
    float scoreMultiplier = 1.0f;
    qint32 distance = GlobalUtils::getDistance(position, target);
    double turnDistance = distance / static_cast<double>(movementPoints);
    if (turnDistance > m_maxProductionTurnRange)
    {
        scoreMultiplier = m_maxProductionTurnRange / turnDistance;
    }
    return scoreMultiplier;
}

float HeavyAi::getBaseDamage(Unit* pAttacker, Unit* pDefender)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    float dmg = 0.0f;
    QString weaponId = pAttacker->getWeapon1ID();
    if (!weaponId.isEmpty())
    {
        dmg = pWeaponManager->getBaseDamage(weaponId, pDefender);
    }
    weaponId = pAttacker->getWeapon2ID();
    if (!weaponId.isEmpty())
    {
        float dmg2 = pWeaponManager->getBaseDamage(weaponId, pDefender);
        if (dmg2 > dmg)
        {
            dmg = dmg2;
        }
    }
    dmg *= pAttacker->getHp() / Unit::MAX_UNIT_HP;
    // cap damage
    if (dmg > pDefender->getHp() * Unit::MAX_UNIT_HP)
    {
        dmg = pDefender->getHp() * Unit::MAX_UNIT_HP;
    }
    return dmg;
}

QVector<double> HeavyAi::getGlobalBuildInfo(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                                            spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                            QVector<std::tuple<Unit*, Unit*>> & transportTargets)
{
    QVector<double> data(BuildingEntryMaxSize, 0.0);
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 infantryUnits = 0;
        qint32 indirectUnits = 0;
        qint32 directUnits = 0;
        qint32 transporterUnits = 0;
        GetOwnUnitCounts(pUnits, pEnemyUnits, pEnemyBuildings,
                         infantryUnits, indirectUnits, directUnits, transporterUnits,
                         transportTargets);
        double count = pUnits->size();
        if (count > 0)
        {
            data[DirectUnitRatio]       = static_cast<double>(directUnits)   / count;
            data[IndirectUnitRatio]     = static_cast<double>(indirectUnits) / count;
            data[InfantryUnitRatio]     = static_cast<double>(infantryUnits) / count;
            data[TransportUnitRatio]    = static_cast<double>(transporterUnits) / count;
        }
        if (pMap->getCurrentDay() >  m_earlyGameDays)
        {
            data[DayProgression] = 1.0 / static_cast<double>(pMap->getCurrentDay() - m_earlyGameDays);
        }
        else
        {
            data[DayProgression] = 1.0;
        }
        double enemeyCount = 0;
        double playerCount = 0;
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            if (!pMap->getPlayer(i)->getIsDefeated())
            {
                playerCount++;
                if (m_pPlayer->isEnemy(pMap->getPlayer(i)))
                {
                    enemeyCount++;
                }
            }
        }
        if (pEnemyBuildings->size() + pBuildings->size() > 0)
        {
            data[TotalBuildingRatio] = pBuildings->size() / static_cast<double>(pBuildings->size() + pEnemyBuildings->size());
        }
        if (playerCount > 0)
        {
            data[EnemyRatio] = static_cast<double>(enemeyCount) / playerCount;
        }
        double productionCount = 0;
        double unusedCount = 0;
        for (qint32 i = 0; i < pBuildings->size(); i++)
        {
            Building* pBuilding = pBuildings->at(i);
            if (pBuilding->isProductionBuilding())
            {
                productionCount++;
                if (pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    unusedCount++;
                    bool found = false;
                    for (auto & building : m_BuildingData)
                    {
                        if (building.m_pBuilding == pBuilding)
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        BuildingData newData;
                        newData.m_pBuilding = pBuilding;
                        m_BuildingData.append(newData);
                    }
                }
                else
                {
                    for (qint32 i = 0; i < m_BuildingData.size(); ++i)
                    {
                        if (m_BuildingData[i].m_pBuilding == pBuilding)
                        {
                            m_BuildingData.removeAt(i);
                            break;
                        }
                    }
                }
            }
        }
        if (productionCount > 0)
        {
            data[ProductionUsage] = static_cast<double>(unusedCount) / productionCount;
        }
    }
    return data;
}

void HeavyAi::getImmuneUnits(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, QVector<Unit*> & immuneUnits)
{
    immuneUnits.clear();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        bool attackable = false;
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            Unit* pEnemyUnit = pEnemyUnits->at(i2);
            auto dmg = getBaseDamage(pUnit, pEnemyUnit);

            if (dmg > m_notAttackableDamage || dmg >= pEnemyUnit->getHp())
            {
                attackable = true;
                break;
            }
        }
        if (!attackable)
        {
            immuneUnits.append(pEnemyUnits->at(i2));
        }
    }
}