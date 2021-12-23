#include "QElapsedTimer"
#include <QSettings>

#include "coreengine/qmlvector.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"

#include "game/player.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/building.h"
#include "game/unitpathfindingsystem.h"

#include "ai/normalai.h"
#include "ai/targetedunitpathfindingsystem.h"

#include "resource_management/weaponmanager.h"
#include "resource_management/movementtablemanager.h"


NormalAi::NormalAi(QString configurationFile, GameEnums::AiTypes aiType)
    : CoreAI(aiType)
{
    setObjectName("NormalAi");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    m_iniData = { // General
                  {"MinMovementDamage", "General", &m_minMovementDamage, 0.3f, 0.0f, 1.0f},
                  {"MinAttackFunds", "General", &m_minAttackFunds, 0.0f, -1.0f, 1.0f},
                  {"MinSuicideDamage", "General", &m_minSuicideDamage, 0.75f, -1.0f, 1.0f},
                  {"SpamingFunds", "General", &m_spamingFunds, 7500.0f, 5000.0f, 10000.0f},
                  {"OwnUnitValue", "General", &m_ownUnitValue, 2.0f, -10.0f, 10.0f},
                  {"BuildingValue", "General", &m_buildingValue, 1.0f, 1.0f, 1.0f},
                  {"NotAttackableDamage", "General", &m_notAttackableDamage, 25.0f, 0.0f, 40.0f},
                  {"MidDamage", "General", &m_midDamage, 55.0f, 40.0f, 60.0f},
                  {"HighDamage", "General", &m_highDamage, 75.0f, 60.0f, 100.0f},
                  {"DirectIndirectRatio", "General", &m_directIndirectRatio, 5.0f, 0.2f, 10.0f},
                  {"MinSiloDamage", "General", &m_minSiloDamage, 7000.0f, 7000.0f, 7000.0f},
                  {"MinSameIslandDistance", "General", &m_minSameIslandDistance, 3.0f, 3.0f, 3.0f},
                  {"SlowUnitSpeed", "General", &m_slowUnitSpeed, 2.0f, 2.0f, 2.0f},
                  // CO Unit
                  {"CoUnitValue", "CoUnit", &m_coUnitValue, 6000.0f, 5000.0f, 10000.0f},
                  {"MinCoUnitScore", "CoUnit", &m_minCoUnitScore, 5000.0f, 3000.0f, 10000.0f},
                  {"CoUnitRankReduction", "CoUnit", &m_coUnitRankReduction, 1000.0f, 0.0f, 5000.0f},
                  {"CoUnitScoreMultiplier", "CoUnit", &m_coUnitScoreMultiplier, 1.1f, 1.0f, 3.0f},
                  {"MinCoUnitCount", "CoUnit", &m_minCoUnitCount, 5.0f, 1.0f, 10.0f},
                  // Repairing
                  {"MinUnitHealth", "Repairing", &m_minUnitHealth, 3.0f, 0.0f, 4.0f},
                  {"MaxUnitHealth", "Repairing", &m_maxUnitHealth, 7.0f, 0.0f, 10.0f},
                  {"FuelResupply", "Repairing", &m_fuelResupply, 0.33f, 0.0f, 0.5f},
                  {"AmmoResupply", "Repairing", &m_ammoResupply, 0.25f, 0.0f, 0.5f},
                  // Moving
                  {"LockedUnitHp", "Moving", &m_lockedUnitHp, 4.0f, 1.0f, 4.0f},
                  {"NoMoveAttackHp", "Moving", &m_noMoveAttackHp, 3.5f, 1.0f, 4.0f},
                  {"MinTerrainDamage", "Moving", &m_minTerrainDamage, 20.0f, 20.0f, 20.0f},

                  // Attacking
                  {"OwnIndirectAttackValue", "Attacking", &m_ownIndirectAttackValue, 2.0f, 0.1f, 10.0f},
                  {"EnemyKillBonus", "Attacking", &m_enemyKillBonus, 2.0f, 0.1f, 10.0f},
                  {"EnemyIndirectBonus", "Attacking", &m_enemyIndirectBonus, 3.0f, 0.1f, 10.0f},
                  {"AntiCaptureHqBonus", "Attacking", &m_antiCaptureHqBonus, 50.0f, 0.0f, 100.0f},
                  {"AntiCaptureBonus", "Attacking", &m_antiCaptureBonus, 21.0f, 0.0f, 100.0f},
                  {"AntiCaptureBonusScoreReduction", "Attacking", &m_antiCaptureBonusScoreReduction, 6.0f, 0.0f, 20.0f},
                  {"AntiCaptureBonusScoreDivider", "Attacking", &m_antiCaptureBonusScoreDivider, 2.0f, 0.1f, 10.0f},
                  {"EnemyCounterDamageMultiplier", "Attacking", &m_enemyCounterDamageMultiplier, 10.0f, 0.1f, 40.0f},
                  {"WatermineDamage", "Attacking", &m_watermineDamage, 4.0f, 4.0f, 4.0f},
                  {"EnemyUnitCountDamageReductionMultiplier", "Attacking", &m_enemyUnitCountDamageReductionMultiplier, 0.5f, 0.01f, 10.0f},
                  {"OwnProdctionMalus", "Attacking", &m_ownProdctionMalus, 5000.0f, 5000.0f, 5000.0f},
                  {"SupportDamageBonus", "Attacking", &m_supportDamageBonus, 1.0f, 0.1f, 10.0f},
                  // Production
                  {"FundsPerBuildingFactorA", "Production", &m_fundsPerBuildingFactorA, 1.85f, 0.5f, 10.0f},
                  {"FundsPerBuildingFactorB", "Production", &m_fundsPerBuildingFactorB, 2.0f, 0.5f, 10.0f},
                  {"FundsPerBuildingFactorC", "Production", &m_fundsPerBuildingFactorC, 3.0f, 1.0f, 10.0f},
                  {"FundsPerBuildingFactorD", "Production", &m_fundsPerBuildingFactorD, 3.0f, 1.0f, 10.0f},
                  {"OwnUnitEnemyUnitRatioAverager", "Production", &m_ownUnitEnemyUnitRatioAverager, 10.0f, 5.0f, 30.0f},
                  {"MaxDayScoreVariancer", "Production", &m_maxDayScoreVariancer, 10.0f, 5.0f, 30.0f},
                  {"StartDayScoreVariancer", "Production", &m_startDayScoreVariancer, 5.0f, 0.0f, 15.0f},
                  {"DirectIndirectUnitBonusFactor", "Production", &m_directIndirectUnitBonusFactor, 1.75f, 0.1f, 10.0f},
                  {"ScoringCutOffDamageHigh", "Production", &m_scoringCutOffDamageHigh, 100.0f, 100.0f, 100.0f},
                  {"ScoringCutOffDamageLow", "Production", &m_scoringCutOffDamageLow, 7.5f, 0.1f, 40.0f},
                  {"SmoothingValue", "Production", &m_smoothingValue, 3.0f, 1.0f, 20.0f},
                  {"MaxDistanceMultiplier", "Production", &m_maxDistanceMultiplier, 1.5f, 1.0f, 10.0f},
                  {"SameIslandBonusInRangeDays", "Production", &m_sameIslandBonusInRangeDays, 2.0f, 1.0f, 10.0f},
                  {"SameIslandOutOfDayMalusFactor", "Production", &m_sameIslandOutOfDayMalusFactor, 0.2f, 0.01f, 1.0f},
                  {"HighDamageBonus", "Production", &m_highDamageBonus, 2.0f, 0.0f, 10.0f},
                  {"MidDamageBonus", "Production", &m_midDamageBonus, 1.5f, 0.0f, 10.0f},
                  {"LowDamageBonus", "Production", &m_lowDamageBonus, 1.0f, 0.0f, 10.0f},
                  {"VeryLowDamageBonus", "Production", &m_veryLowDamageBonus, 0.5f, 0.0f, 10.0f},
                  {"TransportBonus", "Production", &m_transportBonus, 0.125f, 0.0f, 10.0f},
                  {"CurrentlyNotAttackableBonus", "Production", &m_currentlyNotAttackableBonus, 0.5f, 0.0f, 10.0f},
                  {"DifferentIslandBonusInRangeDays", "Production", &m_differentIslandBonusInRangeDays, 1.0f, 1.0f, 1.0f},
                  {"DifferentIslandOutOfDayMalusFactor", "Production", &m_differentIslandOutOfDayMalusFactor, 0.33f, 0.33f, 0.33f},
                  {"NoTransporterBonus", "Production", &m_noTransporterBonus, 70.0f, 70.0f, 70.0f},
                  {"TransporterToRequiredPlaceFactor", "Production", &m_transporterToRequiredPlaceFactor, 6.0f, 6.0f, 6.0f},
                  {"MinFlyingTransportScoreForBonus", "Production", &m_minFlyingTransportScoreForBonus, 15.0f, 15.0f, 15.0f},
                  {"FlyingTransporterBonus", "Production", &m_flyingTransporterBonus, 15.0f, 15.0f, 15.0f},
                  {"SmallTransporterBonus", "Production", &m_smallTransporterBonus, 30.0f, 30.0f, 30.0f},
                  {"UnitToSmallTransporterRatio", "Production", &m_unitToSmallTransporterRatio, 5.0f, 5.0f, 5.0f},
                  {"AdditionalLoadingUnitBonus", "Production", &m_additionalLoadingUnitBonus, 10.0f, 10.0f, 10.0f},
                  {"IndirectUnitAttackCountMalus", "Production", &m_indirectUnitAttackCountMalus, 4.0f, 0.0f, 20.0f},
                  {"MinAttackCountBonus", "Production", &m_minAttackCountBonus, 5.0f, 0.0f, 20.0f},
                  {"LowIndirectUnitBonus", "Production", &m_lowIndirectUnitBonus, 30.0f, 0.0f, 100.0f},
                  {"LowIndirectMalus", "Production", &m_lowIndirectMalus, 30.0f, 0.0f, 100.0f},
                  {"HighIndirectMalus", "Production", &m_highIndirectMalus, 40.0f, 0.0f, 100.0f},
                  {"LowDirectUnitBonus", "Production", &m_highIndirectMalus, 35.0f, 0.0f, 100.0f},
                  {"LowDirectMalus", "Production", &m_lowDirectMalus, 20.0f, 0.0f, 100.0f},
                  {"HighDirectMalus", "Production", &m_highDirectMalus, 40.0f, 0.0f, 100.0f},
                  {"MinUnitCountForDamageBonus", "Production", &m_minUnitCountForDamageBonus, 3.0f, 0.0f, 20.0f},
                  {"CurrentlyNotAttackableScoreBonus", "Production", &m_currentlyNotAttackableScoreBonus, 30.0f, 0.0f, 100.0f},
                  {"CoUnitBuffBonus", "Production", &m_coUnitBuffBonus, 17.0f, 5.0f, 100.0f},
                  {"NearEnemyBonus", "Production", &m_nearEnemyBonus, 10.0f, 0.0f, 100.0f},
                  {"MovementpointBonus", "Production", &m_movementpointBonus, 6.0f, 0.0f, 30.0f},
                  {"DamageToUnitCostRatioBonus", "Production", &m_damageToUnitCostRatioBonus, 20.0f, 0.0f, 100.0f},
                  {"SuperiorityRatio", "Production", &m_superiorityRatio, 1.8f, 1.0f, 10.0f},
                  {"CheapUnitRatio", "Production", &m_cheapUnitRatio, 1.8f, 0.5f, 2.0f},
                  {"NormalUnitRatio", "Production", &m_normalUnitRatio, 1.0f, 0.8f, 1.2f},
                  {"CheapUnitValue", "Production", &m_cheapUnitValue, 3000.0f, 1500.0f, 5000.0f},
                  {"CheapUnitBonusMultiplier", "Production", &m_cheapUnitBonusMultiplier, 45.0f, 0.0f, 100.0f},
                  {"NormalUnitBonusMultiplier", "Production", &m_normalUnitBonusMultiplier, 45.0f, 0.0f, 100.0f},
                  {"ExpensiveUnitBonusMultiplier", "Production", &m_expensiveUnitBonusMultiplier, 20.0f, 0.0f, 100.0f},
                  {"LowOwnBuildingEnemyBuildingRatio", "Production", &m_lowOwnBuildingEnemyBuildingRatio, 1.25f, 0.0f, 10.0f},
                  {"LowInfantryRatio", "Production", &m_lowInfantryRatio, 0.4f, 0.0f, 1.0f},
                  {"LowIncomeInfantryBonusMultiplier", "Production", &m_lowIncomeInfantryBonusMultiplier, 50.0f, 0.0f, 100.0f},
                  {"ProducingTransportSearchrange", "Production", &m_ProducingTransportSearchrange, 6.0f, 6.0f, 6.0f},
                  {"ProducingTransportSizeBonus", "Production", &m_ProducingTransportSizeBonus, 15.0f, 15.0f, 15.0f},
                  {"ProducingTransportRatioBonus", "Production", &m_ProducingTransportRatioBonus, 10.0f, 10.0f, 10.0f},
                  {"ProducingTransportLoadingBonus", "Production", &m_ProducingTransportLoadingBonus, 15.0f, 15.0f, 15.0f},
                  {"ProducingTransportMinLoadingTransportRatio", "Production", &m_ProducingTransportMinLoadingTransportRatio, 4.5f, 2.0f, 7.0f},
                  {"BuildingBonusMultiplier", "Production", &m_buildingBonusMultiplier, 6.0f, 0.0f, 30.0f},
                  {"MinInfantryCount", "Production", &m_minInfantryCount, 2.0f, 0.0f, 10.0f},
                  {"CanSupplyBonus", "Production", &m_canSupplyBonus, 10.0f, 5.0f, 40.0f},
                  {"MaxSupplyUnitRatio", "Production", &m_maxSupplyUnitRatio, 0.05f, 0.01f, 0.05f},
                  {"AverageSupplySupport", "Production", &m_averageSupplySupport, 8.0f, 0.0f, 20.0f},
                  {"CappingFunds", "Production", &m_cappingFunds, 6500.0f, 2000.0f, 8000.0f},
                  {"CappedFunds", "Production", &m_cappedFunds, 1500.0f, 500.0f, 4000.0f},
                  {"TargetPriceDifference", "Production", &m_targetPriceDifference, 0.35f, 0.0f, 1.0f},
                  {"MaxCloseDistanceDamageBonus", "Production", &m_maxCloseDistanceDamageBonus, 1.0f, 1.1f, 10.0f},
                  {"MinCloseDistanceDamageBonus", "Production", &m_minCloseDistanceDamageBonus, 1.0f, 1.1f, 10.0f},
                  {"SameFundsMatchUpBonus", "Production", &m_sameFundsMatchUpBonus, 16.0f, 10.0f, 60.0f},
                  {"AttackCountBonus", "Production", &m_attackCountBonus, 25.0f, 5.0f, 60.0f},
                  {"MaxOverkillBonus", "Production", &m_maxOverkillBonus, 2.0f, 1.5f, 10.0f},
                  {"CounterDamageBonus", "Production", &m_counterDamageBonus, 25.0f, 1.0f, 100.0f},
                  {"EarlyGame", "Production", &m_earlyGame, 5.0f, 2.f, 10.0f},};

    loadIni( "normal/" + configurationFile);
}

void NormalAi::readIni(QString name)
{
    if (QFile::exists(name))
    {
        QSettings settings(name, QSettings::IniFormat);
        CONSOLE_PRINT("NormalAi::readIni status=" + QString::number(settings.status()), Console::eDEBUG);
        QString lastGroup = "";
        for (auto & entry : m_iniData)
        {
            bool ok = false;
            if (entry.m_group != lastGroup)
            {
                if (!lastGroup.isEmpty())
                {
                    settings.endGroup();
                }
                settings.beginGroup(entry.m_group);
                lastGroup = entry.m_group;
            }
            *entry.m_value = settings.value(entry.m_name, entry.m_defaultValue).toDouble(&ok);
            if (!ok)
            {
                *entry.m_value = entry.m_defaultValue;
            }
        }
        settings.endGroup();
    }
}

void NormalAi::saveIni(QString name) const
{
    QSettings settings(name, QSettings::IniFormat);
    CONSOLE_PRINT("NormalAi::saveIni status=" + QString::number(settings.status()), Console::eDEBUG);
    QString lastGroup = "";
    for (auto & entry : m_iniData)
    {
        bool ok = false;
        if (entry.m_group != lastGroup)
        {
            if (!lastGroup.isEmpty())
            {
                settings.endGroup();
            }
            settings.beginGroup(entry.m_group);
            lastGroup = entry.m_group;
        }
        settings.setValue(entry.m_name, *entry.m_value);
    }
    settings.endGroup();
}

void NormalAi::randomizeIni(QString name, float chance, float mutationRate)
{
    for (auto & entry : m_iniData)
    {
        if (GlobalUtils::randFloat(0.0f, 1.0f) < chance)
        {
            if (qAbs(*entry.m_value) <= 0.05f)
            {
                qint32 rand = GlobalUtils::randInt(-1, 1);
                if (rand == 0)
                {
                    *entry.m_value = 0.0f;
                }
                else if (rand > 0)
                {
                    *entry.m_value = 0.075f;
                }
                else if (rand < 0)
                {
                    *entry.m_value = -0.075f;
                }
            }
            else
            {
                qint32 rand = GlobalUtils::randInt(0, 1);
                if (rand == 0)
                {
                    *entry.m_value -= *entry.m_value * mutationRate;
                }
                else
                {
                    *entry.m_value += *entry.m_value * mutationRate;
                }
            }
        }
        if (*entry.m_value < entry.m_minValue)
        {
            *entry.m_value = entry.m_minValue;
        }
        else if (*entry.m_value > entry.m_maxValue)
        {
            *entry.m_value = entry.m_maxValue;
        }
    }
    saveIni(name);
}

void NormalAi::process()
{
    spQmlVectorBuilding pBuildings = spQmlVectorBuilding(m_pPlayer->getBuildings());
    pBuildings->randomize();
    spQmlVectorUnit pUnits;
    spQmlVectorUnit pEnemyUnits;
    spQmlVectorBuilding pEnemyBuildings;
    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= m_minSiloDamage);

    if (useBuilding(pBuildings)){}
    else
    {
        pUnits = m_pPlayer->getUnits();
        pUnits->sortShortestMovementRange(true);
        pEnemyUnits = m_pPlayer->getEnemyUnits();
        pEnemyUnits->randomize();
        pEnemyBuildings = m_pPlayer->getEnemyBuildings();
        pEnemyBuildings->randomize();
        updateAllUnitData(pUnits);
        if (useCOPower(pUnits, pEnemyUnits))
        {
            clearEnemyData();
        }
        else
        {
            turnMode = GameEnums::AiTurnMode_DuringDay;
            if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
            else
            {
                if (m_aiStep == AISteps::buildUnits)
                {
                    m_aiStep = AISteps::moveUnits;
                }
                if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
                else
                {
                    if (m_aiStep == AISteps::buildUnits)
                    {
                        m_aiStep = AISteps::moveUnits;
                    }
                    clearEnemyData();
                    m_IslandMaps.clear();
                    turnMode = GameEnums::AiTurnMode_EndOfDay;
                    if (useCOPower(pUnits, pEnemyUnits))
                    {
                        m_usedTransportSystem = false;
                        turnMode = GameEnums::AiTurnMode_DuringDay;
                    }
                    else
                    {
                        turnMode = GameEnums::AiTurnMode_StartOfDay;
                        finishTurn();
                    }
                }
            }
        }
    }
}

void NormalAi::finishTurn()
{
    CONSOLE_PRINT("NormalAi::finishTurn()", Console::eDEBUG);
    m_productionData.clear();
    CoreAI::finishTurn();
}

bool NormalAi::performActionSteps(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits,
                                  spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    if (m_aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveFlares(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && captureBuildings(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    // indirect units
    else if (m_aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 2, std::numeric_limits<qint32>::max(), pBuildings, pEnemyBuildings)){}
    // direct units
    else if (m_aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 1, 1, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveUnits && repairUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveToTargets && refillUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, 1)){}
    else if (m_aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 2, std::numeric_limits<qint32>::max())){}
    else if (m_aiStep <= AISteps::loadUnits && !m_usedTransportSystem && loadUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveTransporters && !m_usedTransportSystem && moveTransporters(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
    else
    {
        if (!m_usedTransportSystem)
        {
            m_usedTransportSystem = true;
            m_aiStep = AISteps::moveUnits;
            return performActionSteps(pUnits, pEnemyUnits,  pBuildings, pEnemyBuildings);
        }
        else if (m_aiStep <= AISteps::loadUnits && loadUnits(pUnits, pBuildings, pEnemyBuildings)){}
        else if (m_aiStep <= AISteps::moveSupportUnits && moveSupport(AISteps::moveSupportUnits, pUnits, true)){}
        else if (m_aiStep <= AISteps::moveSupportUnits && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, std::numeric_limits<qint32>::max(), true)){}
        else if (m_aiStep <= AISteps::moveAway && moveAwayFromProduction(pUnits)){}
        else if (m_aiStep <= AISteps::buildUnits && buildUnits(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings)){}
        else
        {
            return false;
        }
    }
    return true;
}

bool NormalAi::isUsingUnit(Unit* pUnit)
{
    if (needsRefuel(pUnit))
    {
        return false;
    }
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(pUnit->Unit::getX(), pUnit->Unit::getY()))
    {
        Building* pBuilding = pMap->getTerrain(pUnit->Unit::getX(), pUnit->Unit::getY())->getBuilding();
        if (pBuilding == nullptr && pUnit->getHpRounded() <= m_minUnitHealth)
        {
            return false;
        }
        else if (pBuilding != nullptr && pBuilding->getOwner() == m_pPlayer &&
                 pUnit->getHpRounded() <= m_maxUnitHealth)
        {
            return false;
        }
    }
    if (pUnit->getHasMoved())
    {
        return false;
    }
    return true;
}

bool NormalAi::captureBuildings(spQmlVectorUnit & pUnits)
{
    CONSOLE_PRINT("NormalAi::captureBuildings()", Console::eDEBUG);
    QVector<QVector3D> captureBuildings;
    qint32 cost = 0;
    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
    bool fireSilos = hasMissileTarget();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() && pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            if (pUnit->getCapturePoints() > 0)
            {
                spGameAction pAction = spGameAction::create(ACTION_CAPTURE);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            else
            {
                GameAction action(ACTION_CAPTURE);
                action.setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                UnitPathFindingSystem pfs(pUnit);
                pfs.explore();
                QVector<QPoint> targets = pfs.getAllNodePoints();
                for (qint32 i2 = 0; i2 < targets.size(); i2++)
                {
                    action.setActionID(ACTION_CAPTURE);
                    action.setMovepath(QVector<QPoint>(1, targets[i2]), 0);
                    if (action.canBePerformed())
                    {
                        captureBuildings.append(QVector3D(targets[i2].x(), targets[i2].y(), i));
                    }
                    else
                    {
                        action.setActionID(ACTION_MISSILE);
                        if (action.canBePerformed() && fireSilos)
                        {
                            captureBuildings.append(QVector3D(targets[i2].x(), targets[i2].y(), i));
                        }
                    }
                }
            }
        }
    }
    if (captureBuildings.size() > 0)
    {
        spGameMap pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            if (!pUnit->getHasMoved() && pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                QVector<QVector3D> captures;
                for (qint32 i2 = 0; i2 < captureBuildings.size(); i2++)
                {
                    if (static_cast<qint32>(captureBuildings[i2].z()) == i)
                    {
                        captures.append(captureBuildings[i2]);
                    }
                }
                bool perform = false;
                qint32 targetIndex = 0;
                bool productionBuilding = false;
                if (captures.size() > 0)
                {
                    if (captures.size() == 0)
                    {
                        // we have only one target go for it
                        targetIndex = 0;
                        perform = true;
                    }
                    else
                    {
                        // check if we have a building only we can capture and capture it
                        for (qint32 i2 = 0; i2 < captures.size(); i2++)
                        {
                            qint32 captureCount = 0;
                            for (qint32 i3 = 0; i3 < captureBuildings.size(); i3++)
                            {
                                if (static_cast<qint32>(captureBuildings[i3].x()) == static_cast<qint32>(captures[i2].x()) &&
                                    static_cast<qint32>(captureBuildings[i3].y()) == static_cast<qint32>(captures[i2].y()))
                                {
                                    captureCount++;
                                }
                            }
                            bool isProductionBuilding = pMap->getTerrain(static_cast<qint32>(captures[i2].x()), static_cast<qint32>(captures[i2].y()))->getBuilding()->getActionList().contains(ACTION_BUILD_UNITS);
                            if ((captureCount == 1 && perform == false) ||
                                (captureCount == 1 && productionBuilding == false && perform == true && isProductionBuilding))
                            {
                                productionBuilding = isProductionBuilding;
                                targetIndex = i2;
                                perform = true;
                            }
                        }
                        // check if there unique captures open
                        bool skipUnit = false;
                        for (qint32 i2 = 0; i2 < captureBuildings.size(); i2++)
                        {
                            qint32 captureCount = 0;
                            for (qint32 i3 = 0; i3 < captureBuildings.size(); i3++)
                            {
                                if (static_cast<qint32>(captureBuildings[i3].x()) == static_cast<qint32>(captureBuildings[i2].x()) &&
                                    static_cast<qint32>(captureBuildings[i3].y()) == static_cast<qint32>(captureBuildings[i2].y()))
                                {
                                    captureCount++;
                                }
                            }
                            if (captureCount == 1)
                            {
                                skipUnit = true;
                            }
                        }
                        // if not we can select a target from the list
                        if (!skipUnit)
                        {
                            targetIndex = 0;
                            qint32 prio = -1;
                            // priorities production buildings over over captures
                            for (qint32 i2 = 0; i2 < captures.size(); i2++)
                            {
                                Building* pBuilding = pMap->getTerrain(static_cast<qint32>(captures[i2].x()), static_cast<qint32>(captures[i2].y()))->getBuilding();
                                qint32 testPrio = std::numeric_limits<qint32>::min();
                                if (pBuilding->getBuildingID() == "HQ")
                                {
                                    testPrio = std::numeric_limits<qint32>::max();
                                }
                                else if (pBuilding->getActionList().contains(ACTION_BUILD_UNITS))
                                {
                                    testPrio = pBuilding->getConstructionList().size();
                                }
                                if (testPrio > prio)
                                {
                                    targetIndex = i2;
                                    prio = testPrio;
                                }
                            }
                            perform = true;
                        }
                    }
                }
                // perform capturing
                if (perform)
                {
                    UnitPathFindingSystem pfs(pUnit);
                    pfs.explore();
                    spGameAction pAction = spGameAction::create(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    QVector<QPoint> path = pfs.getPath(static_cast<qint32>(captures[targetIndex].x()), static_cast<qint32>(captures[targetIndex].y()));
                    pAction->setMovepath(path, pfs.getCosts(path));
                    m_updatePoints.append(pUnit->getPosition());
                    m_updatePoints.append(pAction->getActionTarget());
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                    else
                    {
                        CoreAI::addSelectedFieldData(pAction, rocketTarget);
                        pAction->setActionID(ACTION_MISSILE);
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool NormalAi::fireWithUnits(spQmlVectorUnit & pUnits, qint32 minfireRange, qint32 maxfireRange,
                             spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::fireWithUnits()", Console::eDEBUG);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() &&
            pUnit->getBaseMaxRange() >= minfireRange &&
            pUnit->getBaseMaxRange() <= maxfireRange &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0) &&
            pUnit->hasAction(CoreAI::ACTION_FIRE))
        {
            spGameAction pAction = spGameAction::create(ACTION_FIRE);
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            UnitPathFindingSystem pfs(pUnit);
            pfs.explore();
            QVector<CoreAI::DamageData> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, pAction, &pfs, ret, moveTargetFields);
            qint32 targetIdx = getBestAttackTarget(pUnit, pUnits, ret, moveTargetFields, pBuildings, pEnemyBuildings);
            if (targetIdx >= 0)
            {
                CoreAI::DamageData target = ret[targetIdx];
                QVector<QPoint> path = pfs.getPath(static_cast<qint32>(moveTargetFields[targetIdx].x()),
                                                   static_cast<qint32>(moveTargetFields[targetIdx].y()));
                pAction->setMovepath(path, pfs.getCosts(path));
                CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x), static_cast<qint32>(target.y)));
                if (GameMap::getInstance()->getTerrain(static_cast<qint32>(target.x), static_cast<qint32>(target.y))->getUnit() == nullptr)
                {
                    m_IslandMaps.clear();
                }
                if (pAction->isFinalStep() && pAction->canBePerformed())
                {
                    m_updatePoints.append(pUnit->getPosition());
                    m_updatePoints.append(pAction->getActionTarget());
                    m_updatePoints.append(QPoint(static_cast<qint32>(target.x), static_cast<qint32>(target.y)));
                    emit performAction(pAction);
                    return true;
                }
            }
        }
    }
    return false;
}

bool NormalAi::refillUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::refillUnits()", Console::eDEBUG);
    if (m_aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    m_aiStep = AISteps::moveToTargets;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (isUsingUnit(pUnit) &&
            pUnit->getLoadedUnitCount() == 0)
        {
            QStringList actions = pUnit->getActionList();
            if (isRefuelUnit(actions))
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                UnitPathFindingSystem pfs(pUnit);
                pfs.explore();
                bool found = false;
                QPoint moveTarget;
                if (actions.contains(ACTION_SUPPORTALL_RATION))
                {
                    pAction->setActionID(ACTION_SUPPORTALL_RATION);
                    QPoint refillTarget;
                    found = getBestRefillTarget(pfs, 4, moveTarget, refillTarget);
                }
                else if (actions.contains(ACTION_SUPPORTALL_RATION))
                {
                    pAction->setActionID(ACTION_SUPPORTALL_RATION_MONEY);
                    QPoint refillTarget;
                    found = getBestRefillTarget(pfs, 4, moveTarget, refillTarget);
                }
                else if (actions.contains(ACTION_SUPPORTSINGLE_REPAIR))
                {
                    pAction->setActionID(ACTION_SUPPORTSINGLE_REPAIR);
                    QPoint refillTarget;
                    found = getBestRefillTarget(pfs, 1, moveTarget, refillTarget);
                    CoreAI::addSelectedFieldData(pAction, refillTarget);
                }
                else if (actions.contains(ACTION_SUPPORTSINGLE_FREEREPAIR))
                {
                    pAction->setActionID(ACTION_SUPPORTSINGLE_FREEREPAIR);
                    QPoint refillTarget;
                    found = getBestRefillTarget(pfs, 1, moveTarget, refillTarget);
                    CoreAI::addSelectedFieldData(pAction, refillTarget);
                }
                else if (actions.contains(ACTION_SUPPORTSINGLE_SUPPLY))
                {
                    pAction->setActionID(ACTION_SUPPORTSINGLE_SUPPLY);
                    QPoint refillTarget;
                    found = getBestRefillTarget(pfs, 1, moveTarget, refillTarget);
                    CoreAI::addSelectedFieldData(pAction, refillTarget);
                }
                if (found)
                {
                    QVector<QPoint> path = pfs.getPath(moveTarget.x(), moveTarget.y());
                    pAction->setMovepath(path, pfs.getCosts(path));
                    if (pAction->canBePerformed())
                    {
                        m_updatePoints.append(pUnit->getPosition());
                        m_updatePoints.append(pAction->getActionTarget());
                        emit performAction(pAction);
                        return true;
                    }
                }
                else if (m_usedTransportSystem)
                {
                    CONSOLE_PRINT("move to supply needed units", Console::eDEBUG);
                    QVector<QVector3D> targets;
                    QVector<QVector3D> transporterTargets;
                    pAction->setActionID(ACTION_WAIT);
                    appendRefillTargets(actions, pUnit, pUnits, targets);
                    if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool NormalAi::getBestRefillTarget(UnitPathFindingSystem & pfs, qint32 maxRefillCount, QPoint & moveTarget, QPoint & refillTarget) const
{
    bool ret = false;
    const auto points = pfs.getAllNodePoints();
    spGameMap pMap = GameMap::getInstance();
    spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    qint32 highestCount = 0;
    for (const auto & point : points)
    {
        if (pMap->getTerrain(point.x(), point.y())->getUnit() == nullptr)
        {
            qint32 count = 0;
            for (qint32 i = 0; i < circle->size(); ++i)
            {
                qint32 x = point.x() + circle->at(i).x();
                qint32 y = point.y() + circle->at(i).y();
                if (pMap->onMap(x, y))
                {
                    Unit* pSupplyUnit = pMap->getTerrain(x, y)->getUnit();
                    if (pSupplyUnit != nullptr &&
                        pSupplyUnit->getOwner() == m_pPlayer &&
                        needsRefuel(pSupplyUnit))
                    {
                        ++count;
                        ret = true;
                        if (count == maxRefillCount)
                        {
                            moveTarget = point;
                            refillTarget = QPoint(x, y);
                            break;
                        }
                    }
                }
            }
            if (count == maxRefillCount)
            {
                break;
            }
            else if (count > highestCount)
            {
                moveTarget = point;
                highestCount = count;
            }
        }
    }
    return ret;
}

void NormalAi::appendRefillTargets(const QStringList & actions, Unit* pUnit, spQmlVectorUnit & pUnits, QVector<QVector3D>& targets) const
{
    if (isRefuelUnit(actions))
    {
        spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        spGameMap pMap = GameMap::getInstance();
        qint32 islandIdx = getIslandIndex(pUnit);
        qint32 curX = pUnit->Unit::getX();
        qint32 curY = pUnit->Unit::getY();
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pSupplyUnit = pUnits->at(i);
            if (needsRefuel(pSupplyUnit))
            {
                qint32 unitX = pSupplyUnit->Unit::getX();
                qint32 unitY = pSupplyUnit->Unit::getY();
                for (qint32 i2 = 0; i2 < circle->size(); ++i2)
                {
                    qint32 x = unitX + circle->at(i2).x();
                    qint32 y = unitY + circle->at(i2).y();
                    if (pMap->onMap(x, y))
                    {
                        if (onSameIsland(islandIdx, curX, curY, x, y))
                        {
                            if (!targets.contains(QVector3D(x, y, 1)))
                            {
                                targets.append(QVector3D(x, y, 1));
                            }
                        }
                    }
                }
            }
        }
    }
}

bool NormalAi::moveUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings,
                         spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                         qint32 minfireRange, qint32 maxfireRange, bool supportUnits)
{
    CONSOLE_PRINT("NormalAi::moveUnits()", Console::eDEBUG);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if ((isUsingUnit(pUnit) || m_usedTransportSystem) &&
            !pUnit->getHasMoved() &&
            pUnit->getBaseMaxRange() >= minfireRange &&
            pUnit->getBaseMaxRange() <= maxfireRange &&
            pUnit->hasWeapons() && pUnit->getLoadedUnitCount() == 0)
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            spGameAction pAction = spGameAction::create(ACTION_WAIT);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));

            // find some cool targets
            appendCaptureTargets(actions, pUnit, pEnemyBuildings, targets);
            if (targets.size() > 0)
            {
                appendCaptureTransporterTargets(pUnit, pUnits, pEnemyBuildings, transporterTargets);
                targets.append(transporterTargets);
            }
            appendAttackTargets(pUnit, pEnemyUnits, targets);
            appendAttackTargetsIgnoreOwnUnits(pUnit, pEnemyUnits, targets);
            appendTerrainBuildingAttackTargets(pUnit, pEnemyBuildings, targets);
            if (targets.size() == 0)
            {
                appendRepairTargets(pUnit, pBuildings, targets);
            }
            if (supportUnits)
            {
                appendSupportTargets(actions, pUnit, pUnits, pEnemyUnits, targets);
            }
            if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
            {
                return true;
            }
        }
    }
    return false;
}

bool NormalAi::loadUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::loadUnits()", Console::eDEBUG);
    m_aiStep = AISteps::loadUnits;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved())
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            spGameAction pAction = spGameAction::create(ACTION_LOAD);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));

            // find some cool targets
            appendTransporterTargets(pUnit, pUnits, transporterTargets);
            targets.append(transporterTargets);
            // till now the selected targets are a little bit lame cause we only search for reachable transporters
            // but not for reachable loading places.
            if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
            {
                return true;
            }
        }
    }
    return false;
}

bool NormalAi::moveTransporters(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::moveTransporters()", Console::eDEBUG);
    m_aiStep = AISteps::moveTransporters;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getLoadingPlace() > 0)
        {
            // wooohooo it's a transporter
            if (pUnit->getLoadedUnitCount() > 0)
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                // find possible targets for this unit
                QVector<QVector3D> targets;
                // can one of our units can capture buildings?
                for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
                {
                    Unit* pLoaded = pUnit->getLoadedUnit(i);
                    if (pLoaded->getActionList().contains(ACTION_CAPTURE))
                    {
                        appendUnloadTargetsForCapturing(pUnit, pEnemyBuildings, targets);
                        break;
                    }
                }
                // if not find closest unloading field
                if (targets.size() == 0 || pUnit->getLoadedUnitCount() > 1)
                {
                    appendNearestUnloadTargets(pUnit, pEnemyUnits, pEnemyBuildings, targets);
                }
                if (moveToUnloadArea(pAction, pUnit, pUnits, actions, targets, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
            }
            else
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                // we need to move to a loading place
                QVector<QVector3D> targets;
                QVector<QVector3D> transporterTargets;
                appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, false, targets);
                if (targets.size() == 0)
                {
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, false, targets);
                }
                if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool NormalAi::moveToUnloadArea(spGameAction & pAction, Unit* pUnit, spQmlVectorUnit & pUnits, QStringList& actions,
                                QVector<QVector3D>& targets,
                                spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::moveToUnloadArea()", Console::eDEBUG);
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();
        QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
        pAction->setMovepath(path, turnPfs.getCosts(path));
        pAction->setActionID(ACTION_UNLOAD);
        if (pAction->canBePerformed())
        {
            return unloadUnits(pAction, pUnit);
        }
        else
        {
            return moveUnit(pAction, pUnit, pUnits, actions, targets, targets, true, pBuildings, pEnemyBuildings);
        }
    }
    return false;
}

bool NormalAi::unloadUnits(spGameAction & pAction, Unit* pUnit)
{
    spGameMap pMap = GameMap::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool unloaded = false;
    QVector<qint32> unloadedUnits;
    do
    {
        unloaded = false;
        spMenuData pDataMenu = pAction->getMenuStepData();
        if (pDataMenu->validData())
        {
            QStringList actions = pDataMenu->getActionIDs();
            QVector<qint32> unitIDx = pDataMenu->getCostList();
            QVector<QList<QVariant>> unloadFields;
            for (qint32 i = 0; i < unitIDx.size() - 1; i++)
            {
                QString function1 = "getUnloadFields";
                QJSValueList args1;
                QJSValue obj1 = pInterpreter->newQObject(pAction.get());
                args1 << obj1;
                args1 << unitIDx[i];
                QJSValue ret = pInterpreter->doFunction(ACTION_UNLOAD, function1, args1);
                unloadFields.append(ret.toVariant().toList());
            }
            if (actions.size() > 1)
            {
                for (qint32 i = 0; i < unloadFields.size(); i++)
                {
                    if (!needsRefuel(pUnit->getLoadedUnit(i)))
                    {
                        if (!unloadedUnits.contains(unitIDx[i]))
                        {
                            if (unloadFields[i].size() == 1)
                            {
                                addMenuItemData(pAction, actions[i], unitIDx[i]);
                                spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
                                addSelectedFieldData(pAction, pFields->getPoints()->at(0));
                                unloaded = true;
                                unloadedUnits.append(unitIDx[i]);
                                break;
                            }
                            else if (unloadFields[i].size() > 0 &&
                                     pUnit->getLoadedUnit(i)->getActionList().contains(ACTION_CAPTURE))
                            {
                                for (qint32 i2 = 0; i2 < unloadFields[i].size(); i2++)
                                {
                                    QPoint unloadField = unloadFields[i][i2].toPoint();
                                    Building* pBuilding = pMap->getTerrain(unloadField.x(),
                                                                           unloadField.y())->getBuilding();
                                    if (pBuilding != nullptr && m_pPlayer->isEnemy(pBuilding->getOwner()))
                                    {
                                        addMenuItemData(pAction, actions[i], unitIDx[i]);
                                        addSelectedFieldData(pAction, unloadField);
                                        unloaded = true;
                                        unloadedUnits.append(unitIDx[i]);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                if (unloaded == false &&
                    !needsRefuel(pUnit->getLoadedUnit(0)))
                {
                    qint32 costs = pDataMenu->getCostList()[0];
                    addMenuItemData(pAction, actions[0], costs);
                    unloaded = true;
                    spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
                    qint32 field = GlobalUtils::randIntBase(0, pFields->getPoints()->size() - 1);
                    addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                }
            }
        }
        else
        {
            CONSOLE_PRINT("Error invalid menu data received while unloading units",  Console::eERROR);
            return false;
        }
    }
    while (unloaded);
    addMenuItemData(pAction, ACTION_WAIT, 0);
    m_updatePoints.append(pUnit->getPosition());
    m_updatePoints.append(pAction->getActionTarget());
    if (pAction->canBePerformed())
    {
        emit performAction(pAction);
        return true;
    }
    return false;
}

bool NormalAi::repairUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::repairUnits()", Console::eDEBUG);
    m_aiStep = AISteps::moveUnits;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!isUsingUnit(pUnit) && !pUnit->getHasMoved())
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            spGameAction pAction = spGameAction::create(ACTION_WAIT);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            appendRepairTargets(pUnit, pBuildings, targets);
            if (needsRefuel(pUnit))
            {
                appendTransporterTargets(pUnit, pUnits, transporterTargets);
                targets.append(transporterTargets);
            }
            if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, false, pBuildings, pEnemyBuildings))
            {
                return true;
            }
            else
            {
                pAction = spGameAction::create(ACTION_WAIT);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                UnitPathFindingSystem turnPfs(pUnit);
                turnPfs.explore();
                if (suicide(pAction, pUnit, turnPfs))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool NormalAi::moveUnit(spGameAction & pAction, Unit* pUnit, spQmlVectorUnit & pUnits, QStringList& actions,
                        QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets,
                        bool shortenPathForTarget,
                        spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::moveUnit()", Console::eDEBUG);
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        spGameMap pMap = GameMap::getInstance();
        Unit* pTargetUnit = pMap->getTerrain(targetFields.x(), targetFields.y())->getUnit();
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();
        if (CoreAI::contains(transporterTargets, targetFields) &&
            pTargetUnit->getHasMoved() == false)
        {
            QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
            pAction->setMovepath(path, turnPfs.getCosts(path));
            pAction->setActionID(ACTION_LOAD);
            if (pAction->canBePerformed())
            {
                m_updatePoints.append(pUnit->getPosition());
                m_updatePoints.append(pAction->getActionTarget());
                emit performAction(pAction);
                return true;
            }
        }
        else if (!shortenPathForTarget && CoreAI::contains(targets, targetFields))
        {
            QVector<QPoint> movePath = turnPfs.getClosestReachableMovePath(targetFields);
            pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
            pAction->setActionID(ACTION_WAIT);
            if (pAction->canBePerformed())
            {
                m_updatePoints.append(pUnit->getPosition());
                m_updatePoints.append(pAction->getActionTarget());
                emit performAction(pAction);
                return true;
            }
        }
        else
        {
            QVector<QPoint> movePath = turnPfs.getClosestReachableMovePath(targetFields);
            if (movePath.size() == 0)
            {
                movePath.append(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            }
            qint32 idx = getMoveTargetField(pUnit, pUnits, turnPfs, movePath, pBuildings, pEnemyBuildings);
            if (idx < 0)
            {
                std::tuple<QPoint, float, bool> target = moveToSafety(pUnit, pUnits, turnPfs, movePath[0], pBuildings, pEnemyBuildings);
                QPoint ret = std::get<0>(target);
                float minDamage = std::get<1>(target);
                bool allEqual = std::get<2>(target);
                if (((ret.x() == pUnit->Unit::getX() && ret.y() == pUnit->Unit::getY()) ||
                     minDamage > pUnit->getCoUnitValue() / 2 ||
                     allEqual) && minDamage > 0.0f)
                {
                    if (suicide(pAction, pUnit, turnPfs))
                    {
                        return true;
                    }
                    else
                    {
                        QVector<QPoint> movePath = turnPfs.getPath(ret.x(), ret.y());
                        pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
                    }
                }
                else
                {
                    QVector<QPoint> movePath = turnPfs.getPath(ret.x(), ret.y());
                    pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
                }
            }
            else
            {
                QVector<QPoint> path = turnPfs.getPath(movePath[idx].x(), movePath[idx].y());
                pAction->setMovepath(path, turnPfs.getCosts(path));
            }
            bool lockedUnit = (pAction->getMovePath().size() == 1) &&
                              (pUnit->getHp() < m_lockedUnitHp);
            // when we don't move try to attack if possible
            if ((pUnit->getHp() > m_noMoveAttackHp) ||
                lockedUnit)
            {
                pAction->setActionID(ACTION_FIRE);
                QVector<QVector3D> ret;
                QVector<QVector3D> moveTargetFields;
                getBestAttacksFromField(pUnit, pAction, ret, moveTargetFields);
                if (ret.size() > 0 &&
                    (ret[0].z() >= -pUnit->getCoUnitValue()  * m_minSuicideDamage ||
                     lockedUnit))
                {
                    qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
                    QVector3D target = ret[selection];
                    CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                 static_cast<qint32>(target.y())));
                    if (pAction->isFinalStep() && pAction->canBePerformed())
                    {
                        m_updatePoints.append(pUnit->getPosition());
                        m_updatePoints.append(pAction->getActionTarget());
                        m_updatePoints.append(QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
                        emit performAction(pAction);
                        return true;
                    }
                }
            }
            if (pAction->getMovePath().size() > 0)
            {
                m_updatePoints.append(pUnit->getPosition());
                m_updatePoints.append(pAction->getActionTarget());
                if (actions.contains(ACTION_UNLOAD))
                {
                    pAction->setActionID(ACTION_UNLOAD);
                    if (pAction->canBePerformed())
                    {
                        bool performed = unloadUnits(pAction, pUnit);
                        if (performed)
                        {
                            return true;
                        }
                    }
                }
                for (const auto & action : actions)
                {
                    if (action.startsWith(ACTION_SUPPORTALL))
                    {
                        pAction->setActionID(action);
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
                if (actions.contains(ACTION_STEALTH))
                {
                    pAction->setActionID(ACTION_STEALTH);
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                }
                if (actions.contains(ACTION_UNSTEALTH))
                {
                    pAction->setActionID(ACTION_UNSTEALTH);
                    if (pAction->canBePerformed())
                    {
                        float counterDamage = calculateCounterDamage(pUnit, pUnits, pAction->getActionTarget(), nullptr, 0, pBuildings, pEnemyBuildings, true);
                        if (counterDamage <= 0)
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }                
                for (const auto & action : actions)
                {
                    if (action.startsWith(ACTION_PLACE))
                    {
                        pAction->setActionID(action);
                        if (pAction->canBePerformed())
                        {
                            spMarkedFieldData pData = pAction->getMarkedFieldStepData();
                            QPoint point = pData->getPoints()->at(GlobalUtils::randIntBase(0, pData->getPoints()->size() - 1));
                            CoreAI::addSelectedFieldData(pAction, point);
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
                if (pUnit->canMoveAndFire(pAction->getActionTarget()) ||
                    pUnit->getPosition() == pAction->getActionTarget())
                {
                    pAction->setActionID(ACTION_FIRE);
                    // if we run away and still find a target we should attack it
                    QVector<QVector3D> moveTargets(1, QVector3D(pAction->getActionTarget().x(),
                                                                pAction->getActionTarget().y(), 1));
                    QVector<QVector3D> ret;
                    getBestAttacksFromField(pUnit, pAction, ret, moveTargets);
                    if (ret.size() > 0 && ret[0].z() >= -pUnit->getCoUnitValue()  * m_minSuicideDamage)
                    {
                        qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
                        QVector3D target = ret[selection];
                        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                     static_cast<qint32>(target.y())));
                        if (pAction->isFinalStep() && pAction->canBePerformed())
                        {
                            m_updatePoints.append(pUnit->getPosition());
                            m_updatePoints.append(pAction->getActionTarget());
                            m_updatePoints.append(QPoint(static_cast<qint32>(target.x()),
                                                         static_cast<qint32>(target.y())));
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
                pAction->setActionID(ACTION_WAIT);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
                else
                {
                    oxygine::handleErrorPolicy(oxygine::ep_show_error, "NormalAi::moveUnit invalid action calculated");
                }
            }
        }
    }
    return false;
}

bool NormalAi::suicide(spGameAction & pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs)
{
    CONSOLE_PRINT("NormalAi::suicide", Console::eDEBUG);
    // we don't have a good option do the best that we can attack with an all in attack :D
    pAction->setActionID(ACTION_FIRE);
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &turnPfs, ret, moveTargetFields);
    if (ret.size() > 0 && ret[0].z() >= -pUnit->getCoUnitValue() * m_minSuicideDamage)
    {
        qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
        QVector3D target = ret[selection];
        QVector<QPoint> path = turnPfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                               static_cast<qint32>(moveTargetFields[selection].y()));
        pAction->setMovepath(path, turnPfs.getCosts(path));
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep() && pAction->canBePerformed())
        {
            m_updatePoints.append(pUnit->getPosition());
            m_updatePoints.append(pAction->getActionTarget());
            m_updatePoints.append(QPoint(static_cast<qint32>(target.x()),
                                         static_cast<qint32>(target.y())));
            emit performAction(pAction);
            return true;
        }
    }
    return false;
}

std::tuple<QPoint, float, bool> NormalAi::moveToSafety(Unit* pUnit, spQmlVectorUnit & pUnits,
                                                       UnitPathFindingSystem& turnPfs, QPoint target,
                                                       spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::moveToSafety", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> targets = turnPfs.getAllNodePoints();
    QPoint ret(pUnit->Unit::getX(), pUnit->Unit::getY());
    float leastDamageField = std::numeric_limits<float>::max();
    qint32 shortestDistance = std::numeric_limits<qint32>::max();
    bool allFieldsEqual = true;
    for (qint32 i = 0; i < targets.size(); i++)
    {
        qint32 x = targets[i].x();
        qint32 y = targets[i].y();
        if (pMap->getTerrain(x, y)->getUnit() == nullptr &&
            turnPfs.getCosts(turnPfs.getIndex(x, y), x, y, x, y) > 0)
        {
            float currentDamage = calculateCounterDamage(pUnit, pUnits, targets[i], nullptr, 0.0f, pBuildings, pEnemyBuildings);
            if (leastDamageField < std::numeric_limits<float>::max() &&
                static_cast<qint32>(leastDamageField) != static_cast<qint32>(currentDamage))
            {
                allFieldsEqual = false;
            }
            qint32 distance = GlobalUtils::getDistance(target, targets[i]);
            if (currentDamage < leastDamageField)
            {
                ret = targets[i];
                leastDamageField = currentDamage;
                shortestDistance = distance;
            }
            else if (static_cast<qint32>(currentDamage) == static_cast<qint32>(leastDamageField) && distance < shortestDistance)
            {
                ret = targets[i];
                leastDamageField = currentDamage;
                shortestDistance = distance;
            }
        }
    }
    return std::tuple<QPoint, float, bool>(ret, leastDamageField, allFieldsEqual);
}

qint32 NormalAi::getMoveTargetField(Unit* pUnit, spQmlVectorUnit & pUnits, UnitPathFindingSystem& turnPfs,
                                    QVector<QPoint>& movePath, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < movePath.size(); i++)
    {
        // empty or own field
        qint32 x = movePath[i].x();
        qint32 y = movePath[i].y();
        Terrain* pTerrain = pMap->getTerrain(x, y);
        Building* pBuilding = pTerrain->getBuilding();
        if ((pTerrain->getUnit() == nullptr ||
             pTerrain->getUnit() == pUnit) &&
            turnPfs.getCosts(turnPfs.getIndex(x, y), x, y, x, y) > 0)
        {
            if (isMoveableTile(pBuilding))
            {
                float counterDamage = calculateCounterDamage(pUnit, pUnits, movePath[i], nullptr, 0.0f, pBuildings, pEnemyBuildings);
                if (counterDamage < pUnit->getCoUnitValue() * m_minMovementDamage)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

qint32 NormalAi::getBestAttackTarget(Unit* pUnit, spQmlVectorUnit & pUnits, QVector<CoreAI::DamageData>& ret,
                                     QVector<QVector3D>& moveTargetFields,
                                     spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const
{
    spGameMap pMap = GameMap::getInstance();
    qint32 target = -1;
    qint32 currentDamage = std::numeric_limits<qint32>::min();
    qint32 deffense = 0;

    float minFundsDamage = -pUnit->getCoUnitValue() * m_minAttackFunds;

    for (qint32 i = 0; i < ret.size(); i++)
    {
        QPoint moveTarget(static_cast<qint32>(moveTargetFields[i].x()), static_cast<qint32>(moveTargetFields[i].y()));
        Unit* pEnemy = pMap->getTerrain(static_cast<qint32>(ret[i].x), static_cast<qint32>(ret[i].y))->getUnit();

        qint32 minfireRange = pUnit->getMinRange(moveTarget);
        qint32 fundsDamage = 0;
        float bonusDamage = 0.0f;
        if (pEnemy != nullptr)
        {
            float currentHp = pEnemy->getHp();
            float newHp = currentHp - static_cast<float>(ret[i].hpDamage);
            fundsDamage = static_cast<qint32>(ret[i].fundsDamage * calculateCaptureBonus(pEnemy, newHp));
            if (fundsDamage > minFundsDamage && newHp > 0)
            {
                pEnemy->setVirtualHpValue(newHp);
                fundsDamage += getOwnSupportDamage(pUnit, moveTarget, pEnemy, bonusDamage);
                pEnemy->setVirtualHpValue(0.0f);
            }
            if (minfireRange > 1)
            {
                fundsDamage *= m_ownIndirectAttackValue;
            }
            if (newHp <= 0)
            {
                fundsDamage *= m_enemyKillBonus;
            }
            if (pEnemy->getMinRange(pEnemy->getPosition()) > 1)
            {
                fundsDamage *= m_enemyIndirectBonus;
            }
            if (!isMoveableTile(pMap->getTerrain(moveTarget.x(), moveTarget.y())->getBuilding()))
            {
                fundsDamage -= m_ownProdctionMalus;
            }
        }
        else
        {
            fundsDamage = static_cast<qint32>(ret[i].fundsDamage);
        }
        float counterDamage = calculateCounterDamage(pUnit, pUnits, moveTarget, pEnemy, ret[i].hpDamage + bonusDamage, pBuildings, pEnemyBuildings);
        if (counterDamage < 0)
        {
            counterDamage = 0;
        }
        fundsDamage -= counterDamage;
        Terrain* pTerrain = pMap->getTerrain(static_cast<qint32>(ret[i].x), static_cast<qint32>(ret[i].y));
        qint32 targetDefense = pTerrain->getDefense(pUnit);
        if (fundsDamage >= minFundsDamage)
        {
            if (fundsDamage > currentDamage)
            {
                currentDamage = fundsDamage;
                target = i;
                deffense = targetDefense;
            }
            else if (fundsDamage == currentDamage && targetDefense > deffense)
            {
                currentDamage = fundsDamage;
                target = i;
                deffense = targetDefense;
            }
        }
    }
    return target;
}

float NormalAi::getOwnSupportDamage(Unit* pUnit, QPoint moveTarget, Unit* pEnemy, float & hpDamage) const
{
    spGameMap pMap = GameMap::getInstance();
    float supportDamage = 0;
    hpDamage = 0;
    for (auto & pUnitData : m_OwnUnits)
    {
        if (pUnitData.pUnit != pUnit)
        {
            auto position = pUnitData.pUnit->getPosition();
            qint32 movepoints = pUnitData.pUnit->getMovementpoints(position);
            float minFundsDamage = -pUnitData.pUnit->getCoUnitValue() * m_minAttackFunds;
            if (GlobalUtils::getDistance(moveTarget, position) <= movepoints)
            {
                spGameAction pAction = spGameAction::create(ACTION_FIRE);
                pAction->setTarget(QPoint(pUnitData.pUnit->Unit::getX(), pUnitData.pUnit->Unit::getY()));
                QVector<CoreAI::DamageData> ret;
                QVector<QVector3D> moveTargetFields;
                CoreAI::getAttackTargets(pUnitData.pUnit.get(), pAction, pUnitData.pUnitPfs.get(), ret, moveTargetFields);
                QVector<Unit*> pUsedUnits;
                float newFundsDamage = std::numeric_limits<float>::lowest();
                float newHpDamage = std::numeric_limits<float>::lowest();
                for (qint32 i = 0; i < ret.size(); i++)
                {
                    QPoint newMoveTarget(static_cast<qint32>(moveTargetFields[i].x()), static_cast<qint32>(moveTargetFields[i].y()));
                    Unit* pNewEnemy = pMap->getTerrain(static_cast<qint32>(ret[i].x), static_cast<qint32>(ret[i].y))->getUnit();
                    if (pNewEnemy == pEnemy &&
                        moveTarget != newMoveTarget &&
                        pNewEnemy != nullptr)
                    {
                        float newHp = pEnemy->getHp() - static_cast<float>(ret[i].hpDamage);
                        qint32 fundsDamage = static_cast<qint32>(ret[i].fundsDamage * calculateCaptureBonus(pEnemy, newHp));
                        if (fundsDamage > minFundsDamage && fundsDamage > newFundsDamage)
                        {
                            newFundsDamage = fundsDamage;
                            newHpDamage = ret[i].hpDamage;
                        }
                    }
                    else if (pNewEnemy != nullptr &&
                             pNewEnemy != pEnemy &&
                             !pUsedUnits.contains(pNewEnemy))
                    {
                        if (pNewEnemy->getCoUnitValue() >= m_cheapUnitValue)
                        {
                            pUsedUnits.append(pNewEnemy);
                        }
                    }
                }
                if (newFundsDamage > minFundsDamage)
                {
                    supportDamage += newFundsDamage / static_cast<float>(pUsedUnits.size() + 1) * m_supportDamageBonus;
                    hpDamage += newHpDamage / static_cast<float>(pUsedUnits.size() + 1) * m_supportDamageBonus;
                }
            }
        }
    }
    return supportDamage;
}

float NormalAi::calculateCaptureBonus(Unit* pUnit, float newLife) const
{
    float ret = 1.0f;
    qint32 capturePoints = pUnit->getCapturePoints();
    Building* pBuilding = pUnit->getTerrain()->getBuilding();
    if (capturePoints > 0)
    {
        qint32 restCapture = 20 - capturePoints;
        qint32 currentHp = pUnit->getHpRounded();
        qint32 newHp = GlobalUtils::roundUp(newLife);
        qint32 remainingDays = GlobalUtils::roundUp(restCapture / static_cast<float>(currentHp));
        if (newHp <= 0)
        {
            if (remainingDays > 0)
            {
                ret = 1 + (m_antiCaptureBonus - currentHp) / currentHp;
            }
            else
            {
                ret = m_antiCaptureBonus + 1.0f;
            }
        }
        else
        {
            qint32 newRemainingDays = GlobalUtils::roundUp(restCapture / static_cast<float>(newHp));
            if (remainingDays > newRemainingDays)
            {
                ret = 0.8f;
            }
            else if (remainingDays == newRemainingDays && remainingDays < 2)
            {
                ret = 1.0f;
            }
            else if (remainingDays == 0)
            {
                ret = 1.0f;
            }
            else
            {
                ret = 1 + (newRemainingDays - remainingDays) / remainingDays;
            }
            if (ret > m_antiCaptureBonusScoreReduction)
            {
                ret = ret / m_antiCaptureBonusScoreDivider + m_antiCaptureBonusScoreReduction / m_antiCaptureBonusScoreDivider;
            }
        }
    }
    if (pBuilding != nullptr &&
        pBuilding->getOwner() == m_pPlayer &&
        pBuilding->getBuildingID() == "HQ" &&
        pUnit->getActionList().contains(ACTION_CAPTURE))
    {
        ret *= m_antiCaptureHqBonus;
    }
    return ret;
}

float NormalAi::calculateCounterDamage(Unit* pUnit, spQmlVectorUnit & pUnits, QPoint newPosition,
                                       Unit* pEnemyUnit, float enemyTakenDamage,
                                       spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                       bool ignoreOutOfVisionRange) const
{
    qint32 baseCost = pUnit->getCoUnitValue();
    QVector<qint32> baseCosts;
    for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
    {
        baseCosts.append(pUnits->at(i3)->getCoUnitValue());
    }
    spGameMap pMap = GameMap::getInstance();
    float counterDamage = 0;
    for (qint32 i = 0; i < m_EnemyUnits.size(); i++)
    {
        spUnit pNextEnemy = m_EnemyUnits[i].pUnit;
        if (pNextEnemy->getHp() > 0 && pNextEnemy->getTerrain() != nullptr)
        {
            QPoint enemyPos = QPoint(pNextEnemy->Unit::getX(), pNextEnemy->Unit::getY());
            qint32 minFireRange = pNextEnemy->getMinRange(enemyPos);
            qint32 maxFireRange = pNextEnemy->getMaxRange(enemyPos);
            qint32 moveRange = 0;
            qint32 distance = GlobalUtils::getDistance(newPosition, enemyPos);
            bool canMoveAndFire = pNextEnemy->canMoveAndFire(enemyPos);
            if (canMoveAndFire)
            {
                moveRange = pNextEnemy->getMovementpoints(enemyPos);
            }
            if (distance <= moveRange + maxFireRange &&
                pNextEnemy->isAttackable(pUnit, true))
            {
                float enemyDamage = static_cast<float>(m_VirtualEnemyData[i].x());
                if (pNextEnemy.get() == pEnemyUnit)
                {
                    enemyDamage += enemyTakenDamage;
                }
                enemyDamage *= m_enemyCounterDamageMultiplier;
                if (enemyDamage < pNextEnemy->getHp() * Unit::MAX_UNIT_HP)
                {
                    QRectF damageData;
                    QVector<QPoint> targets = m_EnemyUnits[i].pUnitPfs->getAllNodePoints();
                    if (distance >= minFireRange && distance <= maxFireRange)
                    {
                        // indirect attack
                        damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, enemyPos, pUnit, 0, newPosition, ignoreOutOfVisionRange);
                        for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
                        {
                            distance = GlobalUtils::getDistance(QPoint(pUnits->at(i3)->Unit::getX(), pUnits->at(i3)->Unit::getY()), enemyPos);
                            if (distance >= minFireRange && distance <= maxFireRange &&
                                pNextEnemy->isAttackable(pUnits->at(i3), true))
                            {
                                if (baseCosts[i3] > 0 && baseCost > 0)
                                {
                                    if (baseCost > baseCosts[i3])
                                    {
                                        // reduce damage the more units it can attack
                                        damageData.moveLeft(damageData.x() * baseCosts[i3] / baseCost / 2);
                                    }
                                    else
                                    {
                                        damageData.moveLeft( damageData.x() *  baseCost / baseCosts[i3] / 2);
                                    }
                                }
                            }
                        }
                    }
                    else if (canMoveAndFire)
                    {
                        bool found = false;
                        for (qint32 i2 = 0; i2 < targets.size(); i2++)
                        {
                            distance = GlobalUtils::getDistance(newPosition, targets[i2]);
                            Unit* pTerrainUnit = pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit();
                            if (distance >= minFireRange && distance <= maxFireRange &&
                                (pTerrainUnit == nullptr ||
                                 pTerrainUnit == pNextEnemy ||
                                 pTerrainUnit == pUnit))
                            {
                                damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, targets[i2], pUnit, 0, newPosition, ignoreOutOfVisionRange);
                                found = true;
                                break;
                            }
                        }
                        if (found)
                        {
                            QVector<Unit*> usedUnits;
                            for (qint32 i2 = 0; i2 < targets.size(); i2++)
                            {
                                for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
                                {
                                    Unit* pUnit = pUnits->at(i3);
                                    if (!usedUnits.contains(pUnit))
                                    {
                                        distance = GlobalUtils::getDistance(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()), targets[i2]);
                                        if (distance >= minFireRange && distance <= maxFireRange &&
                                            (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr ||
                                             pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit()->getOwner()->isAlly(m_pPlayer)) &&
                                            pNextEnemy->isAttackable(pUnit, true))
                                        {
                                            usedUnits.append(pUnit);
                                            if (baseCosts[i3] > 0 && baseCost > 0)
                                            {
                                                if (baseCost > baseCosts[i3])
                                                {
                                                    // reduce damage the more units it can attack
                                                    damageData.moveLeft(damageData.x() * baseCosts[i3] / baseCost / 2);
                                                }
                                                else
                                                {
                                                    damageData.moveLeft(damageData.x() *  baseCost / baseCosts[i3] / 2);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            if (damageData.x() < 0)
                            {
                                damageData.moveLeft(damageData.x());
                            }
                            if (damageData.x() > 0)
                            {
                                counterDamage += static_cast<qint32>(calcFundsDamage(damageData, pNextEnemy.get(), pUnit).y());
                            }
                        }
                    }
                }
            }
        }
    }
    counterDamage += calculateCounteBuildingDamage(pUnit, newPosition, pBuildings, pEnemyBuildings);
    static qint32 min = std::numeric_limits<qint32>::max();
    static qint32 max = std::numeric_limits<qint32>::min();
    if (counterDamage < min)
    {
        min = counterDamage;
    }
    if (counterDamage > max)
    {
        max = counterDamage;
    }
    return counterDamage;
}

float NormalAi::calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const
{
    float counterDamage = 0.0f;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        counterDamage += calcBuildingDamage(pUnit, newPosition, pBuilding);
    }
    for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
    {
        Building* pBuilding = pEnemyBuildings->at(i);
        counterDamage += calcBuildingDamage(pUnit, newPosition, pBuilding);
    }
    spQmlVectorPoint pCircle = spQmlVectorPoint(GlobalUtils::getCircle(1, 2));
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pCircle->size(); i++)
    {
        QPoint pos = newPosition + pCircle->at(i);
        if (pMap->onMap(pos.x(), pos.y()))
        {
            Unit* pMine = pMap->getTerrain(pos.x(), pos.y())->getUnit();
            if (pMine != nullptr &&
                !pMine->isStealthed(m_pPlayer) &&
                pMine->getUnitID() == "WATERMINE")
            {
                counterDamage +=  m_watermineDamage;
            }
        }
    }
    return counterDamage;
}

void NormalAi::updateAllUnitData(spQmlVectorUnit & pUnits)
{
    bool initial = m_EnemyUnits.size() == 0;
    rebuildIsland(pUnits);


    spQmlVectorUnit units = spQmlVectorUnit(m_pPlayer->getEnemyUnits());

    updateUnitData(pUnits, m_OwnUnits, false);
    updateUnitData(units, m_EnemyUnits, true);
    if (initial)
    {

        calcVirtualDamage(pUnits);
    }
    m_updatePoints.clear();
}

void NormalAi::updateUnitData(spQmlVectorUnit & pUnits, QVector<MoveUnitData> & pUnitData, bool enemy)
{    
    CONSOLE_PRINT("NormalAi::updateEnemyData", Console::eDEBUG);
    if (pUnitData.size() == 0)
    {
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            auto pUnit = pUnits->at(i);
            if (!pUnit->getHasMoved())
            {
                MoveUnitData data;
                data.pUnitPfs = spUnitPathFindingSystem::create(pUnit);
                if (enemy)
                {
                    data.pUnitPfs->setIgnoreEnemies(true);
                }
                data.pUnitPfs->explore();
                data.pUnit = pUnit;
                if (enemy)
                {
                    m_VirtualEnemyData.append(QPointF(0, 0));
                }
                pUnitData.append(data);
            }
        }
    }
    else
    {
        qint32 i = 0;
        while (i < pUnitData.size())
        {
            if (pUnitData[i].pUnit->getHp() <= 0 ||
                pUnitData[i].pUnit->getTerrain() == nullptr ||
                pUnitData[i].pUnit->getHasMoved())
            {
                pUnitData.removeAt(i);
                if (enemy)
                {
                    m_VirtualEnemyData.removeAt(i);
                }
            }
            else
            {
                i++;
            }
        }
    }
    QVector<qint32> updated;
    for (qint32 i = 0; i < m_updatePoints.size(); i++)
    {
        for (qint32 i2 = 0; i2 < pUnitData.size(); i2++)
        {
            if (!updated.contains(i2))
            {
                if (pUnitData[i2].pUnit->getHp() > 0 && pUnitData[i2].pUnit->getTerrain() != nullptr)
                {
                    if (qAbs(m_updatePoints[i].x() - pUnitData[i2].pUnit->Unit::getX()) +
                        qAbs(m_updatePoints[i].y() - pUnitData[i2].pUnit->Unit::getY()) <=
                        pUnitData[i2].pUnit->getMovementpoints(QPoint(pUnitData[i2].pUnit->Unit::getX(), pUnitData[i2].pUnit->Unit::getY())) + 2)
                    {
                        pUnitData[i2].pUnitPfs = spUnitPathFindingSystem::create(pUnitData[i2].pUnit.get());
                        if (enemy)
                        {
                            pUnitData[i2].pUnitPfs->setIgnoreEnemies(true);
                        }
                        pUnitData[i2].pUnitPfs->explore();
                    }
                    updated.push_back(i2);
                }
            }
        }
    }
}

void NormalAi::calcVirtualDamage(spQmlVectorUnit & pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        QVector<QPoint> attackedUnits;
        QVector<float> damage;
        if (isUsingUnit(pUnit))
        {
            spGameAction action = spGameAction::create(ACTION_FIRE);
            action->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            UnitPathFindingSystem pfs(pUnit);
            pfs.explore();
            QVector<CoreAI::DamageData> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, action, &pfs, ret, moveTargetFields);
            for (qint32 i2 = 0; i2 < ret.size(); i2++)
            {
                QPoint pos(static_cast<qint32>(ret[i2].x), static_cast<qint32>(ret[i2].y));
                if (!attackedUnits.contains(pos))
                {
                    attackedUnits.append(pos);
                    damage.append(ret[i2].hpDamage);
                }
            }
        }
        for (qint32 i2 = 0; i2 < attackedUnits.size(); i2++)
        {
            for (qint32 i3 = 0; i3 < m_EnemyUnits.size(); i3++)
            {
                if (m_EnemyUnits[i3].pUnit->Unit::getX() == attackedUnits[i2].x() &&
                    m_EnemyUnits[i3].pUnit->Unit::getY() == attackedUnits[i2].y())
                {
                    m_VirtualEnemyData[i3].setX(m_VirtualEnemyData[i3].x() + m_enemyUnitCountDamageReductionMultiplier * static_cast<double>(damage[i2]) / (damage.size()));
                    break;
                }
            }
        }
    }
}

void NormalAi::clearEnemyData()
{
    m_VirtualEnemyData.clear();
    m_EnemyUnits.clear();
}

bool NormalAi::buildUnits(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                          spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("NormalAi::buildUnits()", Console::eDEBUG);
    if (m_aiStep < AISteps::buildUnits)
    {
        m_productionData.clear();
    }
    m_aiStep = AISteps::buildUnits;
    spGameMap pMap = GameMap::getInstance();

    qint32 enemeyCount = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (m_pPlayer->isEnemy(pMap->getPlayer(i)) && !pMap->getPlayer(i)->getIsDefeated())
        {
            enemeyCount++;
        }
    }
    float funds = m_pPlayer->getFunds();
    QVector<float> data(BuildItems::Max, 0);
    float productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pBuilding->getTerrain()->getUnit() == nullptr)
        {
            auto buildList = pBuilding->getConstructionList();
            for (auto & unitId : buildList)
            {
                Unit dummy(unitId, m_pPlayer, false);
                if (m_pPlayer->getCosts(unitId, pBuilding->getPosition()) < funds && dummy.hasWeapons())
                {
                    productionBuildings++;
                    break;
                }
            }
        }
    }
    if (productionBuildings <= 0)
    {
        return false;
    }
    UnitCountData countData;
    GetOwnUnitCounts(pUnits, pEnemyUnits, pEnemyBuildings, countData);
    QVector<QVector4D> attackCount(pEnemyUnits->size(), QVector4D(0, 0, 0, 0));
    getEnemyDamageCounts(pUnits, pEnemyUnits, attackCount);
    data[InfantryUnitRatio] = static_cast<float>(countData.infantryUnits) / static_cast<float>(pUnits->size() + 1);
    data[SupplyRatio] = static_cast<float>(countData.supplyUnits) / static_cast<float>(pUnits->size() + 1);
    data[RequiredSupplyRatio] = static_cast<float>(countData.supplyNeededUnits) / static_cast<float>(pUnits->size() + 1);
    data[InfantryCount] = countData.infantryUnits;
    m_currentDirectIndirectRatio = m_directIndirectRatio * getAiCoBuildRatioModifier();
    // calc average costs if we would build same cost units on every building
    float fundsPerFactory = funds - m_cappedFunds * (productionBuildings - 1) * m_fundsPerBuildingFactorD;
    if (fundsPerFactory <= m_cappingFunds)
    {
        data[LowFunds] = 1.0;
        if (fundsPerFactory > m_cappedFunds * m_fundsPerBuildingFactorB)
        {
            fundsPerFactory = m_cappedFunds * m_fundsPerBuildingFactorB;
        }
        else
        {
            fundsPerFactory = m_cappedFunds;
        }
    }
    else if (fundsPerFactory < m_spamingFunds * m_fundsPerBuildingFactorA)
    {
        fundsPerFactory = m_spamingFunds;
    }
    else if (fundsPerFactory >= m_spamingFunds * m_fundsPerBuildingFactorC)
    {
        CONSOLE_PRINT("NormalAI: Building very expensive units", Console::eDEBUG);
        fundsPerFactory = m_spamingFunds * m_fundsPerBuildingFactorC;
        data[UseHighTechUnits] = 1.0f;
    }
    else if (fundsPerFactory >= m_spamingFunds * m_fundsPerBuildingFactorA)
    {
        CONSOLE_PRINT("NormalAI: Building expensive units", Console::eDEBUG);
        fundsPerFactory = m_spamingFunds * m_fundsPerBuildingFactorA;
        data[UseHighTechUnits] = 1.0f;
    }
    CONSOLE_PRINT("NormalAI: fundsPerFactory=" + QString::number(fundsPerFactory), Console::eDEBUG);
    data[DirectUnitRatio] = static_cast<float>(countData.directUnits) / static_cast<float>(countData.indirectUnits + 1);

    data[UnitEnemyRatio] = (static_cast<float>(pUnits->size()) + m_ownUnitEnemyUnitRatioAverager) / (static_cast<float>(pEnemyUnits->size()) + m_ownUnitEnemyUnitRatioAverager);
    if (enemeyCount > 1)
    {
        data[UnitEnemyRatio] *= static_cast<float>(enemeyCount - 1);
    }
    data[UnitCount] = pUnits->size();
    data[EnemyUnitCount] = pEnemyUnits->size();
    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS);
    float bestScore = NO_BUILD_SCORE + 1;
    QVector<qint32> buildingIdx;
    QVector<qint32> unitIDx;
    QVector<float> scores;
    QVector<bool> transporters;
    float variance = pMap->getCurrentDay() - 1 + m_startDayScoreVariancer;
    if (variance > m_maxDayScoreVariancer)
    {
        variance = m_maxDayScoreVariancer;
    }
    spQmlVectorPoint pFields = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pBuilding->getTerrain()->getUnit() == nullptr)
        {
            pAction->setTarget(QPoint(pBuilding->Building::getX(), pBuilding->Building::getY()));
            if (pAction->canBePerformed())
            {
                // we're allowed to build units here
                spMenuData pData = pAction->getMenuStepData();
                if (pData->validData())
                {
                    auto enableList = pData->getEnabledList();
                    auto actionIds = pData->getActionIDs();
                    qint32 index = getIndexInProductionData(pBuilding);
                    auto & buildingData = m_productionData[index];
                    for (qint32 i2 = 0; i2 < pData->getActionIDs().size(); i2++)
                    {
                        if (enableList[i2])
                        {
                            float score = 0.0f;
                            qint32 unitIdx = getUnitProductionIdx(index, actionIds[i2],
                                                                  pUnits, countData.transportTargets,
                                                                  pEnemyUnits, pEnemyBuildings,
                                                                  attackCount, data);
                            bool isTransporter = pBuilding->getBuildingID() == "HARBOUR";
                            if (unitIdx >= 0)
                            {
                                auto & unitData = buildingData.m_buildData[unitIdx];
                                data[UnitCost] = unitData.cost;
                                if (unitData.canMove)
                                {
                                    data[Movementpoints] = unitData.movePoints;
                                    data[ReachDistance] = unitData.closestTarget;
                                    data[COBonus] = unitData.coBonus;
                                    data[NotAttackableCount] = unitData.notAttackableCount;
                                    data[DamageData] = unitData.damage;
                                    data[AttackCount] = unitData.attackCount;
                                    data[FundsFactoryRatio] = unitData.cost / fundsPerFactory;
                                    data[SameFundsMatchUpScore] = unitData.sameFundsMatchUpScore;
                                    data[CounterDamage] = unitData.counterDamage;
                                    if (pEnemyBuildings->size() > 0 && enemeyCount > 0)
                                    {
                                        data[BuildingEnemyRatio] = pBuildings->size() / (static_cast<float>(pEnemyBuildings->size()) / static_cast<float>(enemeyCount));
                                    }
                                    else
                                    {
                                        data[BuildingEnemyRatio] = 0.0;
                                    }
                                    if (unitData.infantryUnit)
                                    {
                                        data[InfantryUnit] = 1.0;
                                    }
                                    else
                                    {
                                        data[InfantryUnit] = 0.0;
                                    }
                                    if (unitData.indirectUnit)
                                    {
                                        data[IndirectUnit] = 1.0;
                                        data[DirectUnit] = 0.0;
                                    }
                                    else
                                    {
                                        data[IndirectUnit] = 0.0;
                                        data[DirectUnit] = 1.0;
                                    }
                                    if (!unitData.isTransporter)
                                    {
                                        score = calcBuildScore(data, unitData);
                                    }
                                    else
                                    {
                                        score = calcTransporterScore(unitData, pUnits, data);
                                        isTransporter = true;
                                    }
                                    score *= BaseGameInputIF::getUnitBuildValue(actionIds[i2]);
                                }
                                else
                                {
                                    score = NO_BUILD_SCORE;
                                }
                            }
                            if (score > bestScore)
                            {
                                bestScore = score;
                                buildingIdx.append(i);
                                unitIDx.append(i2);
                                scores.append(score);
                                transporters.append(isTransporter);
                                qint32 index = 0;
                                while (index < scores.size())
                                {
                                    if (scores[index] < bestScore - variance)
                                    {
                                        buildingIdx.removeAt(index);
                                        unitIDx.removeAt(index);
                                        scores.removeAt(index);
                                        transporters.removeAt(index);
                                    }
                                    else
                                    {
                                        index++;
                                    }
                                }
                            }
                            else if (score >= bestScore - variance)
                            {
                                buildingIdx.append(i);
                                unitIDx.append(i2);
                                scores.append(score);
                                transporters.append(isTransporter);
                            }
                        }
                    }
                }
            }
        }
    }

    if (buildingIdx.size() > 0)
    {
        qint32 item = GlobalUtils::randIntBase(0, buildingIdx.size() - 1);
        Building* pBuilding = pBuildings->at(buildingIdx[item]);
        pAction->setTarget(QPoint(pBuilding->Building::getX(), pBuilding->Building::getY()));
        spMenuData pData = pAction->getMenuStepData();
        QString unitId = pData->getActionIDs()[unitIDx[item]];
        CONSOLE_PRINT("NormalAi::buildUnits() producing " + unitId + " with score " + QString::number(scores[item]), Console::eDEBUG);
        if (transporters[item])
        {
            for (auto & building : m_productionData)
            {
                for (auto & unit : building.m_buildData)
                {
                    if (unit.isTransporter)
                    {
                        unit.noTransporterBonus = 0;
                        unit.transportCount += 1;
                    }
                }
            }
        }
        if (pData->validData())
        {
            CoreAI::addMenuItemData(pAction, unitId, pData->getCostList()[unitIDx[item]]);
        }
        // produce the unit
        if (pAction->isFinalStep())
        {
            m_updatePoints.append(pAction->getActionTarget());
            emit performAction(pAction);
            return true;
        }
    }
    return false;
}

qint32 NormalAi::getIndexInProductionData(Building* pBuilding)
{
    CONSOLE_PRINT("NormalAi::getIndexInProductionData()", Console::eDEBUG);
    qint32 x = pBuilding->Building::getX();
    qint32 y = pBuilding->Building::getY();
    qint32 ret = -1;
    for (qint32 i = 0; i < m_productionData.length(); ++i)
    {
        if (m_productionData[i].m_x == x &&
            m_productionData[i].m_y == y)
        {
            ret = i;
            break;
        }
    }
    if (ret < 0)
    {
        m_productionData.append(ProductionData());
        ret = m_productionData.length() - 1;
        m_productionData[ret].m_x = x;
        m_productionData[ret].m_y = y;
        m_productionData[ret].buildList = pBuilding->getConstructionList();
    }
    return ret;
}

qint32 NormalAi::getUnitProductionIdx(qint32 index, const QString & unitId,
                                      spQmlVectorUnit & pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                      spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                      QVector<QVector4D> & attackCount, QVector<float> & buildData)
{
    CONSOLE_PRINT("NormalAi::getUnitProductionIdx()", Console::eDEBUG);
    qint32 ret = -1;
    auto & data = m_productionData[index];
    for (qint32 i = 0; i < data.m_buildData.length(); ++i)
    {
        auto & unitData = data.m_buildData[i];
        if (unitData.unitId == unitId)
        {
            if (unitData.notAttackableCount > 0)
            {
                Unit dummy(unitId, m_pPlayer, false);
                dummy.setVirtuellX(data.m_x);
                dummy.setVirtuellY(data.m_y);
                float bonusFactor = 1.0f;
                if ((buildData[DirectUnitRatio] > m_directIndirectRatio && unitData.baseRange > 1) ||
                    (buildData[DirectUnitRatio] < m_directIndirectRatio && unitData.baseRange == 1))
                {
                    bonusFactor = m_directIndirectUnitBonusFactor;
                }
                auto damageData = calcExpectedFundsDamage(data.m_x, data.m_y, dummy, pEnemyUnits, attackCount, bonusFactor, unitData.movePoints);
                unitData.notAttackableCount = damageData.notAttackableCount;
                unitData.damage =  damageData.damage;
                unitData.attackCount = damageData.attackCount;
                unitData.counterDamage = damageData.counterDamage;
            }
            ret = i;
            break;
        }
    }
    if (ret < 0)
    {
        data.m_buildData.append(UnitBuildData());
        ret = data.m_buildData.length() - 1;
        auto & unitBuildData = data.m_buildData[ret];
        unitBuildData.unitId = unitId;
        createUnitBuildData(data.m_x, data.m_y, unitBuildData,
                            pUnits, transportTargets,
                            pEnemyUnits, pEnemyBuildings,
                            attackCount, buildData, data.buildList);
    }
    return ret;
}

void NormalAi::createUnitBuildData(qint32 x, qint32 y, UnitBuildData & unitBuildData,
                                   spQmlVectorUnit & pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                   spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                   QVector<QVector4D> & attackCount, QVector<float> & buildData, const QStringList & buildList)
{
    CONSOLE_PRINT("NormalAi::createUnitBuildData()", Console::eDEBUG);
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    Unit dummy(unitBuildData.unitId, m_pPlayer, false);
    dummy.setVirtuellX(x);
    dummy.setVirtuellY(y);
    spTerrain pDummyTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
    qint32 baseMovementCost = pMovementTableManager->getBaseMovementPoints(dummy.getMovementType(), pDummyTerrain.get(), pDummyTerrain.get(), &dummy);
    if (baseMovementCost < 0)
    {
        baseMovementCost = 1;
    }
    createIslandMap(dummy.getMovementType(), dummy.getUnitID());
    UnitPathFindingSystem pfs(&dummy, m_pPlayer);
    pfs.explore();
    unitBuildData.movePoints = dummy.getMovementpoints(QPoint(x, y)) / baseMovementCost;
    auto points = pfs.getAllNodePoints();
    if (points.length() >= unitBuildData.movePoints * 1.5f)
    {
        QStringList actionList = dummy.getActionList();
        unitBuildData.canMove = true;
        unitBuildData.isTransporter = (dummy.getWeapon1ID().isEmpty() &&
                                       dummy.getWeapon2ID().isEmpty());
        unitBuildData.isSupplyUnit = isRefuelUnit(actionList);
        if (unitBuildData.isTransporter)
        {
            getTransporterData(unitBuildData, dummy, pUnits, pEnemyUnits, pEnemyBuildings, transportTargets);
        }
        else
        {
            unitBuildData.closestTarget = getClosestTargetDistance(x, y, dummy, pEnemyUnits, pEnemyBuildings);
            unitBuildData.coBonus += getAiCoUnitMultiplier(m_pPlayer->getCO(0), &dummy);
            unitBuildData.coBonus += getAiCoUnitMultiplier(m_pPlayer->getCO(1), &dummy);
            unitBuildData.baseRange = dummy.getBaseMaxRange();
            float bonusFactor = 1.0f;
            if ((buildData[DirectUnitRatio] > m_directIndirectRatio && unitBuildData.baseRange > 1) ||
                (buildData[DirectUnitRatio] < m_directIndirectRatio && unitBuildData.baseRange == 1))
            {
                bonusFactor = m_directIndirectUnitBonusFactor;
            }
            auto damageData = calcExpectedFundsDamage(x, y, dummy, pEnemyUnits, attackCount, bonusFactor, unitBuildData.movePoints);
            unitBuildData.notAttackableCount = damageData.notAttackableCount;
            unitBuildData.damage = damageData.damage;
            unitBuildData.attackCount = damageData.attackCount;
            unitBuildData.counterDamage = damageData.counterDamage;
            unitBuildData.cost = dummy.getUnitCosts();
            unitBuildData.infantryUnit = (actionList.contains(ACTION_CAPTURE) &&
                                          dummy.getLoadingPlace() == 0);
            unitBuildData.indirectUnit = dummy.getBaseMaxRange() > 1;
            unitBuildData.sameFundsMatchUpScore = calcSameFundsMatchUpScore(dummy, buildList);
        }
    }
}

void NormalAi::getEnemyDamageCounts(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, QVector<QVector4D> & attackCount)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            float dmg1 = 0.0f;
            float hpValue = pUnit->getHpRounded() / Unit::MAX_UNIT_HP;
            Unit* pEnemyUnit = pEnemyUnits->at(i2);
            // get weapon 1 damage
            if (!pUnit->getWeapon1ID().isEmpty())
            {
                dmg1 = pWeaponManager->getBaseDamage(pUnit->getWeapon1ID(), pEnemyUnit) * hpValue;
            }
            // get weapon 2 damage
            float dmg2 = 0.0f;
            if (!pUnit->getWeapon2ID().isEmpty())
            {
                dmg2 = pWeaponManager->getBaseDamage(pUnit->getWeapon2ID(), pEnemyUnit) * hpValue;
            }

            if ((dmg1 > m_notAttackableDamage || dmg2 > m_notAttackableDamage))
            {
                if (onSameIsland(pUnit, pEnemyUnits->at(i2)))
                {
                    attackCount[i2].setY(attackCount[i2].y() + 1);
                }
                attackCount[i2].setX(attackCount[i2].x() + 1);
            }
            if (dmg1 > m_midDamage || dmg2 > m_midDamage)
            {
                attackCount[i2].setZ(attackCount[i2].z() + 1);
            }
            if (dmg1 > m_highDamage || dmg2 > m_highDamage)
            {
                attackCount[i2].setW(attackCount[i2].w() + 1);
            }
        }
    }
}

qint32 NormalAi::getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    qint32 minDistance = std::numeric_limits<qint32>::max();
    QPoint pos(posX, posY);
    qint32 islandIdx = CoreAI::getIslandIndex(&dummy);
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemyUnit = pEnemyUnits->at(i);
        if (onSameIsland(islandIdx, posX, posY, pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY()))
        {
            if (dummy.isAttackable(pEnemyUnit, true))
            {
                qint32 distance = GlobalUtils::getDistance(pos, pEnemyUnit->getPosition());
                if (minDistance > distance)
                {
                    minDistance = distance;
                }
            }
        }
    }
    if (dummy.getActionList().contains(ACTION_CAPTURE))
    {
        bool missileTarget = hasMissileTarget();
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pBuilding = pEnemyBuildings->at(i);
            if (dummy.canMoveOver(pBuilding->Building::getX(), pBuilding->Building::getY()))
            {
                if (pBuilding->isCaptureOrMissileBuilding(missileTarget) &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    qint32 distance = GlobalUtils::getDistance(pos, pBuilding->getPosition());
                    if (minDistance > distance)
                    {
                        minDistance = distance;
                    }
                }
            }
        }
    }
    return minDistance;
}

NormalAi::ExpectedFundsData NormalAi::calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, const QVector<QVector4D> & attackCount, float bonusFactor, float myMovepoints)
{
    ExpectedFundsData ret;
    float notAttackableCount = 0;
    float damageCount = 0;
    float attacksCount = 0;
    float extraMalusCount = 0;

    float counterDamageCount = 0;
    float counterAttacksCount = 0;

    if (myMovepoints == 0)
    {
        myMovepoints = 1;
    }
    spGameMap pMap = GameMap::getInstance();
    float myFirerange = dummy.getBaseMaxRange();
    float ownRange = myMovepoints;
    if (dummy.canMoveAndFire(QPoint(posX, posY)))
    {
        ownRange += myFirerange;
    }
    else
    {
        ownRange = (myMovepoints + myFirerange) * 0.5f;
    }

    QPoint position = dummy.getPosition();
    auto ownValue = dummy.getCoUnitValue();
    for (qint32 i3 = 0; i3 < pEnemyUnits->size(); i3++)
    {
        Unit* pEnemyUnit = pEnemyUnits->at(i3);
        QPoint enemyPosition = pEnemyUnit->getPosition();
        float enemyFirerange = pEnemyUnit->getBaseMaxRange();
        float enemyMovepoints = pEnemyUnit->getBaseMovementPoints();
        float enemyRange = enemyMovepoints;
        if (pEnemyUnit->canMoveAndFire(enemyPosition))
        {
            enemyRange += enemyFirerange;
        }
        else
        {
            enemyRange = (enemyMovepoints + enemyFirerange) * 0.5f;
        }

        float distance = GlobalUtils::getDistance(position, enemyPosition);
        float dmg = dummy.getBaseDamage(pEnemyUnit);
        if (dmg > pEnemyUnit->getHp() * Unit::MAX_UNIT_HP)
        {
            dmg = pEnemyUnit->getHp() * Unit::MAX_UNIT_HP;
        }
        if (dmg > 0.0f)
        {
            bool firstStrikes = ownRange >= enemyRange;
            float resDamage = 0;

            auto enemyValue = pEnemyUnit->getCoUnitValue();
            resDamage = (dmg / (pEnemyUnit->getHp() * Unit::MAX_UNIT_HP)) * enemyValue;

            if (resDamage > enemyValue && resDamage > 0)
            {
                resDamage = enemyValue * (m_maxOverkillBonus - enemyValue / resDamage);
            }
            float mult = (ownRange + m_smoothingValue) / (enemyRange + m_smoothingValue);
            if (mult > m_maxDistanceMultiplier)
            {
                mult = m_maxDistanceMultiplier;
            }
            resDamage *= mult;            
            if (firstStrikes)
            {
                resDamage *= bonusFactor;
            }
            float factor = 0.0f;
            if (dmg > m_highDamage)
            {
                factor += (attackCount[i3].w() + m_smoothingValue) / (attackCount[i3].x() + m_smoothingValue);
            }
            else if (dmg > m_midDamage)
            {
                factor += (attackCount[i3].z() + m_smoothingValue) / (attackCount[i3].x() + m_smoothingValue);
            }
            else if (dmg < m_notAttackableDamage)
            {
                ++extraMalusCount;
            }
            if (onSameIsland(dummy.getMovementType(), posX, posY, pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY()))
            {
                float bonus = (m_sameIslandBonusInRangeDays - (distance / myMovepoints) * m_sameIslandOutOfDayMalusFactor);
                if (bonus > m_maxCloseDistanceDamageBonus)
                {
                    bonus = m_maxCloseDistanceDamageBonus;
                }
                else if (bonus > m_minCloseDistanceDamageBonus)
                {
                    bonus = m_minCloseDistanceDamageBonus;
                }
                factor += bonus;
            }
            else
            {
                float bonus =  (m_differentIslandBonusInRangeDays - (distance / myMovepoints) * m_differentIslandOutOfDayMalusFactor);
                if (bonus > m_maxCloseDistanceDamageBonus)
                {
                    bonus = m_maxCloseDistanceDamageBonus;
                }
                else if (bonus > m_minCloseDistanceDamageBonus)
                {
                    bonus = m_minCloseDistanceDamageBonus;
                }
                factor += bonus;
            }
            if (pEnemyUnit->hasWeapons())
            {
                float notAttackableValue = 0.0f;
                if (dmg > m_highDamage)
                {
                    notAttackableValue = m_highDamageBonus;
                }
                else if (dmg > m_midDamage)
                {
                    notAttackableValue = m_midDamageBonus;
                }
                else if (dmg > m_notAttackableDamage)
                {
                    notAttackableValue = m_lowDamageBonus;
                }
                else
                {
                    factor *= m_veryLowDamageBonus;
                }
                if (attackCount[i3].y() == 0.0f &&
                    attackCount[i3].x() == 0.0f &&
                    attackCount[i3].z() == 0.0f &&
                    attackCount[i3].w() == 0.0f)
                {
                    notAttackableCount += notAttackableValue * m_currentlyNotAttackableBonus;
                }
                else if (attackCount[i3].x() == 0.0f &&
                         attackCount[i3].z() == 0.0f &&
                         attackCount[i3].w() == 0.0f)
                {
                    notAttackableCount  += notAttackableValue * m_currentlyNotAttackableBonus * m_currentlyNotAttackableBonus;
                }
            }
            else
            {
                factor += m_transportBonus;
            }
            if (factor < 0)
            {
                factor = 0;
            }

            damageCount += resDamage * factor;
            attacksCount += factor;
        }

        if (pEnemyUnit->hasWeapons())
        {
            float counterDmg = pEnemyUnit->getBaseDamage(&dummy) * pEnemyUnit->getHp() / Unit::MAX_UNIT_HP;
            if (counterDmg >= 0.0f)
            {
                bool firstStrikes = enemyRange >= ownRange;
                float resDamage = 0;
                resDamage = (counterDmg / (Unit::MAX_UNIT_HP * Unit::MAX_UNIT_HP)) * ownValue;
                if (firstStrikes)
                {
                    resDamage *= bonusFactor;
                }
                float mult = (enemyRange + m_smoothingValue) / (ownRange + m_smoothingValue);
                if (mult > m_maxDistanceMultiplier)
                {
                    mult = m_maxDistanceMultiplier;
                }
                resDamage *= mult;
                if (resDamage > ownValue)
                {
                    resDamage = ownValue;
                }
                counterDamageCount += resDamage;
            }
            ++counterAttacksCount;
        }
    }
    float damage = 0.0f;
    if (attacksCount > 0)
    {
        damage = damageCount / (attacksCount + extraMalusCount);
    }
    if (damage > 0)
    {
        float value = (attacksCount) / static_cast<float>(pEnemyUnits->size());
        if (attacksCount > m_minAttackCountBonus)
        {
            damage *= (attacksCount + m_minAttackCountBonus) / static_cast<float>(pEnemyUnits->size());
        }
        else
        {
            damage *= value;
        }
        // reduce effectiveness of units who can't attack a lot of units
        if (dummy.getMinRange(position) > 1)
        {
            for (qint32 i = m_indirectUnitAttackCountMalus; i > 1; --i)
            {
                float factor = 1 / static_cast<float>(i);
                if (value < factor)
                {
                    notAttackableCount *= factor;
                    damage *= factor;
                    break;
                }
            }
        }
    }
    ret.damage = damage;
    ret.notAttackableCount = notAttackableCount;
    ret.attackCount = attacksCount;
    if (counterAttacksCount > 0)
    {
        ret.counterDamage = counterDamageCount / counterAttacksCount;
    }
    return ret;
}

float NormalAi::calcSameFundsMatchUpScore(Unit& dummy, const QStringList & buildList)
{
    auto dummyValue = dummy.getUnitValue();
    float resultScore = 0;
    auto movepoints = dummy.getMovementpoints(dummy.getPosition());
    float count = 0;
    for (const auto & unitId : qAsConst(buildList))
    {
        Unit dummyMatchUp(unitId, m_pPlayer, false);
        auto matchUpMovepoints = dummyMatchUp.getMovementpoints(dummy.getPosition());
        auto matchUpValue = dummyMatchUp.getUnitValue();

        if (matchUpValue <= dummyValue + dummyValue * m_targetPriceDifference &&
            matchUpValue >= dummyValue - dummyValue * m_targetPriceDifference)
        {
            float dmg = dummy.getBaseDamage(&dummyMatchUp);
            if (dmg >= 0.0f)
            {
                if (matchUpMovepoints > movepoints)
                {
                    dmg *= 0.5f;
                }
                resultScore += dmg;
                ++count;
            }
        }
    }
    if (count > 0)
    {
        return resultScore / Unit::DAMAGE_100 / count;
    }
    else
    {
        return 1.0f;
    }
}

void NormalAi::getTransporterData(UnitBuildData & unitBuildData, Unit& dummy, spQmlVectorUnit & pUnits,
                                  spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                  QVector<std::tuple<Unit*, Unit*>>& transportTargets)
{
    QVector<QVector3D> targets;
    spQmlVectorUnit relevantUnits = spQmlVectorUnit::create();
    QPoint position = dummy.getPosition();
    float movement = dummy.getBaseMovementPoints();
    if (movement == 0)
    {
        movement = 1;
    }
    spGameMap pMap = GameMap::getInstance();
    qint32 loadingPlace = dummy.getLoadingPlace();
    qint32 smallTransporterCount = 0;
    qint32 transporterCount = 0;
    for (qint32 i2 = 0; i2 < pUnits->size(); i2++)
    {
        float distance = GlobalUtils::getDistance(position, pUnits->at(i2)->getPosition());
        if (distance / movement <= maxDayDistance * maxDayDistance)
        {
            relevantUnits->append(pUnits->at(i2));
        }
        qint32 place = pUnits->at(i2)->getLoadingPlace();
        if (place > 1)
        {
            if (place == 1)
            {
                smallTransporterCount++;
            }
            ++transporterCount;
        }
    }
    QVector<Unit*> loadingUnits = appendLoadingTargets(&dummy, relevantUnits, pEnemyUnits, pEnemyBuildings, false, true, targets, true);
    QVector<Unit*> transporterUnits;
    for (qint32 i2 = 0; i2 < transportTargets.size(); i2++)
    {
        if (!transporterUnits.contains(std::get<0>(transportTargets[i2])))
        {
            transporterUnits.append(std::get<0>(transportTargets[i2]));
        }
    }
    qint32 i = 0;
    while ( i < loadingUnits.size())
    {
        if (canTransportToEnemy(&dummy, loadingUnits[i], pEnemyUnits, pEnemyBuildings))
        {
            qint32 transporter = 0;
            for (qint32 i2 = 0; i2 < transportTargets.size(); i2++)
            {
                if (std::get<1>(transportTargets[i2])->getPosition() == loadingUnits[i]->getPosition())
                {
                    transporter++;
                    break;
                }
            }
            if (transporter == 0)
            {
                unitBuildData.noTransporterBonus += m_noTransporterBonus;
            }
            i++;
        }
        else
        {
            loadingUnits.removeAt(i);
        }
    }
    unitBuildData.smallTransporterCount = smallTransporterCount;
    unitBuildData.loadingPlace = loadingPlace;
    unitBuildData.transportCount = transporterUnits.size();
    unitBuildData.loadingCount = loadingUnits.size();
    unitBuildData.transporterCount = transporterCount;
    auto actions = dummy.getActionList();
    unitBuildData.utilityTransporter = !dummy.useTerrainDefense() ||
                                       actions.contains(CoreAI::ACTION_SUPPORTALL_RATION);
}

float NormalAi::calcTransporterScore(UnitBuildData & unitBuildData, spQmlVectorUnit & pUnits, QVector<float>& data)
{
    float score = 0.0f;
    if (score == 0.0f && pUnits->size() / (unitBuildData.smallTransporterCount + 1) > m_unitToSmallTransporterRatio && unitBuildData.loadingPlace == 1)
    {
        spGameMap pMap = GameMap::getInstance();
        if (unitBuildData.smallTransporterCount > 0)
        {
            score += qMin(m_smallTransporterBonus,  pUnits->size() / static_cast<float>(unitBuildData.smallTransporterCount + 1.0f) * 10.0f);

        }
        else
        {
            score += m_smallTransporterBonus;
        }
        // give a bonus to t-heli's or similar units cause they are mostlikly much faster
        if (score > m_minFlyingTransportScoreForBonus)
        {
            score += m_flyingTransporterBonus;
        }
    }
    if (unitBuildData.transportCount > 0 && unitBuildData.loadingCount > 0)
    {
        score += (unitBuildData.loadingCount / static_cast<float>(unitBuildData.transportCount)) * m_ProducingTransportRatioBonus;
    }
    else
    {
        score += unitBuildData.loadingCount * m_ProducingTransportLoadingBonus;
    }
    float supplyScore = calcSupplyScore(data, unitBuildData);
    score += supplyScore;
    if (unitBuildData.loadingCount > 0)
    {
        if (unitBuildData.transportCount <= 0 ||
            static_cast<float>(unitBuildData.loadingCount)  / static_cast<float>(unitBuildData.transportCount) >= m_ProducingTransportMinLoadingTransportRatio)
        {
            if (data[FundsFactoryRatio] <= m_cheapUnitRatio + m_targetPriceDifference)
            {
                score += (1 + m_cheapUnitRatio + m_targetPriceDifference - data[FundsFactoryRatio]) * m_normalUnitBonusMultiplier;
            }
            else
            {
                score -= (1 + m_cheapUnitRatio - m_targetPriceDifference - data[FundsFactoryRatio]) * m_cheapUnitBonusMultiplier;
            }
            score += unitBuildData.loadingPlace * m_additionalLoadingUnitBonus;
            score += unitBuildData.loadingCount * m_additionalLoadingUnitBonus;
            score += unitBuildData.noTransporterBonus;
            score -= unitBuildData.transporterCount;
            CONSOLE_PRINT("NormalAi::calcTransporterScore for " + unitBuildData.unitId +
                          " score=" + QString::number(score) +
                          " loadingCount=" + QString::number(unitBuildData.loadingCount) +
                          " transportCount=" + QString::number(unitBuildData.transportCount) +
                          " transportCounter=" + QString::number(unitBuildData.transporterCount), Console::eDEBUG);
        }
        else if (supplyScore <= 0.0f)
        {
            score = NO_BUILD_SCORE;
        }
    }
    else if (supplyScore <= 0.0f)
    {
        score = NO_BUILD_SCORE;
    }
    return score;
}

float NormalAi::calcBuildScore(QVector<float>& data, UnitBuildData & unitBuildData)
{
    float score = 0;
    // used index 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11
    if (data[InfantryUnit] == 0.0f)
    {
        if (data[IndirectUnit] == 1.0f)
        {
            // indirect unit
            if (data[DirectUnitRatio] > m_currentDirectIndirectRatio)
            {
                score += m_lowIndirectUnitBonus * (data[DirectUnitRatio] - m_currentDirectIndirectRatio);
            }
            else if (data[DirectUnitRatio] < m_currentDirectIndirectRatio / 2)
            {
                score -= m_highIndirectMalus * (m_currentDirectIndirectRatio - data[DirectUnitRatio]);
            }
            else if (data[DirectUnitRatio] < m_directIndirectRatio)
            {
                score -= m_lowIndirectMalus * (m_currentDirectIndirectRatio - data[DirectUnitRatio]);
            }
        }
        else if (data[DirectUnit] == 1.0f)
        {
            // direct unit
            if (data[DirectUnitRatio] < m_currentDirectIndirectRatio)
            {
                score += m_lowDirectUnitBonus * (m_currentDirectIndirectRatio - data[DirectUnitRatio]);
            }
            else if (data[DirectUnitRatio] > m_currentDirectIndirectRatio * 2)
            {
                score -= m_highDirectMalus * (data[DirectUnitRatio] - m_currentDirectIndirectRatio);
            }
            else if (data[DirectUnitRatio] > m_currentDirectIndirectRatio)
            {
                score -= m_lowDirectMalus * (data[DirectUnitRatio] - m_currentDirectIndirectRatio);
            }
        }
    }
    // infantry bonus
    if (data[InfantryUnit] == 1.0f)
    {
        float infScore = 0.0f;
        if (data[InfantryCount] <= m_minInfantryCount && data[BuildingEnemyRatio] < m_lowOwnBuildingEnemyBuildingRatio)
        {
            infScore += (m_minInfantryCount - data[InfantryCount]) * m_minInfantryCount + (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_lowIncomeInfantryBonusMultiplier;
        }
        else if (data[InfantryUnitRatio] < m_lowInfantryRatio)
        {
            infScore += (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_buildingBonusMultiplier;
        }
        else
        {
            infScore += (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_buildingBonusMultiplier;
        }
        if (infScore > 0.0f)
        {
            score += infScore;
        }
        else if (unitBuildData.unitId != UNIT_INFANTRY)
        {
            score += infScore;
        }
    }
    score += calcCostScore(data, unitBuildData);
    // apply movement bonus
    score += data[Movementpoints] * m_movementpointBonus;
    if (data[UnitCount] > m_minUnitCountForDamageBonus)
    {
        // apply not attackable unit bonus
        score += data[NotAttackableCount] * m_currentlyNotAttackableScoreBonus;
    }
    if (data[UnitCost] > 0 &&
       (data[UnitCount] > m_minUnitCountForDamageBonus ||
        GameMap::getInstance()->getCurrentDay() > m_earlyGame))
    {
        float attackScore = 0.0f;
        attackScore += data[DamageData] / data[UnitCost] * m_damageToUnitCostRatioBonus;
        attackScore += (1.0f - data[CounterDamage] / data[UnitCost]) * m_counterDamageBonus;
        if (data[EnemyUnitCount] > 0)
        {
            attackScore += data[AttackCount] / data[EnemyUnitCount] * m_attackCountBonus;
        }

        attackScore += data[SameFundsMatchUpScore] * m_sameFundsMatchUpBonus;
        score += attackScore;
        CONSOLE_PRINT("NormalAi::calcBuildScore damage=" + QString::number(data[DamageData]) +
                      " and counter damage " + QString::number(data[CounterDamage]) +
                      " attack score=" + QString::number(attackScore), Console::eDEBUG);
    }
    // apply co buff bonus
    score += data[COBonus] * m_coUnitBuffBonus;
    score += calcSupplyScore(data, unitBuildData);
    if (data[ReachDistance] > 0 && data[Movementpoints] > 0)
    {
        score += m_nearEnemyBonus * data[Movementpoints] / GlobalUtils::roundUp(data[ReachDistance]);
    }
    if (data[UnitCount] > m_minUnitCountForDamageBonus &&
        data[AttackCount] <= 0 && data[Movementpoints] > 0 &&
        !unitBuildData.isTransporter)
    {
        score = NO_BUILD_SCORE;
    }
    CONSOLE_PRINT("NormalAi::calcBuildScore final score=" + QString::number(score) + " for " + unitBuildData.unitId, Console::eDEBUG);
    return score;
}

float NormalAi::calcSupplyScore(QVector<float>& data, UnitBuildData & unitBuildData)
{
    float score = 0.0f;
    if (unitBuildData.isSupplyUnit && data[SupplyRatio] <= m_maxSupplyUnitRatio)
    {
        qint32 supplyUnits = data[SupplyRatio] * data[UnitCount];
        qint32 supplyNeeds = data[RequiredSupplyRatio] * data[UnitCount] - supplyUnits * m_averageSupplySupport;
        score += m_canSupplyBonus * supplyNeeds;
    }
    return score;
}

float NormalAi::calcCostScore(QVector<float>& data, UnitBuildData & unitBuildData)
{
    float score = 0;
    // funds bonus
    if (data[UseHighTechUnits] > 0.0f && data[FundsFactoryRatio] > m_normalUnitRatio + m_targetPriceDifference)
    {
        score = 0;
    }
    else if (data[UseHighTechUnits] > 0.0f && data[FundsFactoryRatio] > m_normalUnitRatio -  m_targetPriceDifference)
    {
        score += (1 + m_normalUnitRatio + m_targetPriceDifference - data[FundsFactoryRatio]) * m_expensiveUnitBonusMultiplier;
    }
    else if (data[FundsFactoryRatio] > m_superiorityRatio)
    {
        score -= (data[FundsFactoryRatio] - (1.0f - data[UnitEnemyRatio])) * m_expensiveUnitBonusMultiplier;
    }
    else if (data[FundsFactoryRatio] >= m_normalUnitRatio - m_targetPriceDifference &&
             data[FundsFactoryRatio] <= m_normalUnitRatio + m_targetPriceDifference)
    {
        score += (1 + m_normalUnitRatio + m_targetPriceDifference - data[FundsFactoryRatio]) * m_normalUnitBonusMultiplier;
    }
    else if (data[FundsFactoryRatio] < m_cheapUnitRatio + m_targetPriceDifference)
    {
        if (data[LowFunds] > 0)
        {
            score += (2 + m_cheapUnitRatio - data[FundsFactoryRatio]) * m_cheapUnitBonusMultiplier;
        }
        else
        {
            score -= (1 + m_cheapUnitRatio - m_targetPriceDifference - data[FundsFactoryRatio]) * m_cheapUnitBonusMultiplier;
        }
    }
    else
    {
        if (data[LowFunds] > 0 &&
            data[FundsFactoryRatio] < m_normalUnitRatio - m_targetPriceDifference)
        {
            score += (1 + m_cheapUnitRatio - data[FundsFactoryRatio]) * m_cheapUnitBonusMultiplier;
        }
        else
        {
            score -= (2 + data[FundsFactoryRatio] - m_normalUnitRatio + m_targetPriceDifference) * m_expensiveUnitBonusMultiplier;
        }
    }
    CONSOLE_PRINT("NormalAi::calcCostScore score=" + QString::number(score) +
                  " funds ratio=" + QString::number(data[FundsFactoryRatio]), Console::eDEBUG);
    return score;
}
