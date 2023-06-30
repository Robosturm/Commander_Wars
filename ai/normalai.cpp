#include <QElapsedTimer>
#include <QCoreApplication>

#include "coreengine/qmlvector.h"
#include "coreengine/globalutils.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"

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


NormalAi::NormalAi(GameMap* pMap, QString configurationFile, GameEnums::AiTypes aiType, QString jsName)
    : CoreAI(pMap, aiType, jsName),
      m_InfluenceFrontMap(pMap, m_IslandMaps)
{
#ifdef GRAPHICSUPPORT
    setObjectName("NormalAi");
#endif
    CONSOLE_PRINT("Creating normal ai", GameConsole::eDEBUG);
    Interpreter::setCppOwnerShip(this);
    m_timer.setSingleShot(false);
    connect(&m_timer, &QTimer::timeout, this, &NormalAi::process, Qt::QueuedConnection);
    m_iniData = { // General
                  {"MinMovementDamage", "General", &m_minMovementDamage, 0.3f, -1.0f, 1.0f},
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
                  {"EnemyPruneRange", "General", &m_enemyPruneRange, 3.0f, 1.0f, 5.0f},
                  // CO Unit
                  {"CoUnitValue", "CoUnit", &m_coUnitValue, 6000.0f, 5000.0f, 20000.0f},
                  {"MinCoUnitScore", "CoUnit", &m_minCoUnitScore, 5000.0f, 3000.0f, 20000.0f},
                  {"CoUnitRankReduction", "CoUnit", &m_coUnitRankReduction, 1000.0f, 0.0f, 5000.0f},
                  {"CoUnitScoreMultiplier", "CoUnit", &m_coUnitScoreMultiplier, 1.1f, 1.0f, 3.0f},
                  {"MinCoUnitCount", "CoUnit", &m_minCoUnitCount, 5.0f, 1.0f, 10.0f},
                  // Repairing
                  {"MinUnitHealth", "Repairing", &m_minUnitHealth, 3.0f, 0.0f, 10.0f},
                  {"MaxUnitHealth", "Repairing", &m_maxUnitHealth, 7.0f, 0.0f, 10.0f},
                  {"FuelResupply", "Repairing", &m_fuelResupply, 0.33f, 0.0f, 0.7f},
                  {"AmmoResupply", "Repairing", &m_ammoResupply, 0.25f, 0.0f, 0.7f},
                  // Moving
                  {"LockedUnitHp", "Moving", &m_lockedUnitHp, 4.0f, 1.0f, 10.0f},
                  {"NoMoveAttackHp", "Moving", &m_noMoveAttackHp, 3.5f, 1.0f, 10.0f},
                  {"MinTerrainDamage", "Moving", &m_minTerrainDamage, 20.0f, 20.0f, 20.0f},
                  {"InfluenceUnitRange", "Moving", &m_influenceUnitRange, 1.75f, 1.0f, 5.0f},

                  // Attacking
                  {"OwnIndirectAttackValue", "Attacking", &m_ownIndirectAttackValue, 2.0f, 0.1f, 10.0f},
                  {"EnemyKillBonus", "Attacking", &m_enemyKillBonus, 2.0f, 0.1f, 10.0f},
                  {"EnemyIndirectBonus", "Attacking", &m_enemyIndirectBonus, 3.0f, 0.1f, 10.0f},
                  {"AntiCaptureHqBonus", "Attacking", &m_antiCaptureHqBonus, 50.0f, 0.0f, 10000.0f},
                  {"AntiCaptureBonus", "Attacking", &m_antiCaptureBonus, 21.0f, 0.0f, 100.0f},
                  {"AntiCaptureBonusScoreReduction", "Attacking", &m_antiCaptureBonusScoreReduction, 6.0f, 0.0f, 200.0f},
                  {"AntiCaptureBonusScoreDivider", "Attacking", &m_antiCaptureBonusScoreDivider, 2.0f, 0.1f, 10.0f},
                  {"EnemyCounterDamageMultiplier", "Attacking", &m_enemyCounterDamageMultiplier, 10.0f, 0.1f, 40.0f},
                  {"WatermineDamage", "Attacking", &m_watermineDamage, 4.0f, 4.0f, 4.0f},
                  {"EnemyUnitCountDamageReductionMultiplier", "Attacking", &m_enemyUnitCountDamageReductionMultiplier, 0.5f, 0.01f, 10.0f},
                  {"OwnProdctionMalus", "Attacking", &m_ownProdctionMalus, 5000.0f, 5000.0f, 50000.0f},
                  {"SupportDamageBonus", "Attacking", &m_supportDamageBonus, 1.0f, 0.1f, 10.0f},
                  {"InfluenceIgnoreValue", "Attacking", &m_influenceIgnoreValue, 0.2f, 0.01f, 1.0f},
                  {"InfluenceMultiplier", "Attacking", &m_influenceMultiplier, 2.0f, 0.1f, 10.0f},
                  {"MinHpDamage", "Attacking", &m_minHpDamage, -2.0f, -10.0f, 0.0f},

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
                  {"SameFundsMatchUpMovementMalus", "Production", &m_sameFundsMatchUpMovementMalus, 0.3f, 0.1f, 1.0f},
                  {"AttackCountBonus", "Production", &m_attackCountBonus, 25.0f, 5.0f, 60.0f},
                  {"MaxOverkillBonus", "Production", &m_maxOverkillBonus, 2.0f, 1.5f, 10.0f},
                  {"CounterDamageBonus", "Production", &m_counterDamageBonus, 25.0f, 1.0f, 100.0f},
                  {"EarlyGame", "Production", &m_earlyGame, 5.0f, 2.f, 10.0f},
                  {"MaxProductionBuildings", "Production", &m_maxProductionBuildings, 5.0f, 2.f, 10.0f},
                  {"LowThreadDamage", "Production", &m_lowThreadDamage, 10.0f, 1.f, 20.0f},
                  {"MaxProductionBuildingsForB", "Production", &m_maxProductionBuildingsForB, 1.0f, 1.f, 5.0f},
                  {"TurnOneDmageMalus", "Production", &m_turnOneDmageMalus, 10.0f, 1.f, 20.0f},
                  {"CounterUnitRatio", "Production", &m_counterUnitRatio, 2.0f, 1.f, 5.0f},
                  {"SpamInfantryChance", "Production", &m_spamInfantryChance, 50.0f, 100.0f, 50.0f},
                  {"SameFundsMatchUpNoMatchUpValue", "Production", &m_sameFundsMatchUpNoMatchUpValue, 0.5f, 0.0f, 1.0f},
                  {"SpamLightUnitChance", "Production", &m_spamLightUnitChance, 30.0f, 0.0f, 100.0f},
                  {"SpamMediumUnitChance", "Production", &m_spamMediumUnitChance, 30.0f, 0.0f, 100.0f},
                };

    if (m_pMap != nullptr &&
        !m_pMap->getSavegame())
    {
        loadIni( "normal/" + configurationFile);
    }
    m_BuildingChanceModifier.insert("MECH", 1.1f);
}

void NormalAi::process()
{
    AI_CONSOLE_PRINT("NormalAi::process()", GameConsole::eDEBUG);
    if (m_pause)
    {
        m_timer.start(1000);
        return;
    }
    else
    {
        m_timer.stop();
    }
    spQmlVectorBuilding pBuildings = spQmlVectorBuilding(m_pPlayer->getBuildings());
    pBuildings->randomize();
    spQmlVectorUnit pUnits;
    pUnits = spQmlVectorUnit(m_pPlayer->getUnits());
    spQmlVectorUnit pEnemyUnits;
    spQmlVectorBuilding pEnemyBuildings;
    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= m_minSiloDamage);
    if (useBuilding(pBuildings, pUnits)){}
    else
    {
        AI_CONSOLE_PRINT("NormalAi::creating unit arrays()", GameConsole::eDEBUG);
        prepareEnemieData(pUnits, pEnemyUnits, pEnemyBuildings);
        updateAllUnitData(pUnits);
        if (useCOPower(pUnits, pEnemyUnits))
        {
            clearUnitData();
        }
        else
        {
            m_turnMode = GameEnums::AiTurnMode_DuringDay;
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
                        m_aiFunctionStep = 0;
                    }
                    clearUnitData();
                    m_IslandMaps.clear();
                    m_turnMode = GameEnums::AiTurnMode_EndOfDay;
                    if (useCOPower(pUnits, pEnemyUnits))
                    {
                        m_usedTransportSystem = false;
                        m_usedPredefinedAi = false;
                        m_turnMode = GameEnums::AiTurnMode_DuringDay;
                    }
                    else
                    {
                        m_turnMode = GameEnums::AiTurnMode_StartOfDay;
                        finishTurn();
                    }
                }
            }
        }
    }
}

void NormalAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void NormalAi::showIslandMap(QString unitId)
{
    Unit unit(unitId, m_pPlayer, false, m_pMap);
    qint32 unitIslandIdx = getIslandIndex(&unit);
    if (unitIslandIdx >= 0 && unitIslandIdx < m_IslandMaps.size())
    {
        m_IslandMaps[unitIslandIdx]->show();
    }
}

void NormalAi::hideIslandMap(QString unitId)
{
    Unit unit(unitId, m_pPlayer, false, m_pMap);
    qint32 unitIslandIdx = getIslandIndex(&unit);
    if (unitIslandIdx >= 0 && unitIslandIdx < m_IslandMaps.size())
    {
        m_IslandMaps[unitIslandIdx]->hide();
    }
}

void NormalAi::showFrontMap()
{
    m_InfluenceFrontMap.show();
}

void NormalAi::showFrontLines()
{
    m_InfluenceFrontMap.showFrontlines();
}

void NormalAi::hideFrontMap()
{
    m_InfluenceFrontMap.hide();
}

void NormalAi::resetToTurnStart()
{
    clearUnitData();
    m_productionData.clear();
    m_secondMoveRound = false;
    CoreAI::resetToTurnStart();
}

void NormalAi::finishTurn()
{
    AI_CONSOLE_PRINT("NormalAi::finishTurn()", GameConsole::eDEBUG);
    m_productionData.clear();
    m_secondMoveRound = false;
    CoreAI::finishTurn();
}

bool NormalAi::performActionSteps(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits,
                                  spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::performActionSteps()", GameConsole::eDEBUG);
    if (m_aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveFlares(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && captureBuildings(pUnits,  pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveUnits && joinCaptureBuildings(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    // indirect units
    else if (m_aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 1, 2, std::numeric_limits<qint32>::max(), pBuildings, pEnemyBuildings)){}
    // direct units
    else if (m_aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 1, 1, 1, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveUnits && repairUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveToTargets && refillUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, 1)){}
    else if (m_aiStep <= AISteps::moveIndirectsToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 2, std::numeric_limits<qint32>::max())){}
    else if (m_aiStep <= AISteps::loadUnits && !m_usedTransportSystem && loadUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::moveTransporters && !m_usedTransportSystem && moveTransporters(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
    else
    {
        if (!m_usedTransportSystem)
        {
            m_usedTransportSystem = true;
            m_aiStep = AISteps::moveUnits;
            m_aiFunctionStep = 0;
            prepareEnemieData(pUnits, pEnemyUnits, pEnemyBuildings);
            for (auto & unit : m_OwnUnits)
            {
                if (!unit.pUnit->getHasMoved())
                {
                    unit.nextAiStep = m_aiFunctionStep;
                }
            }
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
    
    if (m_pMap->onMap(pUnit->Unit::getX(), pUnit->Unit::getY()))
    {
        Building* pBuilding = m_pMap->getTerrain(pUnit->Unit::getX(), pUnit->Unit::getY())->getBuilding();
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

bool NormalAi::captureBuildings(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::captureBuildings()", GameConsole::eDEBUG);
    struct CaptureInfo
    {
        qint32 x;
        qint32 y;
        qint32 unitIdx;
        bool farAway{false};
    };

    QStringList highPrioBuildings;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({pInterpreter->newQObject(this)});
    auto erg = pInterpreter->doFunction(getAiName(), "getHighPrioBuildings", args);
    highPrioBuildings = erg.toVariant().toStringList();

    std::vector<CaptureInfo> captureBuildings;
    qint32 cost = 0;
    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
    bool fireSilos = hasMissileTarget();

    for (qint32 i = 0; i < m_OwnUnits.size(); ++i)
    {
        QCoreApplication::processEvents();
        auto & unitData = m_OwnUnits[i];
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            Unit* pUnit = unitData.pUnit.get();
            if (!pUnit->getHasMoved() &&
                unitData.actions.contains(ACTION_CAPTURE) &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                if (pUnit->getCapturePoints() > 0)
                {
                    spGameAction pAction = spGameAction::create(ACTION_CAPTURE, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    if (pAction->canBePerformed())
                    {
                        ++unitData.nextAiStep;
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
                else
                {
                    GameAction action(ACTION_CAPTURE, m_pMap);
                    action.setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    auto targets = unitData.pUnitPfs->getAllNodePointsFast();
                    for (auto & target : targets)
                    {
                        action.setActionID(ACTION_CAPTURE);
                        action.setMovepath(QVector<QPoint>(1, target), 0);
                        if (unitData.pUnitPfs->getTargetCosts(target.x(), target.y()) < unitData.movementPoints + 1)
                        {
                            if (action.canBePerformed())
                            {
                                captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, false));
                            }
                            else
                            {
                                action.setActionID(ACTION_MISSILE);
                                if (action.canBePerformed() && fireSilos)
                                {
                                    captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, false));
                                }
                            }
                        }
                        else
                        {
                            Terrain* pTerrain = m_pMap->getTerrain(target.x(), target.y());
                            auto* pBuilding = pTerrain->getBuilding();
                            if (pBuilding != nullptr &&
                                pBuilding->getOwner() == nullptr &&
                                highPrioBuildings.contains(pBuilding->getBuildingID()) &&
                                pTerrain->getUnit() == nullptr)
                            {
                                if (action.canBePerformed())
                                {
                                    captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, true));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (captureBuildings.size() > 0)
    {
        for (qint32 i = 0; i < m_OwnUnits.size(); ++i)
        {
            auto & unitData = m_OwnUnits[i];
            if (unitData.nextAiStep <= m_aiFunctionStep)
            {
                QCoreApplication::processEvents();
                Unit* pUnit = unitData.pUnit.get();
                if (!pUnit->getHasMoved() &&
                    unitData.actions.contains(ACTION_CAPTURE) &&
                    pUnit->getAiMode() == GameEnums::GameAi_Normal)
                {
                    std::vector<CaptureInfo> captures;
                    for (auto & building : captureBuildings)
                    {
                        if (building.unitIdx == i)
                        {
                            captures.push_back(building);
                        }
                    }
                    bool perform = false;
                    qint32 targetIndex = 0;
                    if (captures.size() > 0)
                    {
                        if (captures.size() == 1)
                        {
                            // we have only one target go for it
                            targetIndex = 0;
                            perform = true;
                        }
                        else
                        {
                            qint32 prio = -1;
                            // check if we have a building only we can capture and capture it
                            qint32 i2 = 0;
                            while (i2 < captures.size())
                            {
                                qint32 captureCount = 0;
                                for (auto & buildingPos2 : captureBuildings)
                                {
                                    if (buildingPos2.x == captures[i2].x &&
                                        buildingPos2.y == captures[i2].y)
                                    {
                                        if (buildingPos2.farAway == captures[i2].farAway ||
                                            (buildingPos2.farAway == true && captures[i2].farAway == false))
                                        {
                                            ++captureCount;
                                        }
                                        else
                                        {
                                            --captureCount;
                                        }
                                    }
                                }
                                if (captureCount <= 0)
                                {
                                    captures.erase(captures.cbegin() + i2);
                                }
                                else
                                {
                                    bool isProductionBuilding = m_pMap->getTerrain(captures[i2].x, captures[i2].y)->getBuilding()->isProductionBuilding();
                                    if ((captureCount == 1 && perform == false) ||
                                        (captureCount == 1 && perform == true && isProductionBuilding))
                                    {
                                        Building* pBuilding = m_pMap->getTerrain(captures[i2].x, captures[i2].y)->getBuilding();
                                        qint32 testPrio = std::numeric_limits<qint32>::min();
                                        if (pBuilding->getBuildingID() == CoreAI::BUILDING_HQ)
                                        {
                                            testPrio = std::numeric_limits<qint32>::max();
                                        }
                                        else if (pBuilding->isProductionBuilding())
                                        {
                                            testPrio = pBuilding->getConstructionList().size();
                                        }
                                        if (!perform)
                                        {
                                            prio = testPrio;
                                            targetIndex = i2;
                                        }
                                        else
                                        {
                                            if (testPrio > prio)
                                            {
                                                targetIndex = i2;
                                                prio = testPrio;
                                            }
                                        }
                                        perform = true;
                                    }
                                    ++i2;
                                }
                            }
                            // if not we can select a target from the list
                            if (!perform &&
                                captures.size() > 0)
                            {
                                prio = -1;
                                targetIndex = 0;
                                // priorities production buildings over over captures
                                for (qint32 i2 = 0; i2 < captures.size(); i2++)
                                {
                                    Building* pBuilding = m_pMap->getTerrain(captures[i2].x, captures[i2].y)->getBuilding();
                                    qint32 testPrio = std::numeric_limits<qint32>::min();
                                    if (pBuilding->getBuildingID() == CoreAI::BUILDING_HQ)
                                    {
                                        testPrio = std::numeric_limits<qint32>::max();
                                    }
                                    else if (pBuilding->isProductionBuilding())
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
                        ++unitData.nextAiStep;
                        if (captures[targetIndex].farAway)
                        {
                            spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                            std::vector<QVector3D> targets;
                            targets.push_back(QVector3D(captures[targetIndex].x, captures[targetIndex].y, 1));
                            std::vector<QVector3D> transporterTargets;
                            if (moveUnit(pAction, &unitData, pUnits, unitData.actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                            {
                                return true;
                            }
                        }
                        else
                        {
                            spGameAction pAction = spGameAction::create(ACTION_CAPTURE, m_pMap);
                            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                            auto path = unitData.pUnitPfs->getPathFast(captures[targetIndex].x, captures[targetIndex].y);
                            pAction->setMovepath(path, unitData.pUnitPfs->getCosts(path));
                            m_updatePoints.push_back(pUnit->getPosition());
                            m_updatePoints.push_back(pAction->getActionTarget());
                            if (pAction->canBePerformed())
                            {
                                emit sigPerformAction(pAction);
                                return true;
                            }
                            else
                            {
                                CoreAI::addSelectedFieldData(pAction, rocketTarget);
                                pAction->setActionID(ACTION_MISSILE);
                                if (pAction->canBePerformed())
                                {
                                    emit sigPerformAction(pAction);
                                    return true;
                                }
                            }
                        }
                        qint32 i2 = 0;
                        while (i2 < captureBuildings.size())
                        {
                            if (captureBuildings[i2].x == captures[targetIndex].x &&
                                captureBuildings[i2].y == captures[targetIndex].y)
                            {
                                captureBuildings.erase(captureBuildings.cbegin() + i2);
                            }
                            else
                            {
                                ++i2;
                            }
                        }
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::joinCaptureBuildings(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("NormalAi::joinCaptureBuildings()", GameConsole::eDEBUG);
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            if (!pUnit->getHasMoved() &&
                unitData.actions.contains(ACTION_CAPTURE) &&
                unitData.actions.contains(ACTION_JOIN) &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                spGameAction pAction = spGameAction::create(ACTION_JOIN, m_pMap);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                auto targets = unitData.pUnitPfs->getAllNodePointsFast(unitData.movementPoints + 1);
                for (auto & target : targets)
                {
                    pAction->setMovepath(QVector<QPoint>(1, target), 0);
                    if (pAction->canBePerformed())
                    {
                        Unit* pCaptureUnit = m_pMap->getTerrain(target.x(), target.y())->getUnit();
                        if (pCaptureUnit->getCapturePoints() > 0)
                        {
                            auto path = unitData.pUnitPfs->getPathFast(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()));
                            pAction->setMovepath(path, unitData.pUnitPfs->getCosts(path));
                            m_updatePoints.push_back(pUnit->getPosition());
                            m_updatePoints.push_back(pAction->getActionTarget());
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::fireWithUnits(spQmlVectorUnit & pUnits, qint32 minfireRange, qint32 minMaxfireRange, qint32 maxfireRange,
                             spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::fireWithUnits()", GameConsole::eDEBUG);
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            if (!pUnit->getHasMoved() &&
                unitData.minFireRange >= minfireRange &&
                unitData.maxFireRange >= minMaxfireRange &&
                unitData.maxFireRange <= maxfireRange &&
                (pUnit->getAmmo1() != 0 || pUnit->getAmmo2() != 0) &&
                unitData.actions.contains(CoreAI::ACTION_FIRE) &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                std::vector<CoreAI::DamageData> ret;
                std::vector<QVector3D> moveTargetFields;
                CoreAI::getAttackTargets(pUnit, pAction, unitData.pUnitPfs.get(), ret, moveTargetFields, unitData.movementPoints + 1);
                qint32 targetIdx = getBestAttackTarget(unitData, ret, moveTargetFields, pBuildings, pEnemyBuildings);
                if (targetIdx >= 0)
                {
                    CoreAI::DamageData target = ret[targetIdx];
                    auto path = unitData.pUnitPfs->getPathFast(static_cast<qint32>(moveTargetFields[targetIdx].x()),
                                                               static_cast<qint32>(moveTargetFields[targetIdx].y()));
                    pAction->setMovepath(path, unitData.pUnitPfs->getCosts(path));
                    CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x), static_cast<qint32>(target.y)));
                    if (m_pMap->getTerrain(static_cast<qint32>(target.x), static_cast<qint32>(target.y))->getUnit() == nullptr)
                    {
                        m_IslandMaps.clear();
                    }
                    if (pAction->isFinalStep() && pAction->canBePerformed())
                    {
                        m_updatePoints.push_back(pUnit->getPosition());
                        m_updatePoints.push_back(pAction->getActionTarget());
                        m_updatePoints.push_back(QPoint(static_cast<qint32>(target.x), static_cast<qint32>(target.y)));
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::refillUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::refillUnits()", GameConsole::eDEBUG);
    if (m_aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    m_aiStep = AISteps::moveToTargets;
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            // can we use the unit?
            if (isUsingUnit(pUnit) &&
                pUnit->getLoadedUnitCount() == 0 &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                QStringList & actions = unitData.actions;
                if (isRefuelUnit(actions))
                {
                    spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    bool found = false;
                    QPoint moveTarget;
                    if (actions.contains(ACTION_SUPPORTALL_RATION))
                    {
                        pAction->setActionID(ACTION_SUPPORTALL_RATION);
                        QPoint refillTarget;
                        found = getBestRefillTarget(*unitData.pUnitPfs.get(), 4, moveTarget, refillTarget, unitData.movementPoints);
                    }
                    else if (actions.contains(ACTION_SUPPORTALL_RATION))
                    {
                        pAction->setActionID(ACTION_SUPPORTALL_RATION_MONEY);
                        QPoint refillTarget;
                        found = getBestRefillTarget(*unitData.pUnitPfs.get(), 4, moveTarget, refillTarget, unitData.movementPoints);
                    }
                    else if (actions.contains(ACTION_SUPPORTSINGLE_REPAIR))
                    {
                        pAction->setActionID(ACTION_SUPPORTSINGLE_REPAIR);
                        QPoint refillTarget;
                        found = getBestRefillTarget(*unitData.pUnitPfs.get(), 1, moveTarget, refillTarget, unitData.movementPoints);
                        CoreAI::addSelectedFieldData(pAction, refillTarget);
                    }
                    else if (actions.contains(ACTION_SUPPORTSINGLE_FREEREPAIR))
                    {
                        pAction->setActionID(ACTION_SUPPORTSINGLE_FREEREPAIR);
                        QPoint refillTarget;
                        found = getBestRefillTarget(*unitData.pUnitPfs.get(), 1, moveTarget, refillTarget, unitData.movementPoints);
                        CoreAI::addSelectedFieldData(pAction, refillTarget);
                    }
                    else if (actions.contains(ACTION_SUPPORTSINGLE_SUPPLY))
                    {
                        pAction->setActionID(ACTION_SUPPORTSINGLE_SUPPLY);
                        QPoint refillTarget;
                        found = getBestRefillTarget(*unitData.pUnitPfs.get(), 1, moveTarget, refillTarget, unitData.movementPoints);
                        CoreAI::addSelectedFieldData(pAction, refillTarget);
                    }
                    if (found)
                    {
                        auto path = unitData.pUnitPfs->getPathFast(moveTarget.x(), moveTarget.y());
                        pAction->setMovepath(path, unitData.pUnitPfs->getCosts(path));
                        if (pAction->canBePerformed())
                        {
                            m_updatePoints.push_back(pUnit->getPosition());
                            m_updatePoints.push_back(pAction->getActionTarget());
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                    else if (m_usedTransportSystem)
                    {
                        AI_CONSOLE_PRINT("move to supply needed units", GameConsole::eDEBUG);
                        std::vector<QVector3D> targets;
                        std::vector<QVector3D> transporterTargets;
                        pAction->setActionID(ACTION_WAIT);
                        appendRefillTargets(actions, pUnit, pUnits, targets);
                        if (moveUnit(pAction, &unitData, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::getBestRefillTarget(UnitPathFindingSystem & pfs, qint32 maxRefillCount, QPoint & moveTarget, QPoint & refillTarget, qint32 movepoints) const
{
    bool ret = false;
    const auto points = pfs.getAllNodePointsFast(movepoints + 1);
    
    spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    qint32 highestCount = 0;
    for (const auto & point : points)
    {
        if (m_pMap->getTerrain(point.x(), point.y())->getUnit() == nullptr)
        {
            qint32 count = 0;
            for (const auto circlePos : circle->getVector())
            {
                qint32 x = point.x() + circlePos.x();
                qint32 y = point.y() + circlePos.y();
                if (m_pMap->onMap(x, y))
                {
                    Unit* pSupplyUnit = m_pMap->getTerrain(x, y)->getUnit();
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

void NormalAi::appendRefillTargets(const QStringList & actions, Unit* pUnit, spQmlVectorUnit & pUnits, std::vector<QVector3D>& targets)
{
    if (isRefuelUnit(actions))
    {
        spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        
        qint32 islandIdx = getIslandIndex(pUnit);
        qint32 curX = pUnit->Unit::getX();
        qint32 curY = pUnit->Unit::getY();
        for (auto & pSupplyUnit : pUnits->getVector())
        {
            if (needsRefuel(pSupplyUnit.get()))
            {
                qint32 unitX = pSupplyUnit->Unit::getX();
                qint32 unitY = pSupplyUnit->Unit::getY();
                for (auto & point : circle->getVector())
                {
                    qint32 x = unitX + point.x();
                    qint32 y = unitY + point.y();
                    if (m_pMap->onMap(x, y))
                    {
                        if (onSameIsland(islandIdx, curX, curY, x, y))
                        {
                            if (!GlobalUtils::contains(targets, QVector3D(x, y, 1)))
                            {
                                targets.push_back(QVector3D(x, y, 1));
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
    AI_CONSOLE_PRINT("NormalAi::moveUnits()", GameConsole::eDEBUG);
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            constexpr qint32 AVERAGE_TRANSPORTER_MOVEMENT = 7;
            bool canCapture = unitData.actions.contains(ACTION_CAPTURE);
            qint32 loadingIslandIdx = getIslandIndex(pUnit);
            qint32 loadingIsland = getIsland(pUnit);

            // can we use the unit?
            if (!pUnit->getHasMoved() &&
                pUnit->getBaseMaxRange() >= minfireRange &&
                pUnit->getBaseMaxRange() <= maxfireRange &&
                pUnit->hasWeapons() && pUnit->getLoadedUnitCount() == 0 &&
                (m_usedTransportSystem || (isUsingUnit(pUnit) && hasTargets(AVERAGE_TRANSPORTER_MOVEMENT, pUnit, canCapture, pEnemyUnits.get(), pEnemyBuildings.get(),
                                                                            loadingIslandIdx, loadingIsland, false))))
            {
                std::vector<QVector3D> targets;
                std::vector<QVector3D> transporterTargets;
                spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                QStringList & actions = unitData.actions;
                qint32 distanceModifier = 1;
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                if (pUnit->getAiMode() == GameEnums::GameAi_Normal)
                {
                    // find some cool targets
                    appendCaptureTargets(actions, pUnit, pEnemyBuildings, targets);
                }
                if (targets.size() > 0)
                {
                    distanceModifier = 4;
                    appendCaptureTransporterTargets(pUnit, pUnits, pEnemyBuildings, transporterTargets, distanceModifier);
                    distanceModifier = 5;
                    targets.insert(targets.cbegin(), transporterTargets.cbegin(), transporterTargets.cend());
                }
                if (pUnit->getAiMode() == GameEnums::GameAi_Normal)
                {
                    appendAttackTargets(pUnit, pEnemyUnits, targets, distanceModifier);
                    appendAttackTargetsIgnoreOwnUnits(pUnit, pEnemyUnits, targets, distanceModifier);
                    appendTerrainBuildingAttackTargets(pUnit, pEnemyBuildings, targets, distanceModifier);
                    if (targets.size() == 0)
                    {
                        appendRepairTargets(pUnit, pBuildings, targets);
                    }
                    if (supportUnits)
                    {
                        appendSupportTargets(actions, pUnit, pUnits, pEnemyUnits, targets, distanceModifier);
                    }
                }
                if (targets.size() > 0 || transporterTargets.size() > 0)
                {
                    if (moveUnit(pAction, &unitData, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                    {
                        return true;
                    }
                }
            }
        }
    }
    if (!m_secondMoveRound)
    {
        createUnitInfluenceMap();
        m_secondMoveRound = true;
        m_aiStep = AISteps::moveUnits;
    }
    else
    {
        m_aiStep = AISteps::moveIndirectsToTargets;
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::loadUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::loadUnits()", GameConsole::eDEBUG);
    m_aiStep = AISteps::loadUnits;
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            // can we use the unit?
            if (!pUnit->getHasMoved() &&
                (pUnit->getLoadingPlace() == 0 || (pUnit->getLoadedUnitCount() > 0 && m_usedTransportSystem)))
            {
                std::vector<QVector3D> targets;
                std::vector<QVector3D> transporterTargets;
                spGameAction pAction = spGameAction::create(ACTION_LOAD, m_pMap);
                QStringList & actions = unitData.actions;
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));

                // find some cool targets
                appendTransporterTargets(pUnit, pUnits, transporterTargets);
                targets.insert(targets.cbegin(), transporterTargets.cbegin(), transporterTargets.cend());
                // till now the selected targets are a little bit lame cause we only search for reachable transporters
                // but not for reachable loading places.
                if (moveUnit(pAction, &unitData, pUnits, actions, targets, transporterTargets, false, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::moveTransporters(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::moveTransporters()", GameConsole::eDEBUG);
    m_aiStep = AISteps::moveTransporters;
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            // can we use the unit?
            if (!pUnit->getHasMoved() &&
                pUnit->getLoadingPlace() > 0 &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                // wooohooo it's a transporter
                if (pUnit->getLoadedUnitCount() > 0)
                {
                    spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                    QStringList & actions = unitData.actions;
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    // find possible targets for this unit
                    std::vector<QVector3D> targets;
                    // can one of our units can capture buildings?
                    bool captureFound = false;
                    bool attackFound = false;
                    for (auto & pLoaded : pUnit->getLoadedUnits())
                    {
                        QStringList actions = pLoaded->getActionList();
                        if (!captureFound && actions.contains(ACTION_CAPTURE))
                        {
                            appendUnloadTargetsForCapturing(pUnit, pUnits, pEnemyBuildings, targets);
                            captureFound = true;
                        }
                    }
                    if (!captureFound)
                    {
                        for (auto & pLoaded : pUnit->getLoadedUnits())
                        {
                            QStringList actions = pLoaded->getActionList();
                            if (!attackFound && actions.contains(ACTION_FIRE))
                            {
                                appendUnloadTargetsForAttacking(pUnit, pEnemyUnits, targets, 1);
                                attackFound = true;
                            }
                            if (attackFound)
                            {
                                break;
                            }
                        }
                    }
                    // if not find closest unloading field
                    if (targets.size() == 0)
                    {
                        appendUnloadTargetsForAttacking(pUnit, pEnemyUnits, targets, 3);
                    }
                    if (targets.size() == 0)
                    {
                        appendNearestUnloadTargets(pUnit, pEnemyUnits, pEnemyBuildings, targets);
                    }
                    if (moveToUnloadArea(pAction, &unitData, pUnits, actions, targets, pBuildings, pEnemyBuildings, pEnemyUnits))
                    {
                        return true;
                    }
                }
                else
                {
                    spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                    QStringList & actions = unitData.actions;
                    // find possible targets for this unit
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    // we need to move to a loading place
                    std::vector<QVector3D> targets;
                    std::vector<QVector3D> transporterTargets;
                    appendCaptureTargets(actions, pUnit, pEnemyBuildings, targets);
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, false, targets, false, 5);
                    if (targets.size() == 0)
                    {
                        appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, false, targets, false, 5);
                    }
                    if (moveUnit(pAction, &unitData, pUnits, actions, targets, transporterTargets, false, pBuildings, pEnemyBuildings))
                    {
                        return true;
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::moveToUnloadArea(spGameAction & pAction, MoveUnitData* pUnitData, spQmlVectorUnit & pUnits, QStringList& actions,
                                std::vector<QVector3D>& targets,
                                spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                spQmlVectorUnit & pEnemyUnits)
{
    AI_CONSOLE_PRINT("NormalAi::moveToUnloadArea()", GameConsole::eDEBUG);
    Unit* pUnit = pUnitData->pUnit.get();
    TargetedUnitPathFindingSystem pfs(m_pMap, pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        auto path = pUnitData->pUnitPfs->getPathFast(targetFields.x(), targetFields.y());
        pAction->setMovepath(path, pUnitData->pUnitPfs->getCosts(path));
        pAction->setActionID(ACTION_UNLOAD);
        if (pAction->canBePerformed() && targetFields == pfs.getTarget())
        {
            return unloadUnits(pAction, pUnit, pEnemyUnits);
        }
        else
        {
            return moveUnit(pAction, pUnitData, pUnits, actions, targets, targets, true, pBuildings, pEnemyBuildings);
        }
    }
    return false;
}

bool NormalAi::unloadUnits(spGameAction & pAction, Unit* pUnit, spQmlVectorUnit & pEnemyUnits)
{
    
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool unloaded = false;
    std::vector<qint32> unloadedUnits;
    do
    {
        unloaded = false;
        spMenuData pDataMenu = pAction->getMenuStepData();
        if (pDataMenu->validData())
        {
            QStringList actions = pDataMenu->getActionIDs();
            QVector<qint32> unitIDx = pDataMenu->getCostList();
            std::vector<QList<QVariant>> unloadFields;
            for (qint32 i = 0; i < unitIDx.size() - 1; i++)
            {
                QString function1 = "getUnloadFields";
                QJSValueList args({pInterpreter->newQObject(pAction.get()),
                                   unitIDx[i],
                                   pInterpreter->newQObject(m_pMap),});
                QJSValue ret = pInterpreter->doFunction(ACTION_UNLOAD, function1, args);
                unloadFields.push_back(ret.toVariant().toList());
            }
            if (actions.size() > 1)
            {
                for (qint32 i = 0; i < unloadFields.size(); i++)
                {
                    Unit* pLoadedUnit = pUnit->getLoadedUnit(i);
                    if (!needsRefuel(pLoadedUnit))
                    {
                        if (!GlobalUtils::contains(unloadedUnits, unitIDx[i]))
                        {
                            if (unloadFields[i].size() == 1)
                            {
                                addMenuItemData(pAction, actions[i], unitIDx[i]);
                                spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
                                addSelectedFieldData(pAction, pFields->getPoints()->at(0));
                                unloaded = true;
                                unloadedUnits.push_back(unitIDx[i]);
                                break;
                            }
                            else if (unloadFields[i].size() > 0 &&
                                     pUnit->getLoadedUnit(i)->getActionList().contains(ACTION_CAPTURE))
                            {
                                auto & fields = unloadFields[i];
                                for (auto & field : fields)
                                {
                                    QPoint unloadField = field.toPoint();
                                    Building* pBuilding = m_pMap->getTerrain(unloadField.x(),
                                                                             unloadField.y())->getBuilding();
                                    if (pBuilding != nullptr && m_pPlayer->isEnemy(pBuilding->getOwner()))
                                    {
                                        addMenuItemData(pAction, actions[i], unitIDx[i]);
                                        addSelectedFieldData(pAction, unloadField);
                                        unloaded = true;
                                        unloadedUnits.push_back(unitIDx[i]);
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
                    qint32 field = 0;
                    qint32 bestDistance = std::numeric_limits<qint32>::max();
                    for (qint32 i = 0; i < pFields->getPoints()->size(); ++i)
                    {
                        QPoint unloadPos = pFields->getPoints()->at(i);
                        qint32 currentBestDistance = std::numeric_limits<qint32>::max();
                        for (auto & pEnemy : pEnemyUnits->getVector())
                        {
                            qint32 distance = GlobalUtils::getDistance(unloadPos, pEnemy->Unit::getPosition());
                            if (distance < currentBestDistance)
                            {
                                currentBestDistance = distance;
                            }
                        }
                        if (currentBestDistance < bestDistance ||
                            (currentBestDistance == bestDistance && GlobalUtils::randInt(0, 1) == 1))
                        {
                            bestDistance = currentBestDistance;
                            field = i;
                        }
                    }
                    addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                }
            }
        }
        else
        {
            AI_CONSOLE_PRINT("Error invalid menu data received while unloading units",  GameConsole::eERROR);
            return false;
        }
    }
    while (unloaded);
    addMenuItemData(pAction, ACTION_WAIT, 0);
    m_updatePoints.push_back(pUnit->getPosition());
    m_updatePoints.push_back(pAction->getActionTarget());
    if (pAction->canBePerformed())
    {
        emit sigPerformAction(pAction);
        return true;
    }
    return false;
}

bool NormalAi::repairUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::repairUnits()", GameConsole::eDEBUG);
    m_aiStep = AISteps::moveUnits;
    for (auto & unitData : m_OwnUnits)
    {
        if (unitData.nextAiStep <= m_aiFunctionStep)
        {
            QCoreApplication::processEvents();
            Unit* pUnit = unitData.pUnit.get();
            ++unitData.nextAiStep;
            // can we use the unit?
            if (!isUsingUnit(pUnit) &&
                !pUnit->getHasMoved() &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                std::vector<QVector3D> targets;
                std::vector<QVector3D> transporterTargets;
                spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                QStringList & actions = unitData.actions;
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                appendRepairTargets(pUnit, pBuildings, targets);
                if (needsRefuel(pUnit))
                {
                    appendTransporterTargets(pUnit, pUnits, transporterTargets);
                    targets.insert(targets.cend(), transporterTargets.cbegin(), transporterTargets.cend());
                }
                if (moveUnit(pAction, &unitData, pUnits, actions, targets, transporterTargets, false, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
                else
                {
                    pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    if (suicide(pAction, pUnit, *unitData.pUnitPfs.get(), unitData.movementPoints))
                    {
                        return true;
                    }
                }
            }
        }
    }
    ++m_aiFunctionStep;
    return false;
}

bool NormalAi::moveUnit(spGameAction & pAction, MoveUnitData* pUnitData, spQmlVectorUnit & pUnits, QStringList& actions,
                        std::vector<QVector3D>& targets, std::vector<QVector3D>& transporterTargets,
                        bool shortenPathForTarget,
                        spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::moveUnit()", GameConsole::eDEBUG);
    Unit* pUnit = pUnitData->pUnit.get();
    TargetedUnitPathFindingSystem pfs(m_pMap, pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnitData->movementPoints;
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        Unit* pTargetUnit = m_pMap->getTerrain(targetFields.x(), targetFields.y())->getUnit();
        UnitPathFindingSystem & turnPfs = *(pUnitData->pUnitPfs.get());
        if (CoreAI::contains(transporterTargets, targetFields) &&
            pTargetUnit != nullptr &&
            (pTargetUnit->getHasMoved() == false || m_usedTransportSystem) &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            auto path = turnPfs.getPathFast(targetFields.x(), targetFields.y());
            pAction->setMovepath(path, turnPfs.getCosts(path));
            pAction->setActionID(ACTION_LOAD);
            if (pAction->canBePerformed())
            {
                m_updatePoints.push_back(pUnit->getPosition());
                m_updatePoints.push_back(pAction->getActionTarget());
                emit sigPerformAction(pAction);
                return true;
            }
        }
        else if (!shortenPathForTarget && CoreAI::contains(targets, targetFields))
        {
            auto movePath = turnPfs.getClosestReachableMovePath(targetFields, pUnitData->movementPoints);
            pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
            pAction->setActionID(ACTION_WAIT);
            if (pAction->canBePerformed())
            {
                m_updatePoints.push_back(pUnit->getPosition());
                m_updatePoints.push_back(pAction->getActionTarget());
                emit sigPerformAction(pAction);
                return true;
            }
        }
        else
        {
            auto movePath = turnPfs.getClosestReachableMovePath(targetFields, pUnitData->movementPoints);
            if (movePath.size() == 0)
            {
                movePath.push_back(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            }
            qint32 idx = getMoveTargetField(*pUnitData, turnPfs, movePath, pBuildings, pEnemyBuildings, pUnitData->movementPoints);
            if (idx < 0 || idx == movePath.size() - 1)
            {
                std::tuple<QPoint, float, bool> target = moveToSafety(*pUnitData, turnPfs, movePath[0], pBuildings, pEnemyBuildings, pUnitData->movementPoints);
                QPoint ret = std::get<0>(target);
                float minDamage = std::get<1>(target);
                bool allEqual = std::get<2>(target);
                if (((ret.x() == pUnit->Unit::getX() && ret.y() == pUnit->Unit::getY()) ||
                     minDamage > pUnit->getCoUnitValue() / 2 ||
                     allEqual) && minDamage > 0.0f)
                {
                    if (suicide(pAction, pUnit, turnPfs, pUnitData->movementPoints))
                    {
                        return true;
                    }
                    else
                    {
                        auto movePath = turnPfs.getPathFast(ret.x(), ret.y());
                        pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
                    }
                }
                else
                {
                    auto movePath = turnPfs.getPathFast(ret.x(), ret.y());
                    pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
                }
            }
            else
            {
                auto path = turnPfs.getPathFast(movePath[idx].x(), movePath[idx].y());
                pAction->setMovepath(path, turnPfs.getCosts(path));
            }
            bool lockedUnit = (pAction->getMovePath().size() == 1) &&
                              (pUnit->getHp() < m_lockedUnitHp);
            // when we don't move try to attack if possible
            if ((pUnit->getHp() > m_noMoveAttackHp) ||
                lockedUnit)
            {
                pAction->setActionID(ACTION_FIRE);
                std::vector<QVector3D> ret;
                std::vector<QVector3D> moveTargetFields;
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
                        m_updatePoints.push_back(pUnit->getPosition());
                        m_updatePoints.push_back(pAction->getActionTarget());
                        m_updatePoints.push_back(QPoint(static_cast<qint32>(target.x()),
                                                        static_cast<qint32>(target.y())));
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
            }
            if (pAction->getMovePath().size() > 0)
            {
                m_updatePoints.push_back(pUnit->getPosition());
                m_updatePoints.push_back(pAction->getActionTarget());
                for (const auto & action : actions)
                {
                    if (action.startsWith(ACTION_SUPPORTALL))
                    {
                        pAction->setActionID(action);
                        if (pAction->canBePerformed())
                        {
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                }
                if (actions.contains(ACTION_STEALTH))
                {
                    pAction->setActionID(ACTION_STEALTH);
                    if (pAction->canBePerformed())
                    {
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
                if (actions.contains(ACTION_UNSTEALTH))
                {
                    pAction->setActionID(ACTION_UNSTEALTH);
                    if (pAction->canBePerformed())
                    {
                        float counterDamage = calculateCounterDamage(*pUnitData, pAction->getActionTarget(), nullptr, 0, pBuildings, pEnemyBuildings, true);
                        if (counterDamage <= 0)
                        {
                            emit sigPerformAction(pAction);
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
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                }
                if (pUnit->canMoveAndFire(pAction->getActionTarget()) ||
                    pUnit->getPosition() == pAction->getActionTarget())
                {
                    pAction->setActionID(ACTION_FIRE);
                    // if we run away and still find a target we should attack it
                    std::vector<QVector3D> moveTargets(1, QVector3D(pAction->getActionTarget().x(),
                                                                    pAction->getActionTarget().y(), 1));
                    std::vector<QVector3D> ret;
                    getBestAttacksFromField(pUnit, pAction, ret, moveTargets);
                    if (ret.size() > 0 && ret[0].z() >= -pUnit->getCoUnitValue()  * m_minSuicideDamage)
                    {
                        qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
                        QVector3D target = ret[selection];
                        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                     static_cast<qint32>(target.y())));
                        if (pAction->isFinalStep() && pAction->canBePerformed())
                        {
                            m_updatePoints.push_back(pUnit->getPosition());
                            m_updatePoints.push_back(pAction->getActionTarget());
                            m_updatePoints.push_back(QPoint(static_cast<qint32>(target.x()),
                                                            static_cast<qint32>(target.y())));
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                }
                pAction->setActionID(ACTION_WAIT);
                if (pAction->canBePerformed())
                {
                    emit sigPerformAction(pAction);
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

bool NormalAi::suicide(spGameAction & pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs, qint32 movepoints)
{
    AI_CONSOLE_PRINT("NormalAi::suicide", GameConsole::eDEBUG);
    // we don't have a good option do the best that we can attack with an all in attack :D
    pAction->setActionID(ACTION_FIRE);
    std::vector<QVector3D> ret;
    std::vector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &turnPfs, ret, moveTargetFields, movepoints + 1);
    if (ret.size() > 0 && ret[0].z() >= -pUnit->getCoUnitValue() * m_minSuicideDamage)
    {
        qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
        QVector3D target = ret[selection];
        auto path = turnPfs.getPathFast(static_cast<qint32>(moveTargetFields[selection].x()),
                                        static_cast<qint32>(moveTargetFields[selection].y()));
        pAction->setMovepath(path, turnPfs.getCosts(path));
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep() && pAction->canBePerformed())
        {
            m_updatePoints.push_back(pUnit->getPosition());
            m_updatePoints.push_back(pAction->getActionTarget());
            m_updatePoints.push_back(QPoint(static_cast<qint32>(target.x()),
                                            static_cast<qint32>(target.y())));
            emit sigPerformAction(pAction);
            return true;
        }
    }
    return false;
}

std::tuple<QPoint, float, bool> NormalAi::moveToSafety(MoveUnitData & unitData, UnitPathFindingSystem& turnPfs, QPoint target,
                                                       spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                                       qint32 movePoints)
{
    AI_CONSOLE_PRINT("NormalAi::moveToSafety", GameConsole::eDEBUG);
    Unit* pUnit = unitData.pUnit.get();
    auto targets = turnPfs.getAllNodePointsFast(movePoints + 1);
    QPoint ret(pUnit->Unit::getX(), pUnit->Unit::getY());
    float leastDamageField = std::numeric_limits<float>::max();
    qint32 shortestDistance = std::numeric_limits<qint32>::max();
    bool allFieldsEqual = true;
    for (auto & moveTarget : targets)
    {
        qint32 x = moveTarget.x();
        qint32 y = moveTarget.y();
        if (m_pMap->getTerrain(x, y)->getUnit() == nullptr &&
            turnPfs.getCosts(turnPfs.getIndex(x, y), x, y, x, y, 0) > 0)
        {
            float currentDamage = calculateCounterDamage(unitData, moveTarget, nullptr, 0.0f, pBuildings, pEnemyBuildings, true);
            if (currentDamage < 0)
            {
                currentDamage = 0;
            }
            if (leastDamageField < std::numeric_limits<float>::max() &&
                static_cast<qint32>(leastDamageField) != static_cast<qint32>(currentDamage))
            {
                allFieldsEqual = false;
            }
            qint32 distance = GlobalUtils::getDistance(target, moveTarget);
            if (currentDamage < leastDamageField)
            {
                ret = moveTarget;
                leastDamageField = currentDamage;
                shortestDistance = distance;
            }
            else if (static_cast<qint32>(currentDamage) == static_cast<qint32>(leastDamageField) &&
                     distance < shortestDistance &&
                     distance > 0)
            {
                ret = moveTarget;
                leastDamageField = currentDamage;
                shortestDistance = distance;
            }
        }
    }
    return std::tuple<QPoint, float, bool>(ret, leastDamageField, allFieldsEqual);
}

qint32 NormalAi::getMoveTargetField(MoveUnitData & unitData, UnitPathFindingSystem& turnPfs,
                                    std::vector<QPoint>& movePath, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                    qint32 movePoints)
{
    const float minDamage = unitData.unitCosts * m_minMovementDamage;
    float bestMinDamge = minDamage;
    qint32 bestIdx = -1;
    qint32 movePathSize = movePath.size() - 1;
    for (qint32 i = 0; i <= movePathSize; i++)
    {
        // empty or own field
        qint32 x = movePath[i].x();
        qint32 y = movePath[i].y();
        Terrain* pTerrain = m_pMap->getTerrain(x, y);
        Building* pBuilding = pTerrain->getBuilding();
        qint32 costs = turnPfs.getCosts(turnPfs.getIndex(x, y), x, y, x, y, 0);
        if ((pTerrain->getUnit() == nullptr ||
             pTerrain->getUnit() == unitData.pUnit.get()) &&
            costs >= 0 &&
            costs <= movePoints)
        {
            if (isMoveableTile(pBuilding))
            {
                float counterDamage = calculateCounterDamage(unitData, movePath[i], nullptr, 0.0f, pBuildings, pEnemyBuildings, true);
                if (counterDamage <= bestMinDamge &&
                    (i != movePathSize || counterDamage <= 0))
                {
                    bestIdx = i;
                    if (counterDamage <= 0 )
                    {
                        return bestIdx;
                    }
                    bestMinDamge = counterDamage;
                }
            }
        }
    }
    return bestIdx;
}

qint32 NormalAi::getBestAttackTarget(MoveUnitData & unitData, std::vector<CoreAI::DamageData>& ret,
                                     std::vector<QVector3D>& moveTargetFields,
                                     spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{    
    qint32 target = -1;
    qint32 currentDamage = std::numeric_limits<qint32>::min();
    qint32 deffense = 0;
    Unit* pUnit = unitData.pUnit.get();

    float minFundsDamage = -unitData.unitCosts * m_minAttackFunds;

    for (qint32 i = 0; i < ret.size(); i++)
    {
        QPoint moveTarget(static_cast<qint32>(moveTargetFields[i].x()), static_cast<qint32>(moveTargetFields[i].y()));
        Unit* pEnemy = m_pMap->getTerrain(static_cast<qint32>(ret[i].x), static_cast<qint32>(ret[i].y))->getUnit();

        qint32 minfireRange = pUnit->getMinRange(moveTarget);
        qint32 fundsDamage = 0;
        float bonusDamage = 0.0f;
        if (pEnemy != nullptr)
        {
            float currentHp = pEnemy->getHp();
            float newHp = currentHp - static_cast<float>(ret[i].hpDamage);
            auto captureBonus = calculateCaptureBonus(pEnemy, newHp);
            fundsDamage = static_cast<qint32>(ret[i].fundsDamage * captureBonus);
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
            if (!isMoveableTile(m_pMap->getTerrain(moveTarget.x(), moveTarget.y())->getBuilding()))
            {
                fundsDamage -= m_ownProdctionMalus;
            }
        }
        else
        {
            fundsDamage = static_cast<qint32>(ret[i].fundsDamage);
        }
        float counterDamage = calculateCounterDamage(unitData, moveTarget, pEnemy, ret[i].hpDamage + bonusDamage, pBuildings, pEnemyBuildings, true);
        if (counterDamage < 0)
        {
            counterDamage = 0;
        }
        fundsDamage -= counterDamage;
        Terrain* pTerrain = m_pMap->getTerrain(static_cast<qint32>(ret[i].x), static_cast<qint32>(ret[i].y));
        qint32 targetDefense = pTerrain->getDefense(pUnit);
        if (fundsDamage >= minFundsDamage &&
            ret[i].hpDamageDifference >= m_minHpDamage)
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
    float supportDamage = 0;
    hpDamage = 0;
    for (auto & pUnitData : m_OwnUnits)
    {
        if (pUnitData.pUnit != pUnit &&
            !pUnitData.pUnit->getHasMoved() &&
            pUnitData.pUnit->hasWeapons())
        {
            auto position = pUnitData.pUnit->getPosition();
            qint32 movepoints = pUnitData.movementPoints;
            float minFundsDamage = -pUnitData.pUnit->getCoUnitValue() * m_minAttackFunds;
            if (GlobalUtils::getDistance(moveTarget, position) <= movepoints)
            {
                std::vector<CoreAI::DamageData> ret;
                spQmlVectorPoint firerange;
                firerange = spQmlVectorPoint(GlobalUtils::getCircle(pUnitData.minFireRange, pUnitData.maxFireRange));
                CoreAI::getAttackTargetsFast(pUnitData.pUnit.get(), *firerange.get(), pUnitData.pUnitPfs.get(), ret);

                std::vector<Unit*> pUsedUnits;
                float newFundsDamage = std::numeric_limits<float>::lowest();
                float newHpDamage = std::numeric_limits<float>::lowest();
                for (auto & data : ret)
                {
                    QPoint newMoveTarget(static_cast<qint32>(data.x), static_cast<qint32>(data.y));
                    Unit* pNewEnemy = m_pMap->getTerrain(static_cast<qint32>(data.x), static_cast<qint32>(data.y))->getUnit();
                    if (pNewEnemy == pEnemy &&
                        moveTarget != newMoveTarget &&
                        pNewEnemy != nullptr)
                    {
                        float newHp = pEnemy->getHp() - static_cast<float>(data.hpDamage);
                        qint32 fundsDamage = static_cast<qint32>(data.fundsDamage * calculateCaptureBonus(pEnemy, newHp));
                        if (fundsDamage > minFundsDamage && fundsDamage > newFundsDamage)
                        {
                            newFundsDamage = fundsDamage;
                            newHpDamage = data.hpDamage;
                        }
                    }
                    else if (pNewEnemy != nullptr &&
                             pNewEnemy != pEnemy &&
                             !GlobalUtils::contains(pUsedUnits, pNewEnemy))
                    {
                        if (pNewEnemy->getCoUnitValue() >= m_cheapUnitValue)
                        {
                            pUsedUnits.push_back(pNewEnemy);
                        }
                    }
                }
                if (newFundsDamage > minFundsDamage)
                {
                    supportDamage += static_cast<float>(newFundsDamage) / static_cast<float>(pUsedUnits.size() + 1) * m_supportDamageBonus;
                    hpDamage += static_cast<float>(newHpDamage) / static_cast<float>(pUsedUnits.size() + 1) * m_supportDamageBonus;
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
        qint32 remainingDays = GlobalUtils::roundUp(static_cast<float>(restCapture) / static_cast<float>(currentHp));
        if (remainingDays <= 1)
        {
            if (newHp <= 0)
            {
                ret = m_antiCaptureBonus;
            }
            else
            {
                qint32 newRemainingDays = GlobalUtils::roundUp(static_cast<float>(restCapture) / static_cast<float>(newHp));
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
    }
    if (pBuilding != nullptr &&
        pBuilding->getOwner() == m_pPlayer &&
        pBuilding->getBuildingID() == CoreAI::BUILDING_HQ &&
        pUnit->hasAction(ACTION_CAPTURE))
    {
        ret *= m_antiCaptureHqBonus;
    }
    return ret;
}

float NormalAi::calculateCounterDamage(MoveUnitData & curUnitData, QPoint newPosition,
                                       Unit* pEnemyUnit, float enemyTakenDamage,
                                       spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings,
                                       bool ignoreOutOfVisionRange)
{
    AI_CONSOLE_PRINT("NormalAi calculateCounterDamage", GameConsole::eDEBUG);
    QCoreApplication::processEvents();
    Unit* pUnit = curUnitData.pUnit.get();
    std::map<QString, qint32> unitDamageData;
    float counterDamage = 0;
    for (auto & enemyData : m_EnemyUnits)
    {
        spUnit pNextEnemy = enemyData.pUnit;
        if (pNextEnemy->getHp() > 0 && pNextEnemy->getTerrain() != nullptr)
        {
            QPoint enemyPos = QPoint(pNextEnemy->Unit::getX(), pNextEnemy->Unit::getY());
            qint32 distance = GlobalUtils::getDistance(newPosition, enemyPos);
            qint32 maxFireRange = enemyData.maxFireRange;
            bool hasDamage = unitDamageData.contains(pNextEnemy->getUnitID());
            float unitDamage = -1;
            if (hasDamage)
            {
                unitDamage = unitDamageData[pNextEnemy->getUnitID()];
            }
            qint32 moveRange = 0;
            bool canMoveAndFire = false;
            if (distance <= enemyData.movementPoints + maxFireRange)
            {
                canMoveAndFire = pNextEnemy->canMoveAndFire(enemyPos);
                if (canMoveAndFire)
                {
                    moveRange = enemyData.movementPoints;
                }
            }
            if (distance <= moveRange + maxFireRange &&
                (unitDamage >= 0 ||
                 (!hasDamage && pNextEnemy->isAttackable(pUnit, true))))
            {
                qint32 minFireRange = enemyData.minFireRange;
                float enemyDamage = enemyData.virtualDamageData;
                if (pNextEnemy.get() == pEnemyUnit)
                {
                    enemyDamage += enemyTakenDamage;
                }
                enemyDamage *= m_enemyCounterDamageMultiplier;
                if (enemyDamage < pNextEnemy->getHp() * Unit::MAX_UNIT_HP)
                {
                    QRectF damageData;
                    if (distance >= minFireRange && distance <= maxFireRange)
                    {
                        // indirect attack
                        if (hasDamage)
                        {
                            damageData.setX(unitDamage * pNextEnemy->getHp() / Unit::MAX_UNIT_HP);
                        }
                        else
                        {
                            damageData = CoreAI::calcVirtuelUnitDamage(m_pMap, pNextEnemy.get(), enemyDamage, enemyPos, GameEnums::LuckDamageMode_Average,
                                                                       pUnit, 0, newPosition, GameEnums::LuckDamageMode_Average,
                                                                       ignoreOutOfVisionRange);
                            if (damageData.x() >= 0)
                            {
                                unitDamageData.insert_or_assign(pNextEnemy->getUnitID(), damageData.x() * Unit::MAX_UNIT_HP / pNextEnemy->getHp());
                            }
                        }
                        if (damageData.x() >= m_notAttackableDamage)
                        {
                            for (auto & unitData : m_OwnUnits)
                            {
                                distance = GlobalUtils::getDistance(QPoint(unitData.pUnit->Unit::getX(), unitData.pUnit->Unit::getY()), enemyPos);
                                if (distance >= minFireRange && distance <= maxFireRange &&
                                    pNextEnemy->isAttackable(unitData.pUnit.get(), true))
                                {
                                    if (unitData.unitCosts > 0 && curUnitData.unitCosts > 0)
                                    {
                                        if (curUnitData.unitCosts > unitData.unitCosts)
                                        {
                                            // reduce damage the more units it can attack
                                            damageData.moveLeft(damageData.x() - damageData.x() * 0.5f * unitData.unitCosts / curUnitData.unitCosts);
                                        }
                                        else
                                        {
                                            damageData.moveLeft(damageData.x() - damageData.x() * 0.5f *  curUnitData.unitCosts / unitData.unitCosts);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if (canMoveAndFire)
                    {
                        auto targets = enemyData.pUnitPfs->getAllNodePointsFast(enemyData.movementPoints + 1);
                        bool found = false;
                        for (auto & target : targets)
                        {
                            distance = GlobalUtils::getDistance(newPosition, target);
                            Unit* pTerrainUnit = m_pMap->getTerrain(target.x(), target.y())->getUnit();
                            if (distance >= minFireRange && distance <= maxFireRange &&
                                (pTerrainUnit == nullptr ||
                                 pTerrainUnit == pNextEnemy))
                            {
                                if (hasDamage)
                                {
                                    damageData.setX(unitDamage * pNextEnemy->getHp() / Unit::MAX_UNIT_HP);
                                }
                                else
                                {
                                    damageData = CoreAI::calcVirtuelUnitDamage(m_pMap, pNextEnemy.get(), enemyDamage, target, GameEnums::LuckDamageMode_Average,
                                                                               pUnit, 0, newPosition, GameEnums::LuckDamageMode_Average,
                                                                               ignoreOutOfVisionRange);
                                    if (damageData.x() >= 0)
                                    {
                                        unitDamageData.insert_or_assign(pNextEnemy->getUnitID(), damageData.x() * Unit::MAX_UNIT_HP / pNextEnemy->getHp());
                                    }

                                }
                                found = true;
                                break;
                            }
                        }
                        qint32 enemyIslandIdx = getIslandIndex(pNextEnemy.get());
                        qint32 enemyIsland = getIsland(pNextEnemy.get());
                        if (found &&
                            damageData.x() >= m_notAttackableDamage)
                        {
                            for (auto & unitData : m_OwnUnits)
                            {
                                for (auto & target : targets)
                                {
                                    Unit* pNextUnit = unitData.pUnit.get();
                                    distance = GlobalUtils::getDistance(QPoint(pNextUnit->Unit::getX(), pNextUnit->Unit::getY()), target);
                                    Unit* pTargetUnit = m_pMap->getTerrain(target.x(), target.y())->getUnit();
                                    if (distance >= minFireRange && distance <= maxFireRange &&
                                        (pTargetUnit == nullptr ||
                                         pTargetUnit->getOwner()->isAlly(m_pPlayer)) &&
                                        enemyIsland == m_IslandMaps[enemyIslandIdx]->getIsland(target.x(), target.y()))
                                    {
                                        if (unitData.unitCosts > 0 && curUnitData.unitCosts > 0)
                                        {
                                            if (curUnitData.unitCosts > unitData.unitCosts)
                                            {
                                                // reduce damage the more units it can attack
                                                damageData.moveLeft(damageData.x() - damageData.x() * 0.5f * unitData.unitCosts / curUnitData.unitCosts);
                                            }
                                            else
                                            {
                                                damageData.moveLeft(damageData.x() - damageData.x() * 0.5f *  curUnitData.unitCosts / unitData.unitCosts);
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                            if (damageData.x() < 0)
                            {
                                damageData.moveLeft(0);
                            }
                            if (damageData.x() > 0)
                            {
                                counterDamage += static_cast<qint32>(calcFundsDamage(damageData, pNextEnemy.get(), pUnit).fundsDamage);
                            }
                        }
                    }
                }
            }
        }
    }
    float buildingCounterDamage = calculateCounteBuildingDamage(pUnit, newPosition, pBuildings, pEnemyBuildings);
    float influenceCounterDamage = getMapInfluenceModifier(pUnit, newPosition.x(), newPosition.y());
    float totalCounterDamage = counterDamage + influenceCounterDamage + buildingCounterDamage;
    AI_CONSOLE_PRINT("NormalAi counter damage at x=" + QString::number(newPosition.x()) + " y=" + QString::number(newPosition.y()) +
                     " total score=" + QString::number(totalCounterDamage) +
                     " counter damage=" + QString::number(counterDamage) +
                     " influence damage=" + QString::number(influenceCounterDamage) +
                     " building damage=" + QString::number(buildingCounterDamage), GameConsole::eDEBUG);
    return totalCounterDamage;
}

float NormalAi::calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings) const
{
    float counterDamage = 0.0f;
    for (auto & pBuilding : pEnemyBuildings->getVector())
    {
        counterDamage += calcBuildingDamage(pUnit, newPosition, pBuilding.get());
    }
    for (auto & pBuilding : pEnemyBuildings->getVector())
    {
        counterDamage += calcBuildingDamage(pUnit, newPosition, pBuilding.get());
    }
    spQmlVectorPoint pCircle = spQmlVectorPoint(GlobalUtils::getCircle(1, 2));
    
    for (auto & circlePos : pCircle->getVector())
    {
        QPoint pos = newPosition + circlePos;
        if (m_pMap->onMap(pos.x(), pos.y()))
        {
            Unit* pMine = m_pMap->getTerrain(pos.x(), pos.y())->getUnit();
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
    AI_CONSOLE_PRINT("NormalAi::updateAllUnitData()", GameConsole::eDEBUG);
    bool initial = m_EnemyUnits.size() == 0;
    spQmlVectorUnit enemyUnits = spQmlVectorUnit(m_pPlayer->getEnemyUnits());
    enemyUnits->pruneEnemies(pUnits.get(), m_enemyPruneRange);
    rebuildIsland(pUnits);
    rebuildIsland(enemyUnits);

    updateUnitData(pUnits, m_OwnUnits, false, m_EnemyUnits);
    updateUnitData(enemyUnits, m_EnemyUnits, true, m_OwnUnits);
    sortUnitsFarFromEnemyFirst(m_OwnUnits, enemyUnits);
    if (initial)
    {
        calcVirtualDamage();
    }
    m_updatePoints.clear();

    if (initial)
    {
        createUnitInfluenceMap();
    }
}


void NormalAi::createUnitInfluenceMap()
{
    // create influence map at the start of the turn
    m_InfluenceFrontMap.clear();
    m_InfluenceFrontMap.setOwner(m_pPlayer);
    m_InfluenceFrontMap.addBuildingInfluence();
    for (auto & unit : m_OwnUnits)
    {
        QCoreApplication::processEvents();
        m_InfluenceFrontMap.addUnitInfluence(unit.pUnit.get(), unit.pUnitPfs.get(), unit.movementPoints);
    }
    for (auto & unit : m_EnemyUnits)
    {
        QCoreApplication::processEvents();
        m_InfluenceFrontMap.addUnitInfluence(unit.pUnit.get(), unit.pUnitPfs.get(), unit.movementPoints);
    }
    m_InfluenceFrontMap.updateOwners();
    m_InfluenceFrontMap.calculateGlobalData();
}

void NormalAi::updateUnitData(spQmlVectorUnit & pUnits, std::vector<MoveUnitData> & pUnitData, bool enemy, std::vector<MoveUnitData> & otherUnitData)
{
    AI_CONSOLE_PRINT("NormalAi::updateEnemyData", GameConsole::eDEBUG);
    if (pUnitData.size() == 0)
    {
        pUnitData.reserve(pUnits->size());
        for (auto & pUnit : pUnits->getVector())
        {
            QCoreApplication::processEvents();
            MoveUnitData data;
            createUnitData(pUnit.get(), data, enemy, m_influenceUnitRange, otherUnitData, !enemy);
            pUnitData.push_back(data);
        }
    }
    else
    {
        qint32 i = 0;
        while (i < pUnitData.size())
        {
            if (pUnitData[i].pUnit->getHp() <= 0 ||
                pUnitData[i].pUnit->getTerrain() == nullptr)
            {
                pUnitData.erase(pUnitData.cbegin() + i);
            }
            else
            {
                i++;
            }
        }
    }
    if (!enemy && m_aiStep >= AISteps::moveTransporters)
    {
        pUnitData.reserve(pUnits->size());
        for (auto & pUnit : pUnits->getVector())
        {
            QCoreApplication::processEvents();
            bool found = false;
            for (auto & unitData : pUnitData)
            {
                if (unitData.pUnit == pUnit)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                MoveUnitData data;
                createUnitData(pUnit.get(), data, enemy, m_influenceUnitRange, otherUnitData, !enemy);
                pUnitData.push_back(data);
            }
        }
    }
    std::vector<qint32> updated;
    for (auto point : m_updatePoints)
    {
        for (qint32 i2 = 0; i2 < pUnitData.size(); i2++)
        {
            if (!GlobalUtils::contains(updated, i2))
            {
                QCoreApplication::processEvents();
                auto & unitData = pUnitData[i2];
                Unit* pUnit = unitData.pUnit.get();
                if (pUnit != nullptr &&
                    pUnit->getHp() > 0 &&
                    pUnit->getTerrain() != nullptr)
                {
                    if (qAbs(point.x() - pUnit->Unit::getX()) +
                        qAbs(point.y() - pUnit->Unit::getY()) <=
                        unitData.movementPoints * m_influenceUnitRange + 2)
                    {
                        createUnitData(pUnit, unitData, enemy, m_influenceUnitRange + 1, otherUnitData, true);
                    }
                    updated.push_back(i2);
                }
            }
        }
    }
}

void NormalAi::createUnitData(Unit* pUnit, MoveUnitData & data, bool enemy, double moveMultiplier, std::vector<MoveUnitData> & otherUnitData, bool always)
{
    QPoint pos = pUnit->Unit::getPosition();
    data.pUnitPfs = spUnitPathFindingSystem::create(m_pMap, pUnit);
    data.movementPoints = pUnit->getMovementpoints(pos);
    data.maxFireRange = pUnit->getMaxRange(pos);
    data.pUnit = spUnit(pUnit);
    data.minFireRange = pUnit->getMinRange(pos);
    data.unitCosts = pUnit->getCoUnitValue();
    data.nextAiStep = m_aiFunctionStep;
    bool valid = false;
    if (!always)
    {
        auto range = data.movementPoints + data.maxFireRange;
        for (auto & otherUnit : otherUnitData)
        {
            if (GlobalUtils::getDistance(pos, otherUnit.pUnit->getPosition()) <= range + otherUnit.movementPoints + otherUnit.maxFireRange + 1)
            {
                valid = true;
                break;
            }
        }
    }
    if (valid || always)
    {
        if (enemy)
        {
            data.pUnitPfs->setIgnoreEnemies(UnitPathFindingSystem::CollisionIgnore::OnlyNotMovedEnemies);
        }
        else
        {
            data.actions = pUnit->getActionList();
        }
        if (pUnit->getHasMoved())
        {
            data.pUnitPfs->setMovepoints(moveMultiplier * data.movementPoints - 1);
        }
        else
        {
            data.pUnitPfs->setMovepoints(moveMultiplier * data.movementPoints);
        }
        data.pUnitPfs->explore();
    }
}

void NormalAi::calcVirtualDamage()
{
    for (auto & ownUnit : m_OwnUnits)
    {
        Unit* pUnit = ownUnit.pUnit.get();
        struct DamagePosInfo
        {
            QPoint pos;
            float damage{0};
        };

        std::vector<DamagePosInfo> attacks;
        if (isUsingUnit(pUnit))
        {
            static constexpr float maxDistance = 2;
            spGameAction action = spGameAction::create(ACTION_FIRE, m_pMap);
            action->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            std::vector<CoreAI::DamageData> ret;
            std::vector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, action, ownUnit.pUnitPfs.get(), ret, moveTargetFields, ownUnit.movementPoints * maxDistance + 1);
            QPoint ownPos = pUnit->Unit::getPosition();
            for (auto & damageData : ret)
            {
                QPoint pos(static_cast<qint32>(damageData.x), static_cast<qint32>(damageData.y));
                bool containsPos = false;
                for (auto & attack : attacks)
                {
                    if (attack.pos.x() == pos.x() &&
                        attack.pos.y() == pos.y())
                    {
                        containsPos = true;
                        break;
                    }
                }

                if (!containsPos)
                {
                    float damage = 0;

                    if (GlobalUtils::getDistance(ownPos, pos) > ownUnit.movementPoints)
                    {
                        qint32 dis = ownUnit.movementPoints / maxDistance + 1;
                        damage = (damageData.hpDamage / dis);
                    }
                    else
                    {
                        damage = (damageData.hpDamage);
                    }
                    attacks.push_back(DamagePosInfo{pos, damage});
                }
            }
        }
        float attacksSize = attacks.size();
        for (auto & attack : attacks)
        {
            for (auto & enemyData : m_EnemyUnits)
            {
                if (enemyData.pUnit->Unit::getX() == attack.pos.x() &&
                    enemyData.pUnit->Unit::getY() == attack.pos.y())
                {
                    enemyData.virtualDamageData += (m_enemyUnitCountDamageReductionMultiplier * attack.damage / attacksSize);
                    break;
                }
            }
        }
    }
}

float NormalAi::getMapInfluenceModifier(Unit* pUnit, qint32 x, qint32 y) const
{
    const auto * info = m_InfluenceFrontMap.getInfluenceInfo(x, y);
    float enemyInfluence = info->getEnemyInfluence();
    float ownInfluence = info->getOwnInfluence();
    float influence = 0.0f;
    float influenceDamage = 0.0f;
    if (enemyInfluence > ownInfluence && ownInfluence > 0)
    {
        influence = enemyInfluence / ownInfluence - 1.0f;
    }
    else if (enemyInfluence > 0)
    {
        influence = -ownInfluence / enemyInfluence + 1.0f;
    }
    if (qAbs(influence) > m_influenceIgnoreValue)
    {
        influenceDamage = influence * pUnit->getCoUnitValue() * m_influenceMultiplier;
    }
    return influenceDamage;
}

void NormalAi::clearUnitData()
{
    m_EnemyUnits.clear();
    m_OwnUnits.clear();
    m_usedPredefinedAi = false;
}

bool NormalAi::buildUnits(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits,
                          spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("NormalAi::buildUnits()", GameConsole::eDEBUG);
    pEnemyUnits->pruneEnemies(pUnits.get(), m_enemyPruneRange);
    pBuildings->sortClosestToEnemy(pEnemyUnits);
    if (m_aiStep < AISteps::buildUnits)
    {
        m_productionData.clear();
        m_productionSystem.onNewBuildQueue(pBuildings.get(), pUnits.get(), pEnemyUnits.get(), pEnemyBuildings.get());
    }
    m_aiStep = AISteps::buildUnits;
    bool executed = false;
    if (m_productionSystem.buildUnit(pBuildings.get(), pUnits.get(), pEnemyUnits.get(), pEnemyBuildings.get(), executed))
    {
        return executed;
    }

    qint32 enemeyCount = 0;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        if (m_pPlayer->isEnemy(m_pMap->getPlayer(i)) && !m_pMap->getPlayer(i)->getIsDefeated())
        {
            enemeyCount++;
        }
    }
    float funds = m_pPlayer->getFunds();
    std::vector<float> data(BuildItems::Max, 0);
    float productionBuildings = 0;
    for (auto & pBuilding : pBuildings->getVector())
    {
        if (pBuilding->isProductionBuilding() &&
            pBuilding->getTerrain()->getUnit() == nullptr)
        {
            auto buildList = pBuilding->getConstructionList();
            for (auto & unitId : buildList)
            {
                Unit dummy(unitId, m_pPlayer, false, m_pMap);
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
    GetOwnUnitCounts(m_OwnUnits, pUnits, pEnemyUnits, pEnemyBuildings, countData);
    std::vector<NotAttackableData> attackCount(pEnemyUnits->size(), NotAttackableData());
    getEnemyDamageCounts(pUnits, pEnemyUnits, attackCount);
    data[InfantryUnitRatio] = static_cast<float>(countData.infantryUnits) / static_cast<float>(pUnits->size() + 1);
    data[SupplyRatio] = static_cast<float>(countData.supplyUnits) / static_cast<float>(pUnits->size() + 1);
    data[RequiredSupplyRatio] = static_cast<float>(countData.supplyNeededUnits) / static_cast<float>(pUnits->size() + 1);
    data[InfantryCount] = countData.infantryUnits;
    m_currentDirectIndirectRatio = m_directIndirectRatio * getAiCoBuildRatioModifier();
    if (productionBuildings > m_maxProductionBuildings)
    {
        productionBuildings = m_maxProductionBuildings;
    }
    // calc average costs if we would build same cost units on every building
    float fundsPerFactory = funds - m_cappedFunds * (productionBuildings - 1) * m_fundsPerBuildingFactorD;
    AI_CONSOLE_PRINT("NormalAI: Funds: " + QString::number(funds) + " funds for the next factory: " + QString::number(fundsPerFactory), GameConsole::eDEBUG);
    auto chance = GlobalUtils::randInt(0, 100);
    if (funds >= m_spamingFunds && productionBuildings > 1 && chance <= m_spamLightUnitChance)
    {
        fundsPerFactory = m_spamingFunds;
    }
    else if (funds >= m_spamingFunds * m_fundsPerBuildingFactorA && productionBuildings > 1 && chance < m_spamLightUnitChance + m_spamMediumUnitChance)
    {
        AI_CONSOLE_PRINT("NormalAI: Building expensive units", GameConsole::eDEBUG);
        fundsPerFactory = m_spamingFunds * m_fundsPerBuildingFactorA;
        data[UseHighTechUnits] = FundsMode::Expensive;
    }
    else if (fundsPerFactory <= m_cappingFunds)
    {
        if (funds >= m_spamingFunds && chance > m_spamInfantryChance)
        {
            fundsPerFactory = m_spamingFunds;
        }
        else
        {
            data[LowFunds] = 1.0;
            if (fundsPerFactory > m_cappedFunds * m_fundsPerBuildingFactorB && productionBuildings <= m_maxProductionBuildingsForB)
            {
                fundsPerFactory = m_cappedFunds * m_fundsPerBuildingFactorB;
            }
            else
            {
                fundsPerFactory = m_cappedFunds;
            }
        }
    }
    else if (fundsPerFactory < m_spamingFunds * m_fundsPerBuildingFactorA)
    {
        fundsPerFactory = m_spamingFunds;
    }
    else if (fundsPerFactory >= m_spamingFunds * m_fundsPerBuildingFactorC)
    {
        AI_CONSOLE_PRINT("NormalAI: Building very expensive units", GameConsole::eDEBUG);
        fundsPerFactory = m_spamingFunds * m_fundsPerBuildingFactorC;
        data[UseHighTechUnits] = FundsMode::VeryExpensive;
    }
    else if (fundsPerFactory >= m_spamingFunds * m_fundsPerBuildingFactorA)
    {
        if (fundsPerFactory >= m_spamingFunds * m_fundsPerBuildingFactorA + m_spamingFunds)
        {
            AI_CONSOLE_PRINT("NormalAI: Building expensive units with no spend malus", GameConsole::eDEBUG);
            data[UseHighTechUnits] = NoSpendMalus;
        }
        else
        {
            AI_CONSOLE_PRINT("NormalAI: Building expensive units", GameConsole::eDEBUG);
            data[UseHighTechUnits] = FundsMode::Expensive;
        }
        fundsPerFactory = m_spamingFunds * m_fundsPerBuildingFactorA;
    }
    AI_CONSOLE_PRINT("NormalAI: fundsPerFactory=" + QString::number(fundsPerFactory), GameConsole::eDEBUG);
    data[DirectUnitRatio] = static_cast<float>(countData.directUnits) / static_cast<float>(countData.indirectUnits + 1);

    data[UnitEnemyRatio] = (static_cast<float>(pUnits->size()) + m_ownUnitEnemyUnitRatioAverager) / (static_cast<float>(pEnemyUnits->size()) + m_ownUnitEnemyUnitRatioAverager);
    if (enemeyCount > 1)
    {
        data[UnitEnemyRatio] *= static_cast<float>(enemeyCount - 1);
    }
    data[UnitCount] = pUnits->size();
    data[EnemyUnitCount] = pEnemyUnits->size();
    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS, m_pMap);
    float bestScore = NO_BUILD_SCORE + 1;
    std::vector<qint32> buildingIdx;
    std::vector<qint32> unitIDx;
    std::vector<float> scores;
    std::vector<bool> transporters;
    float variance = m_pMap->getCurrentDay() - 1 + m_startDayScoreVariancer;
    if (variance > m_maxDayScoreVariancer)
    {
        variance = m_maxDayScoreVariancer;
    }
    spQmlVectorPoint pFields = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        QCoreApplication::processEvents();
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
                        QCoreApplication::processEvents();
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
                                    data[TurnOneDamageMalus] = unitData.turnOneDamage;
                                    if (pEnemyBuildings->size() > 0 && enemeyCount > 0)
                                    {
                                        data[BuildingEnemyRatio] = static_cast<float>(pBuildings->size()) / (static_cast<float>(pEnemyBuildings->size()) / static_cast<float>(enemeyCount));
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
                                }
                                else
                                {
                                    score = NO_BUILD_SCORE;
                                }
                            }
                            if (score > bestScore)
                            {
                                bestScore = score;
                                buildingIdx.push_back(i);
                                unitIDx.push_back(i2);
                                scores.push_back(score);
                                transporters.push_back(isTransporter);
                                qint32 index = 0;
                                while (index < scores.size())
                                {
                                    if (scores[index] < bestScore - variance)
                                    {
                                        buildingIdx.erase(buildingIdx.cbegin() + index);
                                        unitIDx.erase(unitIDx.cbegin() + index);
                                        scores.erase(scores.cbegin() + index);
                                        transporters.erase(transporters.cbegin() + index);
                                    }
                                    else
                                    {
                                        index++;
                                    }
                                }
                            }
                            else if (score >= bestScore - variance)
                            {
                                buildingIdx.push_back(i);
                                unitIDx.push_back(i2);
                                scores.push_back(score);
                                transporters.push_back(isTransporter);
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
        AI_CONSOLE_PRINT("NormalAi::buildUnits() producing " + unitId + " with score " + QString::number(scores[item]), GameConsole::eDEBUG);
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
            if (pAction->canBePerformed())
            {
                m_updatePoints.push_back(pAction->getActionTarget());
                emit sigPerformAction(pAction);
                return true;
            }
        }
    }
    return false;
}

qint32 NormalAi::getIndexInProductionData(Building* pBuilding)
{
    AI_CONSOLE_PRINT("NormalAi::getIndexInProductionData()", GameConsole::eDEBUG);
    qint32 x = pBuilding->Building::getX();
    qint32 y = pBuilding->Building::getY();
    qint32 ret = -1;
    for (qint32 i = 0; i < m_productionData.size(); ++i)
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
        m_productionData.push_back(ProductionData());
        ret = m_productionData.size() - 1;
        m_productionData[ret].m_x = x;
        m_productionData[ret].m_y = y;
        m_productionData[ret].buildList = pBuilding->getConstructionList();
    }
    return ret;
}

qint32 NormalAi::getUnitProductionIdx(qint32 index, const QString & unitId,
                                      spQmlVectorUnit & pUnits, std::vector<std::tuple<Unit*, Unit*>> & transportTargets,
                                      spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                      std::vector<NotAttackableData> & attackCount, std::vector<float> & buildData)
{
    AI_CONSOLE_PRINT("NormalAi::getUnitProductionIdx()", GameConsole::eDEBUG);
    qint32 ret = -1;
    auto & data = m_productionData[index];
    for (qint32 i = 0; i < data.m_buildData.length(); ++i)
    {
        auto & unitData = data.m_buildData[i];
        if (unitData.unitId == unitId)
        {
            if (unitData.notAttackableCount > 0)
            {
                Unit dummy(unitId, m_pPlayer, false, m_pMap);
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
                unitData.turnOneDamage = damageData.turnOneDamage;
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
                                   spQmlVectorUnit & pUnits, std::vector<std::tuple<Unit*, Unit*>> & transportTargets,
                                   spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                   std::vector<NotAttackableData> & attackCount, std::vector<float> & buildData, const QStringList & buildList)
{
    QCoreApplication::processEvents();
    AI_CONSOLE_PRINT("NormalAi::createUnitBuildData()", GameConsole::eDEBUG);
    MovementTableManager* pMovementTableManager = MovementTableManager::getInstance();
    Unit dummy(unitBuildData.unitId, m_pPlayer, false, m_pMap);
    dummy.setVirtuellX(x);
    dummy.setVirtuellY(y);
    spTerrain pDummyTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", m_pMap);
    qint32 baseMovementCost = pMovementTableManager->getBaseMovementPoints(dummy.getMovementType(), pDummyTerrain.get(), pDummyTerrain.get(), &dummy);
    if (baseMovementCost < 0)
    {
        baseMovementCost = 1;
    }
    createIslandMap(dummy.getMovementType(), dummy.getUnitID());
    UnitPathFindingSystem pfs(m_pMap, &dummy, m_pPlayer);
    pfs.explore();
    unitBuildData.movePoints = dummy.getMovementpoints(QPoint(x, y)) / baseMovementCost;
    auto points = pfs.getAllNodePointsFast();
    if (points.size() >= unitBuildData.movePoints * 1.5f)
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
            unitBuildData.turnOneDamage = damageData.turnOneDamage;
            unitBuildData.cost = dummy.getUnitCosts();
            unitBuildData.infantryUnit = (actionList.contains(ACTION_CAPTURE) &&
                                          dummy.getLoadingPlace() == 0);
            unitBuildData.indirectUnit = dummy.getBaseMaxRange() > 1;
            unitBuildData.sameFundsMatchUpScore = calcSameFundsMatchUpScore(dummy, buildList);
        }
    }
}

void NormalAi::getEnemyDamageCounts(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, std::vector<NotAttackableData> & attackCount)
{
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        for (auto & pUnit : pUnits->getVector())
        {
            float dmg1 = 0.0f;
            float hpValue = static_cast<float>(pUnit->getHpRounded()) / Unit::MAX_UNIT_HP;
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

            if (dmg1 > m_highDamage || dmg2 > m_highDamage)
            {
                attackCount[i2].highDamageCount += 1;
            }
            else if (dmg1 > m_midDamage || dmg2 > m_midDamage)
            {
                attackCount[i2].midDamageCount += 1;
            }
            else if ((dmg1 > m_notAttackableDamage || dmg2 > m_notAttackableDamage))
            {
                if (onSameIsland(pUnit.get(), pEnemyUnits->at(i2)))
                {
                    attackCount[i2].sameIslandNotAttackableCount += 1;
                }
                attackCount[i2].notAttackableCount += 1;
            }
        }
    }
}

qint32 NormalAi::getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    qint32 minDistance = std::numeric_limits<qint32>::max();
    QPoint pos(posX, posY);
    qint32 islandIdx = CoreAI::getIslandIndex(&dummy);
    for (auto & pEnemyUnit : pEnemyUnits->getVector())
    {
        if (onSameIsland(islandIdx, posX, posY, pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY()))
        {
            if (dummy.isAttackable(pEnemyUnit.get(), true))
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
        for (auto & pBuilding : pEnemyBuildings->getVector())
        {
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

NormalAi::ExpectedFundsData NormalAi::calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit & pEnemyUnits, const std::vector<NotAttackableData> & attackCount, float bonusFactor, float myMovepoints)
{
    ExpectedFundsData ret;
    float notAttackableCount = 0;
    float damageCount = 0;
    float attacksCount = 0;
    qint32 baseAttacksCount = 0;
    float extraMalusCount = 0;

    float counterDamageCount = 0;
    float counterAttacksCount = 0;

    float turnOneDamage = 0;
    float turnOneDamageCount = 0;

    if (myMovepoints == 0)
    {
        myMovepoints = 1;
    }
    
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

    qint32 lowUnitAttackCount = 0;
    qint32 lowUnitDamageCount = 0;

    auto enemyUnitCount = pEnemyUnits->size();
    for (qint32 i3 = 0; i3 < enemyUnitCount; i3++)
    {
        Unit* pEnemyUnit = pEnemyUnits->at(i3);
        QPoint enemyPosition = pEnemyUnit->getPosition();
        float enemyFirerange = pEnemyUnit->getBaseMaxRange();
        float enemyMovepoints = pEnemyUnit->getBaseMovementPoints();
        float enemyRange = enemyMovepoints;
        float turnOneRange = enemyFirerange;
        if (pEnemyUnit->canMoveAndFire(enemyPosition))
        {
            turnOneRange += enemyMovepoints;
            enemyRange += enemyFirerange;
        }
        else
        {
            enemyRange = (enemyMovepoints + enemyFirerange) * 0.5f;
        }
        float distance = GlobalUtils::getDistance(position, enemyPosition);
        float dmg = getBaseDamage(&dummy, pEnemyUnit).x();
        float counterDmg = 0.0f;
        float baseCounterDmg = getBaseDamage(pEnemyUnit, &dummy).x();
        if ((baseCounterDmg < m_lowThreadDamage && dmg >= m_midDamage) ||
            dmg > pEnemyUnit->getHp() * Unit::MAX_UNIT_HP)
        {
            dmg = pEnemyUnit->getHp() * Unit::MAX_UNIT_HP;
        }
        counterDmg = baseCounterDmg * pEnemyUnit->getHp() / Unit::MAX_UNIT_HP;

        if (counterDmg > m_lowThreadDamage &&
            distance <= turnOneRange)
        {
            turnOneDamage += counterDmg;
            turnOneDamageCount += 1;
        }

        if (dmg > 0.0f)
        {
            bool firstStrikes = ownRange >= enemyRange || counterDmg <= m_notAttackableDamage;
            float resDamage = 0;

            auto enemyValue = pEnemyUnit->getCoUnitValue();
            resDamage = (dmg / (pEnemyUnit->getHp() * Unit::MAX_UNIT_HP)) * enemyValue;

            if (resDamage > enemyValue && resDamage > 0)
            {
                resDamage = enemyValue * (m_maxOverkillBonus - enemyValue / resDamage);
            }
            float mult = (ownRange + m_smoothingValue) / (enemyRange + m_smoothingValue);
            if (mult > m_maxDistanceMultiplier || counterDmg <= m_notAttackableDamage)
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
                factor += 1.5f - (static_cast<float>(attackCount[i3].highDamageCount) + m_smoothingValue) / (static_cast<float>(attackCount[i3].notAttackableCount) + m_smoothingValue);
            }
            else if (dmg > m_midDamage)
            {
                factor += 1.0f - (static_cast<float>(attackCount[i3].midDamageCount) + m_smoothingValue) / (static_cast<float>(attackCount[i3].notAttackableCount) + m_smoothingValue);
            }
            else if (dmg < m_notAttackableDamage)
            {
                ++extraMalusCount;
            }
            ++baseAttacksCount;
            bool sameIsland = onSameIsland(dummy.getMovementType(), posX, posY, pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY());
            float distanceBonus = 0;
            if (sameIsland)
            {
                distanceBonus = (m_sameIslandBonusInRangeDays - (distance / myMovepoints) * m_sameIslandOutOfDayMalusFactor);
                if (distanceBonus > m_maxCloseDistanceDamageBonus)
                {
                    distanceBonus = m_maxCloseDistanceDamageBonus;
                }
                else if (distanceBonus > m_minCloseDistanceDamageBonus)
                {
                    distanceBonus = m_minCloseDistanceDamageBonus;
                }
            }
            else
            {
                distanceBonus =  (m_differentIslandBonusInRangeDays - (distance / myMovepoints) * m_differentIslandOutOfDayMalusFactor);
                if (distanceBonus > m_maxCloseDistanceDamageBonus)
                {
                    distanceBonus = m_maxCloseDistanceDamageBonus;
                }
                else if (distanceBonus > m_minCloseDistanceDamageBonus)
                {
                    distanceBonus = m_minCloseDistanceDamageBonus;
                }
            }
            factor += distanceBonus;
            if (pEnemyUnit->hasWeapons())
            {
                float notAttackableValue = 0.0f;
                if (dmg >= m_highDamage)
                {
                    notAttackableValue = m_highDamageBonus;
                    if (sameIsland && distanceBonus >= 1)
                    {
                        if (attackCount[i3].midDamageCount == 0 &&
                            attackCount[i3].highDamageCount > 0 )
                        {
                            lowUnitAttackCount += attackCount[i3].highDamageCount;
                            lowUnitDamageCount++;
                        }
                    }
                }
                else if (dmg >= m_midDamage)
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
                if (!sameIsland)
                {
                    notAttackableValue *= 0.5f;
                }


                if (attackCount[i3].sameIslandNotAttackableCount == 0 &&
                    attackCount[i3].notAttackableCount == 0 &&
                    attackCount[i3].midDamageCount == 0 &&
                    attackCount[i3].highDamageCount == 0)
                {
                    notAttackableCount += notAttackableValue * m_currentlyNotAttackableBonus;
                }
                else if (attackCount[i3].notAttackableCount == 0 &&
                         attackCount[i3].midDamageCount == 0 &&
                         attackCount[i3].highDamageCount == 0)
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
            if (dmg >= m_midDamage)
            {
                attacksCount += factor;
            }
            else
            {
                extraMalusCount += factor;
            }
        }
        if (counterDmg >= 0.0f)
        {
            if ((counterDmg >= m_notAttackableDamage) ||
                (counterDmg <= m_notAttackableDamage && dmg <= m_notAttackableDamage))
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
    // low on counter units
    if (lowUnitAttackCount < lowUnitDamageCount * 2.0f &&
        lowUnitAttackCount > 0)
    {
        notAttackableCount += m_highDamageBonus * m_currentlyNotAttackableBonus;
    }

    float damage = 0.0f;
    if (attacksCount > 0.0f)
    {
        damage = damageCount / (attacksCount + extraMalusCount);
    }
    if (damage > 0)
    {
        float value = static_cast<float>(baseAttacksCount) / static_cast<float>(enemyUnitCount);
        if (baseAttacksCount > m_minAttackCountBonus)
        {
            damage *= static_cast<float>(baseAttacksCount + m_minAttackCountBonus) / static_cast<float>(enemyUnitCount);
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
                float factor = 1.0f / static_cast<float>(i);
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
    if (turnOneDamageCount > 0)
    {
        ret.turnOneDamage = turnOneDamage / (Unit::MAX_UNIT_HP * turnOneDamageCount);
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
        Unit dummyMatchUp(unitId, m_pPlayer, false, m_pMap);
        auto matchUpMovepoints = dummyMatchUp.getMovementpoints(dummy.getPosition());
        auto matchUpValue = dummyMatchUp.getUnitValue();

        if (matchUpValue <= dummyValue + dummyValue * m_targetPriceDifference &&
            matchUpValue >= dummyValue - dummyValue * m_targetPriceDifference)
        {
            float dmg = dummy.getBaseDamage(&dummyMatchUp);
            float counterDmg = dummyMatchUp.getBaseDamage(&dummy);
            if (dmg >= 0.0f &&
                counterDmg >= 0.0f)
            {
                if (dmg <= m_notAttackableDamage)
                {
                    dmg = 0.0f;
                }
                else if (matchUpMovepoints > movepoints)
                {
                    dmg *= m_sameFundsMatchUpMovementMalus;
                }
                resultScore += dmg;
                ++count;
            }
        }
    }
    if (count > 0)
    {
        return resultScore / (Unit::DAMAGE_100 / 2) / count;
    }
    else
    {
        return m_sameFundsMatchUpNoMatchUpValue;
    }
}

void NormalAi::getTransporterData(UnitBuildData & unitBuildData, Unit& dummy, spQmlVectorUnit & pUnits,
                                  spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                                  std::vector<std::tuple<Unit*, Unit*>>& transportTargets)
{
    std::vector<QVector3D> targets;
    spQmlVectorUnit relevantUnits = spQmlVectorUnit::create();
    QPoint position = dummy.getPosition();
    float movement = dummy.getBaseMovementPoints();
    if (movement == 0)
    {
        movement = 1;
    }
    
    qint32 loadingPlace = dummy.getLoadingPlace();
    qint32 smallTransporterCount = 0;
    qint32 transporterCount = 0;
    static constexpr float maxDayDistance = 6.0f;
    for (auto & pUnit : pUnits->getVector())
    {
        float distance = GlobalUtils::getDistance(position, pUnit->getPosition());
        if (distance / movement <= maxDayDistance)
        {
            relevantUnits->append(pUnit.get());
        }
        qint32 place = pUnit->getLoadingPlace();
        if (place > 1)
        {
            if (place == 1)
            {
                smallTransporterCount++;
            }
            ++transporterCount;
        }
    }
    bool onlyTrueIslands = loadingPlace > 1;
    std::vector<Unit*> loadingUnits = appendLoadingTargets(&dummy, relevantUnits, pEnemyUnits, pEnemyBuildings, false, true, targets, false, 1, onlyTrueIslands);
    std::vector<Unit*> transporterUnits;
    for (auto target : transportTargets)
    {
        if (!GlobalUtils::contains(transporterUnits, std::get<0>(target)))
        {
            transporterUnits.push_back(std::get<0>(target));
        }
    }
    qint32 i = 0;
    while ( i < loadingUnits.size())
    {
        QCoreApplication::processEvents();
        if (canTransportToEnemy(&dummy, loadingUnits[i], pEnemyUnits, pEnemyBuildings))
        {
            qint32 transporter = 0;
            for (auto & target : transportTargets)
            {
                if (std::get<1>(target)->getPosition() == loadingUnits[i]->getPosition())
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
            loadingUnits.erase(loadingUnits.cbegin() + i);
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

float NormalAi::calcTransporterScore(UnitBuildData & unitBuildData, spQmlVectorUnit & pUnits, std::vector<float>& data)
{
    float score = 0.0f;
    if (score == 0.0f && pUnits->size() / (unitBuildData.smallTransporterCount + 1) > m_unitToSmallTransporterRatio && unitBuildData.loadingPlace == 1)
    {
        
        if (unitBuildData.smallTransporterCount > 0)
        {
            score += qMin(m_smallTransporterBonus,  static_cast<float>(pUnits->size()) / static_cast<float>(unitBuildData.smallTransporterCount + 1.0f) * 10.0f);

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
        score += (static_cast<float>(unitBuildData.loadingCount) / static_cast<float>(unitBuildData.transportCount)) * m_ProducingTransportRatioBonus;
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
            static_cast<float>(unitBuildData.loadingCount)  / static_cast<float>(unitBuildData.transportCount + 1) >= m_ProducingTransportMinLoadingTransportRatio)
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
            score -= unitBuildData.transporterCount * m_additionalLoadingUnitBonus;
            AI_CONSOLE_PRINT("NormalAi::calcTransporterScore for " + unitBuildData.unitId +
                             " score=" + QString::number(score) +
                             " loadingCount=" + QString::number(unitBuildData.loadingCount) +
                             " transportCount=" + QString::number(unitBuildData.transportCount) +
                             " transportCounter=" + QString::number(unitBuildData.transporterCount), GameConsole::eDEBUG);
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

float NormalAi::calcBuildScore(std::vector<float>& data, UnitBuildData & unitBuildData)
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
         m_pMap->getCurrentDay() > m_earlyGame))
    {
        float attackScore = 0.0f;
        float attackCountScore = 0.0f;
        attackScore += data[DamageData] / data[UnitCost] * m_damageToUnitCostRatioBonus;
        attackScore += (1.0f - data[CounterDamage] / data[UnitCost]) * m_counterDamageBonus;
        if (data[EnemyUnitCount] > 0)
        {
            attackCountScore += data[AttackCount] / data[EnemyUnitCount] * m_attackCountBonus;
        }
        float sameFoundsScore = data[SameFundsMatchUpScore] * m_sameFundsMatchUpBonus;
        score += attackScore+ attackCountScore + sameFoundsScore;
        AI_CONSOLE_PRINT("NormalAi::calcBuildScore damage=" + QString::number(data[DamageData]) +
                         " and counter damage " + QString::number(data[CounterDamage]) +
                         " attack score=" + QString::number(attackScore) +
                         " attack count score=" + QString::number(attackCountScore) +
                         " same founds score=" + QString::number(sameFoundsScore), GameConsole::eDEBUG);
    }
    // apply co buff bonus
    score += data[COBonus] * m_coUnitBuffBonus;
    score += calcSupplyScore(data, unitBuildData);
    score -= data[TurnOneDamageMalus] * m_turnOneDmageMalus;
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
    score *= BaseGameInputIF::getUnitBuildValue(unitBuildData.unitId);
    AI_CONSOLE_PRINT("NormalAi::calcBuildScore final score=" + QString::number(score) + " for " + unitBuildData.unitId, GameConsole::eDEBUG);
    return score;
}

float NormalAi::calcSupplyScore(std::vector<float>& data, UnitBuildData & unitBuildData)
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

float NormalAi::calcCostScore(std::vector<float>& data, UnitBuildData & unitBuildData)
{
    float score = 0;
    // funds bonus
    double normalDifference = data[FundsFactoryRatio] - m_normalUnitRatio;
    double cheapDifference = data[FundsFactoryRatio] - m_cheapUnitRatio;
    const double outScore = 0.25f;
    const double inScore = 0.5f;

    if (data[UseHighTechUnits] == static_cast<float>(NoSpendMalus) &&
        data[FundsFactoryRatio] > m_normalUnitRatio + m_targetPriceDifference)
    {
        // spend what we can mode
        score = 0;
    }
    else if (data[UseHighTechUnits] > static_cast<float>(FundsMode::Normal) &&
             normalDifference > m_targetPriceDifference)
    {
        // expensive malus
        score = m_normalUnitBonusMultiplier * outScore - m_normalUnitBonusMultiplier * (qAbs(normalDifference) - m_targetPriceDifference);
    }
    else if (data[FundsFactoryRatio] > m_superiorityRatio)
    {
        score = m_normalUnitBonusMultiplier * outScore - m_expensiveUnitBonusMultiplier * (qAbs(normalDifference) - m_targetPriceDifference);
    }
    else if (qAbs(normalDifference) <= m_targetPriceDifference)
    {
        score = m_normalUnitBonusMultiplier * (1.0 - qAbs(normalDifference) / m_targetPriceDifference * inScore);
    }
    else if (qAbs(cheapDifference) <= m_targetPriceDifference &&
             data[UseHighTechUnits] <= static_cast<float>(FundsMode::Expensive))
    {
        if (data[LowFunds] > 0)
        {
            score = m_cheapUnitBonusMultiplier * (1.0 - qAbs(cheapDifference) / m_targetPriceDifference * inScore);
        }
        else
        {
            score = m_normalUnitBonusMultiplier * outScore - m_cheapUnitBonusMultiplier * (qAbs(normalDifference) - m_targetPriceDifference);
        }
    }
    else
    {
        score = m_normalUnitBonusMultiplier * outScore - m_normalUnitBonusMultiplier * (qAbs(normalDifference) - m_targetPriceDifference);
    }
    AI_CONSOLE_PRINT("NormalAi::calcCostScore score=" + QString::number(score) +
                     " funds ratio=" + QString::number(data[FundsFactoryRatio]), GameConsole::eDEBUG);
    return score;
}
