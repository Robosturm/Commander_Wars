#include "ai/heavyai.h"

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"
#include "resource_management/weaponmanager.h"
#include "resource_management/movementtablemanager.h"

// code for building units is here

bool HeavyAi::buildUnits(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                         spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    scoreUnitBuildings(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings);
    double bestScore = std::numeric_limits<float>::lowest();
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
        CONSOLE_PRINT("HeavyAi::buildUnits " + item.buildingDataInput[item.m_selectedData].unitId + " with scored value " + QString::number(bestScore), Console::eDEBUG);
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
        CONSOLE_PRINT("HeavyAi::buildUnits build no units highest value was: " + QString::number(bestScore), Console::eDEBUG);
    }
    return false;
}

void HeavyAi::scoreUnitBuildings(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                                 spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
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
    qint32 coCount = m_pPlayer->getCoCount();
    qint32 maxCoCount = m_pPlayer->getMaxCoCount();
    for (qint32 i = 0; i < building.buildingDataInput.size(); ++i)
    {
        if (building.buildingDataInput[i].enabled)
        {
            qint32 x = building.m_pBuilding->Building::getX();
            qint32 y = building.m_pBuilding->Building::getY();
            auto score = m_neuralNetworks[NeuralNetworks::Production]->predict(building.buildingDataInput[i].unitBuildingDataInput);
            double value = score[0] * BaseGameInputIF::getUnitBuildValue(building.buildingDataInput[i].unitId);
            if (m_pPlayer->getCoCount() > 0)
            {
                Unit dummy(building.buildingDataInput[i].unitId, m_pPlayer, false, m_pMap);
                dummy.setVirtuellX(x);
                dummy.setVirtuellY(y);
                double bonusScore = 0;
                for (qint32 co = 0; co < maxCoCount; ++co)
                {
                    bonusScore += getAiCoUnitMultiplier(m_pPlayer->getCO(co), &dummy);
                }
                double modifier = value * (1 + bonusScore / (CO::MAX_CO_UNIT_VALUE * coCount));
                if (bonusScore > 0)
                {
                    value += modifier;
                }
                else
                {
                    value -= modifier;
                }
                // todo boost co indirect / direct ratio
            }
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
        building.m_action = spGameAction::create(CoreAI::ACTION_BUILD_UNITS, m_pMap);
        building.m_action->setTarget(QPoint(building.m_pBuilding->Building::getX(), building.m_pBuilding->Building::getY()));
        CoreAI::addMenuItemData(building.m_action, building.buildingDataInput[building.m_selectedData].unitId, building.buildingDataInput[building.m_selectedData].cost);
    }
}

void HeavyAi::createUnitBuildData(BuildingData & building, QVector<double> & data, qint32 funds, const QVector<Unit*> & immuneUnits,
                                  const QVector<std::tuple<Unit*, Unit*>> & transportTargets, spQmlVectorBuilding & pEnemyBuildings)
{
    // create new
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    spTerrain pDummyTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", m_pMap);
    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS, m_pMap);
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
                    Unit dummy(unitData.unitId, m_pPlayer, false, m_pMap);
                    dummy.setVirtuellX(x);
                    dummy.setVirtuellY(y);
                    spTerrain pDummyTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", m_pMap);
                    qint32 baseMovementCost = pMovementTableManager->getBaseMovementPoints(dummy.getMovementType(), pDummyTerrain.get(), pDummyTerrain.get(), &dummy);
                    if (baseMovementCost < 0)
                    {
                        baseMovementCost = 1;
                    }
                    qint32 movementPoints = dummy.getMovementpoints(QPoint(x, y)) / baseMovementCost;
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
    GameAction action = GameAction(ACTION_BUILD_UNITS, m_pMap);
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
    
    if (m_pMap != nullptr)
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
        data.unitBuildingDataInput[BuildingEntry::Movementpoints] = static_cast<double>(movementPoints) / static_cast<double>(m_maxMovementpoints);
        data.unitBuildingDataInput[BuildingEntry::VisionPotential] = pUnit->getVision(position) / m_maxVision;
        data.unitBuildingDataInput[BuildingEntry::MapMovementpoints] = movementPoints / static_cast<double>(m_pMap->getMapHeight() * m_pMap->getMapWidth());
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

void HeavyAi::getTransportInputVector(Building* pBuilding, Unit* pUnit, const QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                      spQmlVectorBuilding & pEnemyBuildings, qint32 movementPoints, UnitBuildData & data)
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
        data.unitBuildingDataInput[BuildingEntry::RequiredUnitsToTransportRatio] = requiredTransporterCount / static_cast<double>(transportTargets.size() + 1);
        data.unitBuildingDataInput[BuildingEntry::UnitsToTransportRatio] = transporterCount / static_cast<double>(m_pUnits->size() + 1);
    }
}

void HeavyAi::calculateUnitProductionDamage(Building* pBuilding, Unit* pUnit, qint32 movementPoints, QPoint position, UnitBuildData & data, const QVector<Unit*> & immuneUnits)
{
    
    if (m_pMap != nullptr)
    {
        qint32 mapWidth = m_pMap->getMapWidth();
        qint32 mapHeight = m_pMap->getMapHeight();
        qint32 unitValue = pUnit->getCoUnitValue();
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
                Terrain* pTerrain = m_pMap->getTerrain(x, y);
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
                        double enemyUnitValue = pEnemyUnit->getCoUnitValue();
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
    float dmg = pAttacker->getBaseDamage(pDefender);
    dmg *= pAttacker->getHp() / Unit::MAX_UNIT_HP;
    // cap damage
    if (dmg > pDefender->getHp() * Unit::MAX_UNIT_HP)
    {
        dmg = pDefender->getHp() * Unit::MAX_UNIT_HP;
    }
    return dmg;
}

QVector<double> HeavyAi::getGlobalBuildInfo(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                                            spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                            QVector<std::tuple<Unit*, Unit*>> & transportTargets)
{
    QVector<double> data(BuildingEntryMaxSize, 0.0);
    
    if (m_pMap != nullptr)
    {
        UnitCountData countData;
        GetOwnUnitCounts(pUnits, pEnemyUnits, pEnemyBuildings, countData);
        double count = pUnits->size() + 1;
        data[DirectUnitRatio]       = static_cast<double>(countData.directUnits)   / count;
        data[IndirectUnitRatio]     = static_cast<double>(countData.indirectUnits) / count;
        data[InfantryUnitRatio]     = static_cast<double>(countData.infantryUnits) / count;
        data[TransportUnitRatio]    = static_cast<double>(countData.transporterUnits) / count;
        data[SupplyUnitRatio]       = static_cast<double>(countData.supplyUnits) / count;
        data[SupplyRequiredRatio]   = static_cast<double>(countData.supplyNeededUnits) / count;
        if (m_pMap->getCurrentDay() >  m_earlyGameDays)
        {
            data[DayProgression] = 1.0 / static_cast<double>(m_pMap->getCurrentDay() - m_earlyGameDays);
        }
        else
        {
            data[DayProgression] = 1.0;
        }
        double enemeyCount = 0;
        double playerCount = 0;
        for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
        {
            if (!m_pMap->getPlayer(i)->getIsDefeated())
            {
                playerCount++;
                if (m_pPlayer->isEnemy(m_pMap->getPlayer(i)))
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
        double unusedCount = 0;
        float funds = m_pPlayer->getFunds();
        for (qint32 i = 0; i < pBuildings->size(); i++)
        {
            Building* pBuilding = pBuildings->at(i);
            if (pBuilding->isProductionBuilding())
            {
                if (pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    bool found = false;
                    for (auto & building : m_BuildingData)
                    {
                        if (building.m_pBuilding == pBuilding)
                        {
                            unusedCount++;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        auto buildList = pBuilding->getConstructionList();
                        for (auto & unitId : buildList)
                        {
                            Unit dummy(unitId, m_pPlayer, false, m_pMap);
                            if (m_pPlayer->getCosts(unitId, pBuilding->getPosition()) < funds && dummy.hasWeapons())
                            {
                                BuildingData newData;
                                newData.m_pBuilding = pBuilding;
                                m_BuildingData.append(newData);
                                unusedCount++;
                                break;
                            }
                        }
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
        if (m_BuildingData.size() > 0)
        {
            data[ProductionUsage] = static_cast<double>(unusedCount) / static_cast<double>(m_BuildingData.size());
        }
    }
    return data;
}

void HeavyAi::getImmuneUnits(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, QVector<Unit*> & immuneUnits)
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
