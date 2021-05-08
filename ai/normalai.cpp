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


NormalAi::NormalAi(QString configurationFile)
    : CoreAI (GameEnums::AiTypes_Normal)
{
    setObjectName("NormalAi");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    loadIni("normal/" + configurationFile);
}


void NormalAi::readIni(QString name)
{
    if (QFile::exists(name))
    {
        QSettings settings(name, QSettings::IniFormat);
        settings.beginGroup("general");
        bool ok = false;
        m_minMovementDamage = settings.value("MinMovementDamage", 0.3f).toFloat(&ok);
        if(!ok)
        {
            m_minMovementDamage = 0.3f;
        }
        m_minAttackFunds = settings.value("MinAttackFunds", 0).toFloat(&ok);
        if(!ok)
        {
            m_minAttackFunds = 0;
        }
        m_minAttackFunds = settings.value("MinSuicideDamage", 0.75f).toFloat(&ok);
        if(!ok)
        {
            m_minSuicideDamage = 0.75f;
        }
        m_spamingFunds = settings.value("SpamingFunds", 7500).toFloat(&ok);
        if(!ok)
        {
            m_spamingFunds = 0;
        }
        m_ownUnitValue = settings.value("OwnUnitValue", 2.0f).toFloat(&ok);
        if(!ok)
        {
            m_ownUnitValue = 2.0f;
        }
        m_buildingValue = settings.value("BuildingValue", 1.0f).toFloat(&ok);
        if(!ok)
        {
            m_buildingValue = 1.0f;
        }
        m_notAttackableDamage = settings.value("NotAttackableDamage", 25).toFloat(&ok);
        if(!ok)
        {
            m_notAttackableDamage = 25;
        }
        m_midDamage = settings.value("MidDamage", 55).toFloat(&ok);
        if(!ok)
        {
            m_midDamage = 55;
        }
        m_highDamage = settings.value("HighDamage", 65).toFloat(&ok);
        if(!ok)
        {
            m_highDamage = 65;
        }
        m_directIndirectRatio = settings.value("DirectIndirectRatio", 1.75f).toFloat(&ok);
        if(!ok)
        {
            m_directIndirectRatio = 1.75f;
        }
        m_minSiloDamage = settings.value("MinSiloDamage", 7000).toFloat(&ok);
        if(!ok)
        {
            m_minSiloDamage = 7000;
        }
        settings.endGroup();
        settings.beginGroup("CoUnit");
        m_coUnitValue = settings.value("CoUnitValue", 6000).toInt(&ok);
        if(!ok)
        {
            m_coUnitValue = 6000;
        }
        m_minCoUnitScore = settings.value("MinCoUnitScore", 5000).toFloat(&ok);
        if(!ok)
        {
            m_minCoUnitScore = 5000;
        }
        m_coUnitRankReduction = settings.value("CoUnitRankReduction", 1000).toFloat(&ok);
        if(!ok)
        {
            m_coUnitRankReduction = 1000;
        }
        m_coUnitScoreMultiplier = settings.value("CoUnitScoreMultiplier", 1.1f).toFloat(&ok);
        if(!ok)
        {
            m_coUnitScoreMultiplier = 1.1f;
        }
        m_minCoUnitCount = settings.value("MinCoUnitCount", 1.1f).toInt(&ok);
        if(!ok)
        {
            m_minCoUnitCount = 5;
        }
        settings.endGroup();
        settings.beginGroup("Repairing");
        m_minUnitHealth = settings.value("MinUnitHealth", 3).toInt(&ok);
        if(!ok)
        {
            m_minUnitHealth = 3;
        }
        m_maxUnitHealth = settings.value("MaxUnitHealth", 7).toInt(&ok);
        if(!ok)
        {
            m_maxUnitHealth = 7;
        }
        m_fuelResupply = settings.value("FuelResupply", 0.33f).toFloat(&ok);
        if(!ok)
        {
            m_fuelResupply = 0.33f;
        }
        m_ammoResupply = settings.value("AmmoResupply", 0.25f).toFloat(&ok);
        if(!ok)
        {
            m_ammoResupply = 0.25f;
        }
        settings.endGroup();
        settings.beginGroup("Moving");
        m_lockedUnitHp = settings.value("LockedUnitHp", 4).toFloat(&ok);
        if(!ok)
        {
            m_lockedUnitHp = 4;
        }
        m_noMoveAttackHp = settings.value("NoMoveAttackHp", 3.5f).toFloat(&ok);
        if(!ok)
        {
            m_noMoveAttackHp = 3.5f;
        }
        settings.endGroup();
        settings.beginGroup("Attacking");
        m_ownIndirectAttackValue = settings.value("OwnIndirectAttackValue", 2.0f).toFloat(&ok);
        if(!ok)
        {
            m_ownIndirectAttackValue= 2.0f;
        }
        m_enemyKillBonus = settings.value("EnemyKillBonus", 2.0f).toFloat(&ok);
        if(!ok)
        {
            m_enemyKillBonus= 2.0f;
        }
        m_enemyIndirectBonus = settings.value("EnemyIndirectBonus", 3.0f).toFloat(&ok);
        if(!ok)
        {
            m_enemyIndirectBonus= 3.0f;
        }

        m_antiCaptureHqBonus = settings.value("AntiCaptureHqBonus", 50.0f).toFloat(&ok);
        if(!ok)
        {
            m_antiCaptureHqBonus= 50.0f;
        }
        m_antiCaptureBonus = settings.value("AntiCaptureBonus", 21.0f).toFloat(&ok);
        if(!ok)
        {
            m_antiCaptureBonus= 21.0f;
        }
        m_antiCaptureBonusScoreReduction = settings.value("AntiCaptureBonusScoreReduction", 6.0f).toFloat(&ok);
        if(!ok)
        {
            m_antiCaptureBonusScoreReduction= 6.0f;
        }
        m_antiCaptureBonusScoreReduction = settings.value("AntiCaptureBonusScoreDivider", 2.0f).toFloat(&ok);
        if(!ok || m_antiCaptureBonusScoreReduction < 0)
        {
            m_antiCaptureBonusScoreReduction = 2.0f;
        }
        m_enemyCounterDamageMultiplier = settings.value("EnemyCounterDamageMultiplier", 10.0f).toFloat(&ok);
        if(!ok)
        {
            m_enemyCounterDamageMultiplier = 10.0f;
        }
        m_watermineDamage = settings.value("WatermineDamage", 4.0f).toFloat(&ok);
        if(!ok)
        {
            m_watermineDamage = 4.0f;
        }
        m_enemyUnitCountDamageReductionMultiplier = settings.value("EnemyUnitCountDamageReductionMultiplier", 0.5f).toFloat(&ok);
        if(!ok)
        {
            m_enemyUnitCountDamageReductionMultiplier = 0.5f;
        }
        settings.endGroup();
        settings.beginGroup("Production");
        m_fundsPerBuildingFactorA = settings.value("FundsPerBuildingFactorA", 2.5f).toFloat(&ok);
        if(!ok)
        {
            m_fundsPerBuildingFactorA = 2.5f;
        }
        m_fundsPerBuildingFactorB = settings.value("FundsPerBuildingFactorB", 1.65f).toFloat(&ok);
        if(!ok)
        {
            m_fundsPerBuildingFactorB = 1.65f;
        }
        m_ownUnitEnemyUnitRatioAverager = settings.value("OwnUnitEnemyUnitRatioAverager", 10).toFloat(&ok);
        if(!ok)
        {
            m_ownUnitEnemyUnitRatioAverager = 10;
        }
        m_maxDayScoreVariancer = settings.value("MaxTransporterDayScoreVariancer", 10).toFloat(&ok);
        if(!ok)
        {
            m_maxDayScoreVariancer = 10;
        }
        m_directIndirectUnitBonusFactor = settings.value("DirectIndirectUnitBonusFactor", 1.2f).toFloat(&ok);
        if(!ok)
        {
            m_directIndirectUnitBonusFactor = 1.2f;
        }
        m_maxBuildingTargetFindLoops = settings.value("MaxBuildingTargetFindLoops", 5).toFloat(&ok);
        if(!ok)
        {
            m_maxBuildingTargetFindLoops = 5;
        }
        m_scoringCutOffDamageHigh = settings.value("ScoringCutOffDamageHigh", Unit::DAMAGE_100).toFloat(&ok);
        if(!ok)
        {
            m_scoringCutOffDamageHigh = Unit::DAMAGE_100;
        }
        m_scoringCutOffDamageLow = settings.value("ScoringCutOffDamageLow", 7.5f).toFloat(&ok);
        if(!ok)
        {
            m_scoringCutOffDamageLow = 7.5f;
        }
        m_smoothingValue = settings.value("SmoothingValue", 3).toFloat(&ok);
        if(!ok)
        {
            m_smoothingValue = 3;
        }
        m_maxDistanceMultiplier = settings.value("MaxDistanceMultiplier", 1.5f).toFloat(&ok);
        if(!ok)
        {
            m_maxDistanceMultiplier = 1.5f;
        }
        m_sameIslandBonusInRangeDays = settings.value("SameIslandBonusInRangeDays", 2).toFloat(&ok);
        if(!ok)
        {
            m_sameIslandBonusInRangeDays = 2;
        }
        m_sameIslandOutOfDayMalusFactor = settings.value("SameIslandOutOfDayMalusFactor", 0.2f).toFloat(&ok);
        if(!ok)
        {
            m_sameIslandOutOfDayMalusFactor = 0.2f;
        }
        m_highDamageBonus = settings.value("HighDamageBonus", 2).toFloat(&ok);
        if(!ok)
        {
            m_highDamageBonus = 2;
        }
        m_midDamageBonus = settings.value("MidDamageBonus", 1.5f).toFloat(&ok);
        if(!ok)
        {
            m_midDamageBonus = 1.5f;
        }

        m_lowDamageBonus = settings.value("LowDamageBonus", 1).toFloat(&ok);
        if(!ok)
        {
            m_lowDamageBonus = 1;
        }

        m_veryLowDamageBonus = settings.value("VeryLowDamageBonus", 0.5f).toFloat(&ok);
        if(!ok)
        {
            m_veryLowDamageBonus = 0.5f;
        }
        m_transportBonus = settings.value("TransportBonus", 0.125f).toFloat(&ok);
        if(!ok)
        {
            m_transportBonus = 0.125f;
        }
        m_currentlyNotAttackableBonus = settings.value("CurrentlyNotAttackableBonus", 0.5f).toFloat(&ok);
        if(!ok)
        {
            m_currentlyNotAttackableBonus = 0.5f;
        }
        m_differentIslandBonusInRangeDays = settings.value("DifferentIslandBonusInRangeDays", 1).toFloat(&ok);
        if(!ok)
        {
            m_differentIslandBonusInRangeDays = 1;
        }
        m_differentIslandOutOfDayMalusFactor = settings.value("DifferentIslandOutOfDayMalusFactor", 0.33f).toFloat(&ok);
        if(!ok)
        {
            m_differentIslandOutOfDayMalusFactor = 0.33f;
        }

        m_noTransporterBonus = settings.value("NoTransporterBonus", 70).toFloat(&ok);
        if(!ok)
        {
            m_noTransporterBonus = 70;
        }
        m_transporterToRequiredPlaceFactor = settings.value("TransporterToRequiredPlaceFactor", 3).toFloat(&ok);
        if(!ok)
        {
            m_transporterToRequiredPlaceFactor = 3;
        }

        m_minFlyingTransportScoreForBonus = settings.value("MinFlyingTransportScoreForBonus", 15).toFloat(&ok);
        if(!ok)
        {
            m_minFlyingTransportScoreForBonus = 15;
        }
        m_flyingTransporterBonus = settings.value("FlyingTransporterBonus", 15).toFloat(&ok);
        if(!ok)
        {
            m_flyingTransporterBonus = 15;
        }
        m_smallTransporterBonus = settings.value("SmallTransporterBonus", 30).toFloat(&ok);
        if(!ok)
        {
            m_smallTransporterBonus = 30;
        }
        m_unitToSmallTransporterRatio = settings.value("UnitToSmallTransporterRatio", 5).toFloat(&ok);
        if(!ok)
        {
            m_unitToSmallTransporterRatio = 5;
        }
        m_additionalLoadingUnitBonus = settings.value("AdditionalLoadingUnitBonus", 5).toFloat(&ok);
        if(!ok)
        {
            m_additionalLoadingUnitBonus = 5;
        }
        m_indirectUnitAttackCountMalus = settings.value("IndirectUnitAttackCountMalus", 4).toInt(&ok);
        if(!ok)
        {
            m_indirectUnitAttackCountMalus = 4;
        }
        m_minAttackCountBonus = settings.value("MinAttackCountBonus", 5).toFloat(&ok);
        if(!ok)
        {
            m_minAttackCountBonus = 5;
        }
        m_lowIndirectUnitBonus = settings.value("LowIndirectUnitBonus", 0.3f).toFloat(&ok);
        if(!ok)
        {
            m_lowIndirectUnitBonus = 0.3f;
        }
        m_lowIndirectMalus = settings.value("LowIndirectMalus", 0.5f).toFloat(&ok);
        if(!ok)
        {
            m_lowIndirectMalus = 0.5f;
        }
        m_highIndirectMalus = settings.value("HighIndirectMalus", 0.6f).toFloat(&ok);
        if(!ok)
        {
            m_highIndirectMalus = 0.6f;
        }
        m_lowDirectUnitBonus = settings.value("LowDirectUnitBonus", 0.35f).toFloat(&ok);
        if(!ok)
        {
            m_lowDirectUnitBonus = 0.35f;
        }
        m_lowDirectMalus = settings.value("LowDirectMalus", 0.3f).toFloat(&ok);
        if(!ok)
        {
            m_lowDirectMalus = 0.3f;
        }
        m_highDirectMalus = settings.value("HighDirectMalus", 0.6f).toFloat(&ok);
        if(!ok)
        {
            m_highDirectMalus = 0.6f;
        }

        m_minUnitCountForDamageBonus = settings.value("MinUnitCountForDamageBonus", 3).toFloat(&ok);
        if(!ok)
        {
            m_minUnitCountForDamageBonus = 3;
        }
        m_currentlyNotAttackableScoreBonus = settings.value("CurrentlyNotAttackableScoreBonus", 30).toFloat(&ok);
        if(!ok)
        {
            m_currentlyNotAttackableScoreBonus = 30;
        }
        m_coUnitBuffBonus = settings.value("CoUnitBuffBonus", 17).toFloat(&ok);
        if(!ok)
        {
            m_coUnitBuffBonus = 17;
        }
        m_nearEnemyBonus = settings.value("NearEnemyBonus", 10).toFloat(&ok);
        if(!ok)
        {
            m_nearEnemyBonus = 10;
        }
        m_movementpointBonus = settings.value("MovementpointBonus", 0.33f).toFloat(&ok);
        if(!ok)
        {
            m_movementpointBonus = 0.33f;
        }
        m_damageToUnitCostRatioBonus = settings.value("DamageToUnitCostRatioBonus", 20).toFloat(&ok);
        if(!ok)
        {
            m_damageToUnitCostRatioBonus = 20;
        }
        m_superiorityRatio = settings.value("SuperiorityRatio", 2.5f).toFloat(&ok);
        if(!ok)
        {
            m_superiorityRatio = 2.5f;
        }
        m_cheapUnitRatio = settings.value("CheapUnitRatio", 0.9f).toFloat(&ok);
        if(!ok)
        {
            m_cheapUnitRatio = 0.9f;
        }
        m_cheapUnitBonusMultiplier = settings.value("CheapUnitBonusMultiplier", 40).toFloat(&ok);
        if(!ok)
        {
            m_cheapUnitBonusMultiplier = 40;
        }
        m_normalUnitBonusMultiplier = settings.value("NormalUnitBonusMultiplier", 10).toFloat(&ok);
        if(!ok)
        {
            m_normalUnitBonusMultiplier = 10;
        }
        m_damageToUnitCostRatioBonus = settings.value("ExpensiveUnitBonusMultiplier", 5).toFloat(&ok);
        if(!ok)
        {
            m_damageToUnitCostRatioBonus = 5;
        }

        m_lowOwnBuildingEnemyBuildingRatio = settings.value("LowOwnBuildingEnemyBuildingRatio", 1.25f).toFloat(&ok);
        if(!ok)
        {
            m_lowOwnBuildingEnemyBuildingRatio = 1.25f;
        }
        m_lowInfantryRatio = settings.value("LowInfantryRatio", 0.4f).toFloat(&ok);
        if(!ok)
        {
            m_lowInfantryRatio = 0.4f;
        }
        m_lowIncomeInfantryBonusMultiplier = settings.value("LowIncomeInfantryBonusMultiplier", 50).toFloat(&ok);
        if(!ok)
        {
            m_lowIncomeInfantryBonusMultiplier = 50;
        }

        m_ProducingTransportSearchrange = settings.value("ProducingTransportSearchrange", 6).toFloat(&ok);
        if(!ok)
        {
            m_ProducingTransportSearchrange = 6;
        }
        m_ProducingTransportSizeBonus = settings.value("ProducingTransportSizeBonus", 10).toFloat(&ok);
        if(!ok)
        {
            m_ProducingTransportSizeBonus = 10;
        }
        m_ProducingTransportRatioBonus = settings.value("ProducingTransportRatioBonus", 10.0f).toFloat(&ok);
        if(!ok)
        {
            m_ProducingTransportRatioBonus = 10.0f;
        }
        m_ProducingTransportLoadingBonus = settings.value("ProducingTransportLoadingBonus", 15.0f).toFloat(&ok);
        if(!ok)
        {
            m_ProducingTransportLoadingBonus = 15.0f;
        }
        m_ProducingTransportMinLoadingTransportRatio = settings.value("ProducingTransportMinLoadingTransportRatio", 3.0f).toFloat(&ok);
        if(!ok)
        {
            m_ProducingTransportMinLoadingTransportRatio = 3.0f;
        }
        settings.endGroup();
    }
}

void NormalAi::process()
{
    spQmlVectorBuilding pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    spQmlVectorUnit pUnits = nullptr;
    spQmlVectorUnit pEnemyUnits = nullptr;
    spQmlVectorBuilding pEnemyBuildings = nullptr;
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
        updateEnemyData(pUnits);
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
                if (aiStep == AISteps::buildUnits)
                {
                    aiStep = AISteps::moveUnits;
                }
                if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
                else
                {
                    if (aiStep == AISteps::buildUnits)
                    {
                        aiStep = AISteps::moveUnits;
                    }
                    clearEnemyData();
                    m_IslandMaps.clear();
                    turnMode = GameEnums::AiTurnMode_EndOfDay;
                    if (useCOPower(pUnits, pEnemyUnits))
                    {
                        usedTransportSystem = false;
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
    Console::print("NormalAi::finishTurn()", Console::eDEBUG);
    m_productionData.clear();
    CoreAI::finishTurn();
}

bool NormalAi::performActionSteps(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits,
                                  spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    if (aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveFlares(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && captureBuildings(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    // indirect units
    else if (aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 2, std::numeric_limits<qint32>::max(), pBuildings, pEnemyBuildings)){}
    // direct units
    else if (aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 1, 1, pBuildings, pEnemyBuildings)){}
    else if (aiStep <= AISteps::moveUnits && repairUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (aiStep <= AISteps::moveToTargets && refillUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, 1)){}
    else if (aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 2, std::numeric_limits<qint32>::max())){}
    else if (aiStep <= AISteps::loadUnits && !usedTransportSystem && loadUnits(pUnits, pBuildings, pEnemyBuildings)){}
    else if (aiStep <= AISteps::moveTransporters && !usedTransportSystem && moveTransporters(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
    else
    {
        if (!usedTransportSystem)
        {
            usedTransportSystem = true;
            aiStep = AISteps::moveUnits;
            return performActionSteps(pUnits, pEnemyUnits,  pBuildings, pEnemyBuildings);
        }
        else if (aiStep <= AISteps::loadUnits && loadUnits(pUnits, pBuildings, pEnemyBuildings)){}
        else if (aiStep <= AISteps::moveSupportUnits && moveSupport(AISteps::moveSupportUnits, pUnits, true)){}
        else if (aiStep <= AISteps::moveSupportUnits && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, std::numeric_limits<qint32>::max(), true)){}
        else if (aiStep <= AISteps::moveAway && moveAwayFromProduction(pUnits)){}
        else if (aiStep <= AISteps::buildUnits && buildUnits(pBuildings, pUnits, pEnemyUnits, pEnemyBuildings)){}
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

bool NormalAi::captureBuildings(spQmlVectorUnit pUnits)
{
    Console::print("NormalAi::captureBuildings()", Console::eDEBUG);
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

bool NormalAi::fireWithUnits(spQmlVectorUnit pUnits, qint32 minfireRange, qint32 maxfireRange,
                             spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::fireWithUnits()", Console::eDEBUG);
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
            QVector<QVector4D> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, pAction, &pfs, ret, moveTargetFields);
            qint32 targetIdx = getBestAttackTarget(pUnit, pUnits, ret, moveTargetFields, pBuildings, pEnemyBuildings);
            if (targetIdx >= 0)
            {
                QVector4D target = ret[targetIdx];
                QVector<QPoint> path = pfs.getPath(static_cast<qint32>(moveTargetFields[targetIdx].x()),
                                                   static_cast<qint32>(moveTargetFields[targetIdx].y()));
                pAction->setMovepath(path, pfs.getCosts(path));
                CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
                if (GameMap::getInstance()->getTerrain(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()))->getUnit() == nullptr)
                {
                    m_IslandMaps.clear();
                }
                if (pAction->isFinalStep() && pAction->canBePerformed())
                {
                    m_updatePoints.append(pUnit->getPosition());
                    m_updatePoints.append(pAction->getActionTarget());
                    m_updatePoints.append(QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
                    emit performAction(pAction);
                    return true;
                }
            }
        }
    }
    return false;
}

bool NormalAi::refillUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::refillUnits()", Console::eDEBUG);
    if (aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    aiStep = AISteps::moveToTargets;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (isUsingUnit(pUnit))
        {
            QStringList actions = pUnit->getActionList();
            if (actions.contains(ACTION_SUPPORTALL_RATION) ||
                actions.contains(ACTION_SUPPORTSINGLE_REPAIR) ||
                actions.contains(ACTION_SUPPORTSINGLE_FREEREPAIR))
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
                else if (usedTransportSystem)
                {
                    Console::print("move to supply needed units", Console::eDEBUG);
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

bool NormalAi::getBestRefillTarget(UnitPathFindingSystem & pfs, qint32 maxRefillCount, QPoint & moveTarget, QPoint & refillTarget)
{
    bool ret = false;
    const auto points = pfs.getAllNodePoints();
    spGameMap pMap = GameMap::getInstance();
    spQmlVectorPoint circle = GlobalUtils::getCircle(1, 1);
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

void NormalAi::appendRefillTargets(QStringList actions, Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector3D>& targets)
{
    if (actions.contains(ACTION_SUPPORTALL_RATION) ||
        actions.contains(ACTION_SUPPORTSINGLE_REPAIR) ||
        actions.contains(ACTION_SUPPORTSINGLE_FREEREPAIR))
    {
        spQmlVectorPoint circle = GlobalUtils::getCircle(1, 1);
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

bool NormalAi::moveUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings,
                         spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                         qint32 minfireRange, qint32 maxfireRange, bool supportUnits)
{
    Console::print("NormalAi::moveUnits()", Console::eDEBUG);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if ((isUsingUnit(pUnit) || usedTransportSystem) &&
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

bool NormalAi::loadUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::loadUnits()", Console::eDEBUG);
    aiStep = AISteps::loadUnits;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() &&
            // we don't support multi transporting for the ai for now this will break the system trust me
            pUnit->getLoadingPlace() <= 0)
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

bool NormalAi::moveTransporters(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::moveTransporters()", Console::eDEBUG);
    aiStep = AISteps::moveTransporters;
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

bool NormalAi::moveToUnloadArea(spGameAction pAction, Unit* pUnit, spQmlVectorUnit pUnits, QStringList& actions,
                                QVector<QVector3D>& targets,
                                spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::moveToUnloadArea()", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        if (CoreAI::contains(targets, targetFields))
        {
            UnitPathFindingSystem turnPfs(pUnit);
            turnPfs.explore();
            QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
            pAction->setMovepath(path, turnPfs.getCosts(path));
            pAction->setActionID(ACTION_UNLOAD);
            if (pAction->canBePerformed())
            {
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
                            QJSValue ret = pInterpreter->doFunction("ACTION_UNLOAD", function1, args1);
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
            }
        }
        else
        {
            return moveUnit(pAction, pUnit, pUnits, actions, targets, targets, true, pBuildings, pEnemyBuildings);
        }
    }
    return false;
}

bool NormalAi::repairUnits(spQmlVectorUnit pUnits, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::repairUnits()", Console::eDEBUG);
    aiStep = AISteps::moveUnits;
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

bool NormalAi::moveUnit(spGameAction pAction, Unit* pUnit, spQmlVectorUnit pUnits, QStringList& actions,
                        QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets,
                        bool shortenPathForTarget,
                        spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::moveUnit()", Console::eDEBUG);
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();
        if (CoreAI::contains(transporterTargets, targetFields))
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
                     minDamage > pUnit->getUnitValue() / 2 ||
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
                    (ret[0].z() >= -pUnit->getUnitValue()  * m_minSuicideDamage ||
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
                    if (ret.size() > 0 && ret[0].z() >= -pUnit->getUnitValue()  * m_minSuicideDamage)
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
                    Q_ASSERT(false);
                }
            }
        }
    }
    return false;
}

bool NormalAi::suicide(spGameAction pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs)
{
    Console::print("NormalAi::suicide", Console::eDEBUG);
    // we don't have a good option do the best that we can attack with an all in attack :D
    pAction->setActionID(ACTION_FIRE);
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &turnPfs, ret, moveTargetFields);
    if (ret.size() > 0 && ret[0].z() >= -pUnit->getUnitValue() * m_minSuicideDamage)
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

std::tuple<QPoint, float, bool> NormalAi::moveToSafety(Unit* pUnit, spQmlVectorUnit pUnits,
                                                       UnitPathFindingSystem& turnPfs, QPoint target,
                                                       spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::moveToSafety", Console::eDEBUG);
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

qint32 NormalAi::getMoveTargetField(Unit* pUnit, spQmlVectorUnit pUnits, UnitPathFindingSystem& turnPfs,
                                    QVector<QPoint>& movePath, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < movePath.size(); i++)
    {
        // empty or own field
        qint32 x = movePath[i].x();
        qint32 y = movePath[i].y();
        if ((pMap->getTerrain(x, y)->getUnit() == nullptr ||
             pMap->getTerrain(x, y)->getUnit() == pUnit) &&
            turnPfs.getCosts(turnPfs.getIndex(x, y), x, y, x, y) > 0)
        {
            float counterDamage = calculateCounterDamage(pUnit, pUnits, movePath[i], nullptr, 0.0f, pBuildings, pEnemyBuildings);
            if (counterDamage < pUnit->getUnitValue() * m_minMovementDamage)
            {
                return i;
            }
        }
    }
    return -1;
}

qint32 NormalAi::getBestAttackTarget(Unit* pUnit, spQmlVectorUnit pUnits, QVector<QVector4D>& ret,
                                     QVector<QVector3D>& moveTargetFields,
                                     spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 target = -1;
    qint32 currentDamage = std::numeric_limits<qint32>::min();
    qint32 deffense = 0;

    float minFundsDamage = -pUnit->getUnitValue() * m_minAttackFunds;

    for (qint32 i = 0; i < ret.size(); i++)
    {
        QPoint moveTarget(static_cast<qint32>(moveTargetFields[i].x()), static_cast<qint32>(moveTargetFields[i].y()));
        Unit* pEnemy = pMap->getTerrain(static_cast<qint32>(ret[i].x()), static_cast<qint32>(ret[i].y()))->getUnit();
        qint32 minfireRange = pUnit->getMinRange(moveTarget);
        qint32 fundsDamage = 0;
        float newHp = 0.0f;
        if (pEnemy != nullptr)
        {
            newHp = pEnemy->getHp() - static_cast<float>(ret[i].w());
            fundsDamage = static_cast<qint32>(ret[i].z() * calculateCaptureBonus(pEnemy, newHp));
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

        }
        else
        {
            fundsDamage = static_cast<qint32>(ret[i].z());
        }
        float counterDamage = calculateCounterDamage(pUnit, pUnits, moveTarget, pEnemy, ret[i].w(), pBuildings, pEnemyBuildings);
        if (counterDamage < 0)
        {
            counterDamage = 0;
        }
        fundsDamage -= counterDamage;
        qint32 targetDefense = pMap->getTerrain(static_cast<qint32>(ret[i].x()), static_cast<qint32>(ret[i].y()))->getDefense(pUnit);
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

float NormalAi::calculateCaptureBonus(Unit* pUnit, float newLife)
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

float NormalAi::calculateCounterDamage(Unit* pUnit, spQmlVectorUnit pUnits, QPoint newPosition,
                                       Unit* pEnemyUnit, float enemyTakenDamage,
                                       spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings,
                                       bool ignoreOutOfVisionRange)
{
    qint32 baseCost = pUnit->getUnitValue();
    QVector<qint32> baseCosts;
    for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
    {
        baseCosts.append(pUnits->at(i3)->getUnitValue());
    }
    spGameMap pMap = GameMap::getInstance();
    float counterDamage = 0;
    for (qint32 i = 0; i < m_EnemyUnits.size(); i++)
    {
        spUnit pNextEnemy = m_EnemyUnits[i];
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
                QRectF damageData;
                QVector<QPoint> targets = m_EnemyPfs[i]->getAllNodePoints();
                if (distance >= minFireRange && distance <= maxFireRange)
                {
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
                                    damageData.setX(damageData.x() -  damageData.x() * baseCosts[i3] / baseCost / 2);
                                }
                                else
                                {
                                    damageData.setX(damageData.x() -  damageData.x() *  baseCost / baseCosts[i3] / 2);
                                }
                            }
                        }
                    }
                }
                else if (canMoveAndFire)
                {
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        distance = GlobalUtils::getDistance(newPosition, targets[i2]);
                        if (distance >= minFireRange && distance <= maxFireRange &&
                            (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr ||
                             (targets[i2].x() == pNextEnemy->Unit::getX() && targets[i2].y() == pNextEnemy->Unit::getY())))
                        {
                            damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, targets[i2], pUnit, 0, newPosition, ignoreOutOfVisionRange);
                            break;
                        }
                    }
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
                        {
                            distance = GlobalUtils::getDistance(QPoint(pUnits->at(i3)->Unit::getX(), pUnits->at(i3)->Unit::getY()), targets[i2]);
                            if (distance >= minFireRange && distance <= maxFireRange &&
                                (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr ||
                                 pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit()->getOwner()->isAlly(m_pPlayer)) &&
                                pNextEnemy->isAttackable(pUnits->at(i3), true))
                            {
                                if (baseCosts[i3] > 0 && baseCost > 0)
                                {
                                    if (baseCost > baseCosts[i3])
                                    {
                                        // reduce damage the more units it can attack
                                        damageData.setX(damageData.x() -  damageData.x() * baseCosts[i3] / baseCost / 2);
                                    }
                                    else
                                    {
                                        damageData.setX(damageData.x() -  damageData.x() *  baseCost / baseCosts[i3] / 2);
                                    }
                                }
                            }
                        }
                    }
                }

                if (damageData.x() < 0)
                {
                    damageData.setX(0);
                }
                if (damageData.x() > 0)
                {
                    counterDamage += static_cast<qint32>(calcFundsDamage(damageData, pNextEnemy.get(), pUnit).y());
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

float NormalAi::calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, spQmlVectorBuilding pBuildings, spQmlVectorBuilding pEnemyBuildings)
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
    spQmlVectorPoint pCircle = GlobalUtils::getCircle(1, 2);
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

void NormalAi::updateEnemyData(spQmlVectorUnit pUnits)
{    
    Console::print("NormalAi::updateEnemyData", Console::eDEBUG);
    rebuildIsland(pUnits);
    if (m_EnemyUnits.size() == 0)
    {
        m_EnemyUnits = m_pPlayer->getSpEnemyUnits();
        for (qint32 i = 0; i < m_EnemyUnits.size(); i++)
        {
            m_EnemyPfs.append(spUnitPathFindingSystem::create(m_EnemyUnits[i].get()));
            m_EnemyPfs[i]->explore();
            m_VirtualEnemyData.append(QPointF(0, 0));
        }
        calcVirtualDamage(pUnits);
    }
    else
    {
        qint32 i = 0;
        while (i < m_EnemyUnits.size())
        {
            if (m_EnemyUnits[i]->getHp() <= 0 ||
                m_EnemyUnits[i]->getTerrain() == nullptr)
            {
                m_EnemyUnits.removeAt(i);
                m_EnemyPfs.removeAt(i);
                m_VirtualEnemyData.removeAt(i);
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
        for (qint32 i2 = 0; i2 < m_EnemyUnits.size(); i2++)
        {
            if (!updated.contains(i2))
            {
                if (m_EnemyUnits[i2]->getHp() > 0 && m_EnemyUnits[i2]->getTerrain() != nullptr)
                {
                    if (qAbs(m_updatePoints[i].x() - m_EnemyUnits[i2]->Unit::getX()) +
                        qAbs(m_updatePoints[i].y() - m_EnemyUnits[i2]->Unit::getY()) <=
                        m_EnemyUnits[i2]->getMovementpoints(QPoint(m_EnemyUnits[i2]->Unit::getX(), m_EnemyUnits[i2]->Unit::getY())) + 2)
                    {
                        m_EnemyPfs[i2] = spUnitPathFindingSystem::create(m_EnemyUnits[i2].get());
                        m_EnemyPfs[i2]->explore();
                    }
                    updated.push_back(i2);
                }
            }
        }
    }
    m_updatePoints.clear();
}

void NormalAi::calcVirtualDamage(spQmlVectorUnit pUnits)
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
            QVector<QVector4D> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, action, &pfs, ret, moveTargetFields);
            for (qint32 i2 = 0; i2 < ret.size(); i2++)
            {
                QPoint pos(static_cast<qint32>(ret[i2].x()), static_cast<qint32>(ret[i2].y()));
                if (!attackedUnits.contains(pos))
                {
                    attackedUnits.append(pos);
                    damage.append(ret[i2].w());
                }
            }
        }
        for (qint32 i2 = 0; i2 < attackedUnits.size(); i2++)
        {
            for (qint32 i3 = 0; i3 < m_EnemyUnits.size(); i3++)
            {
                if (m_EnemyUnits[i3]->Unit::getX() == attackedUnits[i2].x() &&
                    m_EnemyUnits[i3]->Unit::getY() == attackedUnits[i2].y())
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
    m_EnemyPfs.clear();
}

bool NormalAi::buildUnits(spQmlVectorBuilding pBuildings, spQmlVectorUnit pUnits,
                          spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    Console::print("NormalAi::buildUnits()", Console::eDEBUG);
    if (aiStep < AISteps::buildUnits)
    {
        m_productionData.clear();
    }
    aiStep = AISteps::buildUnits;
    spGameMap pMap = GameMap::getInstance();

    qint32 enemeyCount = 0;
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        if (m_pPlayer->isEnemy(pMap->getPlayer(i)) && !pMap->getPlayer(i)->getIsDefeated())
        {
            enemeyCount++;
        }
    }

    QVector<float> data(BuildItems::Max, 0);
    qint32 productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pMap->getTerrain(pBuilding->Building::getX(), pBuilding->Building::getY())->getUnit() == nullptr)
        {
            productionBuildings++;
        }
    }
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    QVector<std::tuple<Unit*, Unit*>> transportTargets;
    GetOwnUnitCounts(pUnits, pEnemyUnits, pEnemyBuildings,
                     infantryUnits, indirectUnits, directUnits,
                     transportTargets);
    QVector<QVector4D> attackCount(pEnemyUnits->size(), QVector4D(0, 0, 0, 0));
    getEnemyDamageCounts(pUnits, pEnemyUnits, attackCount);
    float funds = m_pPlayer->getFunds();
    // calc average costs if we would build same cost units on every building
    float fundsPerFactory = funds / (static_cast<float>(productionBuildings));
    if (productionBuildings > 2)
    {
        // if we have a bigger number of buildings we wanna spam out units but not at an average costs overall buildings
        // but more a small amount of strong ones and a large amount of cheap ones
        // so we use a small (x - a) / (x - b) function here
        float test = funds * ((productionBuildings - m_fundsPerBuildingFactorA) / (static_cast<float>(productionBuildings) - m_fundsPerBuildingFactorB));
        if (test > m_spamingFunds)
        {
            test = m_spamingFunds;
        }
        if (test > fundsPerFactory)
        {
            fundsPerFactory = test;
        }
    }
    // position 0 direct to indirect ratio
    if (indirectUnits > 0)
    {
        data[DirectUnitRatio] = static_cast<float>(directUnits) / static_cast<float>(indirectUnits);
    }
    else
    {
        data[DirectUnitRatio] = static_cast<float>(directUnits);
    }
    // position 1 infatry to unit count ratio
    if (pUnits->size() > 0)
    {
        data[InfantryUnitRatio] = infantryUnits / static_cast<float>(pUnits->size());
    }
    else
    {
        data[InfantryUnitRatio] = 0.0;
    }
    data[InfantryCount] = infantryUnits;
    data[UnitEnemyRatio] = (pUnits->size() + m_ownUnitEnemyUnitRatioAverager) / (pEnemyUnits->size() + m_ownUnitEnemyUnitRatioAverager);
    if (enemeyCount > 1)
    {
        data[UnitEnemyRatio] *= (enemeyCount - 1);
    }
    data[UnitCount] = pUnits->size();

    spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS);
    float bestScore = std::numeric_limits<float>::lowest();
    QVector<qint32> buildingIdx;
    QVector<qint32> unitIDx;
    QVector<float> scores;
    QVector<bool> transporters;
    float variance = pMap->getCurrentDay() - 1;
    if (variance > m_maxDayScoreVariancer)
    {
        variance = m_maxDayScoreVariancer;
    }
    spQmlVectorPoint pFields = GlobalUtils::getCircle(1, 1);
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
                                                                  pUnits, transportTargets,
                                                                  pEnemyUnits, pEnemyBuildings,
                                                                  attackCount, data);
                            bool isTransporter = false;
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
                                    data[DamageData] =  unitData.damage;
                                    data[FundsFactoryRatio] = unitData.cost / fundsPerFactory;
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

                                        score = calcBuildScore(data);
                                    }
                                    else
                                    {
                                        score = calcTransporterScore(unitData, pUnits, data);
                                        isTransporter = true;
                                    }
                                }
                            }
                            score *= BaseGameInputIF::getUnitBuildValue(actionIds[i2]);
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
        spMenuData pData = pAction->getMenuStepData();
        if (pData->validData())
        {
            CoreAI::addMenuItemData(pAction, pData->getActionIDs()[unitIDx[item]], pData->getCostList()[unitIDx[item]]);
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
    Console::print("NormalAi::getIndexInProductionData()", Console::eDEBUG);
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
    }
    return ret;
}

qint32 NormalAi::getUnitProductionIdx(qint32 index, QString unitId,
                                      spQmlVectorUnit pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                      spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                      QVector<QVector4D> & attackCount, QVector<float> & buildData)
{
    Console::print("NormalAi::getUnitProductionIdx()", Console::eDEBUG);
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
                auto damageData = calcExpectedFundsDamage(data.m_x, data.m_y, dummy, pEnemyUnits, attackCount, bonusFactor);
                unitData.notAttackableCount = std::get<1>(damageData);
                unitData.damage =  std::get<0>(damageData);
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
                            attackCount, buildData);
    }
    return ret;
}

void NormalAi::createUnitBuildData(qint32 x, qint32 y, UnitBuildData & unitBuildData,
                                   spQmlVectorUnit pUnits, QVector<std::tuple<Unit*, Unit*>> & transportTargets,
                                   spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                   QVector<QVector4D> & attackCount, QVector<float> & buildData)
{
    Console::print("NormalAi::createUnitBuildData()", Console::eDEBUG);
    Unit dummy(unitBuildData.unitId, m_pPlayer, false);
    dummy.setVirtuellX(x);
    dummy.setVirtuellY(y);
    createIslandMap(dummy.getMovementType(), dummy.getUnitID());
    UnitPathFindingSystem pfs(&dummy, m_pPlayer);
    pfs.explore();
    unitBuildData.movePoints = dummy.getMovementpoints(QPoint(x, y));
    auto points = pfs.getAllNodePoints();
    if (points.length() >= unitBuildData.movePoints * 1.5f)
    {
        unitBuildData.canMove = true;
        unitBuildData.isTransporter = (dummy.getWeapon1ID().isEmpty() &&
                                       dummy.getWeapon2ID().isEmpty());
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
            auto damageData = calcExpectedFundsDamage(x, y, dummy, pEnemyUnits, attackCount, bonusFactor);
            unitBuildData.notAttackableCount = std::get<1>(damageData);
            unitBuildData.damage =  std::get<0>(damageData);
            unitBuildData.cost = dummy.getUnitCosts();
            unitBuildData.infantryUnit = (dummy.getActionList().contains(ACTION_CAPTURE) &&
                                          dummy.getLoadingPlace() == 0);
            unitBuildData.indirectUnit = dummy.getBaseMaxRange() > 1;
        }
    }
}

void NormalAi::getEnemyDamageCounts(spQmlVectorUnit pUnits, spQmlVectorUnit pEnemyUnits, QVector<QVector4D> & attackCount)
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

            if ((dmg1 > m_notAttackableDamage || dmg2 > m_notAttackableDamage) &&
                pEnemyUnit->getMovementpoints(QPoint(pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY())) - pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())) < 2)
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

qint32 NormalAi::getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
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

std::tuple<float, qint32> NormalAi::calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, spQmlVectorUnit pEnemyUnits, QVector<QVector4D> attackCount, float bonusFactor)
{
    Console::print("NormalAi::calcExpectedFundsDamage() find enemies", Console::eDEBUG);
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
    qint32 notAttackableCount = 0;
    float damageCount = 0;
    float attacksCount = 0;
    float myMovepoints = dummy.getBaseMovementPoints();
    if (myMovepoints == 0)
    {
        myMovepoints = 1;
    }
    spGameMap pMap = GameMap::getInstance();
    qint32 maxCounter = pMap->getMapWidth() * pMap->getMapHeight() / (myMovepoints * 2);
    float myFirerange = dummy.getBaseMaxRange();
    float enemyFirerange = dummy.getBaseMaxRange();
    QPoint position = dummy.getPosition();
    qint32 counter = 1;
    while (attacksCount  < m_maxBuildingTargetFindLoops &&
           pEnemyUnits->size() > m_maxBuildingTargetFindLoops &&
           counter <= maxCounter)
    {
        for (qint32 i3 = 0; i3 < pEnemyUnits->size(); i3++)
        {
            Unit* pEnemyUnit = pEnemyUnits->at(i3);
            QPoint enemyPosition = pEnemyUnit->getPosition();
            float distance = GlobalUtils::getDistance(position, enemyPosition);
            if (distance / myMovepoints <= counter &&
                distance / myMovepoints >= (counter - 1))
            {
                float dmg = 0.0f;
                if (!dummy.getWeapon1ID().isEmpty())
                {
                    dmg = pWeaponManager->getBaseDamage(dummy.getWeapon1ID(), pEnemyUnit);
                }
                if (!dummy.getWeapon2ID().isEmpty())
                {
                    float dmg2 = pWeaponManager->getBaseDamage(dummy.getWeapon2ID(), pEnemyUnit);
                    if (dmg2 > dmg)
                    {
                        dmg = dmg2;
                    }
                }
                if (dmg > pEnemyUnit->getHp() * Unit::MAX_UNIT_HP)
                {
                    dmg = pEnemyUnit->getHp() * Unit::MAX_UNIT_HP;
                }
                if (dmg > 0.0f)
                {
                    float counterDmg = 0;
                    if (!pEnemyUnit->getWeapon1ID().isEmpty())
                    {
                        counterDmg = pWeaponManager->getBaseDamage(pEnemyUnit->getWeapon1ID(), &dummy);
                    }
                    if (!pEnemyUnit->getWeapon2ID().isEmpty())
                    {
                        float dmg2 = pWeaponManager->getBaseDamage(pEnemyUnit->getWeapon2ID(), &dummy);
                        if (dmg2 > counterDmg)
                        {
                            counterDmg = dmg2;
                        }
                    }
                    if (counterDmg > m_scoringCutOffDamageHigh)
                    {
                        counterDmg = m_scoringCutOffDamageHigh;
                    }
                    else if (counterDmg < m_scoringCutOffDamageLow)
                    {
                        counterDmg = 0.0f;
                    }
                    float resDamage = 0;

                    float enemyMovepoints = pEnemyUnit->getBaseMovementPoints();
                    if (myMovepoints + myFirerange >= enemyMovepoints)
                    {
                        float mult = (myMovepoints + myFirerange + m_smoothingValue) / (enemyMovepoints + enemyFirerange + m_smoothingValue);
                        if (mult > m_maxDistanceMultiplier)
                        {
                            mult = m_maxDistanceMultiplier;
                        }
                        if (myFirerange > 1)
                        {
                            // increased bonus for indirects since they should be attacked less often and attack more often
                            mult += 1.0f;
                        }
                        resDamage = dmg / (pEnemyUnit->getHp() * Unit::MAX_UNIT_HP) * pEnemyUnit->getUnitValue() * mult * bonusFactor -
                                    counterDmg / Unit::DAMAGE_100 * pEnemyUnit->getUnitValue();
                    }
                    else
                    {
                        float mult = (enemyMovepoints + enemyFirerange + m_smoothingValue) / (myMovepoints + myFirerange + m_smoothingValue);
                        if (mult > m_maxDistanceMultiplier)
                        {
                            mult = m_maxDistanceMultiplier;
                        }
                        resDamage = dmg / (pEnemyUnit->getHp() * Unit::MAX_UNIT_HP) * pEnemyUnit->getUnitValue() * bonusFactor -
                                    counterDmg / Unit::DAMAGE_100 * pEnemyUnit->getUnitValue() * mult;
                    }
                    if (resDamage > pEnemyUnit->getUnitValue())
                    {
                        resDamage = pEnemyUnit->getUnitValue();
                    }
                    float factor = 1.0f;
                    if (dmg > m_highDamage)
                    {
                        factor += (attackCount[i3].w() + m_smoothingValue) / (attackCount[i3].x() + m_smoothingValue);
                    }
                    else if (dmg > m_midDamage)
                    {
                        factor += (attackCount[i3].z() + m_smoothingValue) / (attackCount[i3].z() + m_smoothingValue);
                    }
                    if (onSameIsland(dummy.getMovementType(), posX, posY, pEnemyUnit->Unit::getX(), pEnemyUnit->Unit::getY()))
                    {
                        factor += (m_sameIslandBonusInRangeDays - (distance / static_cast<float>(myMovepoints) * m_sameIslandOutOfDayMalusFactor));
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
                                notAttackableCount += notAttackableValue;
                            }
                            else if (attackCount[i3].y() == 0.0f &&
                                     attackCount[i3].z() == 0.0f &&
                                     attackCount[i3].w() == 0.0f)
                            {
                                notAttackableCount  += notAttackableValue * m_currentlyNotAttackableBonus;
                            }
                        }
                        else
                        {
                            factor *= m_transportBonus;
                        }
                    }
                    else
                    {
                        factor += (m_differentIslandBonusInRangeDays - (distance / static_cast<float>(myMovepoints) * m_differentIslandOutOfDayMalusFactor));
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
                    }
                    if (factor < 0)
                    {
                        factor = 0;
                    }
                    damageCount += resDamage * factor;
                    attacksCount++;
                }
            }
        }
        counter++;
    }
    Console::print("NormalAi::calcExpectedFundsDamage() calc damage", Console::eDEBUG);
    if (attacksCount <= 0)
    {
        attacksCount = 1;
    }
    float damage = damageCount / attacksCount;
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
    return std::tuple<float, qint32>(damage, notAttackableCount);
}

void NormalAi::getTransporterData(UnitBuildData & unitBuildData, Unit& dummy, spQmlVectorUnit pUnits,
                                  spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings,
                                  QVector<std::tuple<Unit*, Unit*>>& transportTargets)
{
    Console::print("NormalAi::getTransporterData()", Console::eDEBUG);
    QVector<QVector3D> targets;
    spQmlVectorUnit relevantUnits = spQmlVectorUnit::create();
    QPoint position = dummy.getPosition();
    qint32 movement = dummy.getBaseMovementPoints();
    if (movement == 0)
    {
        movement = 1;
    }
    spGameMap pMap = GameMap::getInstance();
    qint32 loadingPlace = dummy.getLoadingPlace();
    qint32 smallTransporterCount = 0;
    qint32 maxCounter = m_ProducingTransportSearchrange;
    qint32 counter = 1;
    while (relevantUnits->size()  < loadingPlace * m_transporterToRequiredPlaceFactor &&
           pUnits->size() > loadingPlace * (m_transporterToRequiredPlaceFactor - 1) &&
           counter <= maxCounter)
    {
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            qint32 distance = GlobalUtils::getDistance(position, pUnits->at(i)->getPosition());
            if (distance / movement <= counter * maxDayDistance &&
                distance / movement >= (counter - 1) * maxDayDistance)
            {
                relevantUnits->append(pUnits->at(i));
            }
            if (loadingPlace == 1)
            {
                if (pUnits->at(i)->getLoadingPlace() == 1)
                {
                    smallTransporterCount++;
                }
            }
        }
        counter++;
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
    unitBuildData.loadingPlace = dummy.getLoadingPlace();
    unitBuildData.transportCount = transporterUnits.size();
    unitBuildData.loadingCount = loadingUnits.size();
    unitBuildData.flying = !dummy.useTerrainDefense();
}

float NormalAi::calcTransporterScore(UnitBuildData & unitBuildData,  spQmlVectorUnit pUnits, QVector<float>& data)
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
        if (unitBuildData.flying && score > m_minFlyingTransportScoreForBonus)
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
    if (unitBuildData.loadingCount > 0 || unitBuildData.loadingCount == 1)
    {
        if (unitBuildData.transportCount <= 0 ||
            static_cast<float>(unitBuildData.loadingCount) / static_cast<float>(unitBuildData.transportCount) < m_ProducingTransportMinLoadingTransportRatio ||
            unitBuildData.loadingCount == 1)
        {
            score += calcCostScore(data);
            score += unitBuildData.loadingPlace * m_additionalLoadingUnitBonus;
            score += unitBuildData.loadingCount * m_additionalLoadingUnitBonus;
        }
    }
    else
    {
        score = std::numeric_limits<float>::lowest();
    }
    return score;
}

bool NormalAi::canTransportToEnemy(Unit* pUnit, Unit* pLoadedUnit, spQmlVectorUnit pEnemyUnits, spQmlVectorBuilding pEnemyBuildings)
{
    spQmlVectorPoint pUnloadArea = GlobalUtils::getCircle(1, 1);
    // check for enemis
    qint32 loadedUnitIslandIdx = getIslandIndex(pLoadedUnit);
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    QVector<qint32> checkedIslands;
    QVector<QVector3D> targets;
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemy = pEnemyUnits->at(i);
        qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx]->getIsland(pEnemy->Unit::getX(), pEnemy->Unit::getY());
        // check if we could reach the enemy if we would be on his island
        // and we didn't checked this island yet -> improves the speed
        if (targetIsland >= 0 )
        {
            // could we beat his ass? -> i mean can we attack him
            // if so this is a great island
            if (pLoadedUnit->isAttackable(pEnemy, true))
            {
                checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands, unitIslandIdx, unitIsland,
                                        loadedUnitIslandIdx, targetIsland, pUnloadArea.get(), targets);
                if (targets.size() > 0)
                {
                    break;
                }
            }
        }
    }
    // check for capturable buildings
    if (pLoadedUnit->getActionList().contains(ACTION_CAPTURE) && targets.size() == 0)
    {
        bool missileTarget = hasMissileTarget();
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pEnemyBuilding = pEnemyBuildings->at(i);

            qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx]->getIsland(pEnemyBuilding->Building::getX(), pEnemyBuilding->Building::getY());
            // check if we could reach the enemy if we would be on his island
            // and we didn't checked this island yet -> improves the speed
            if (targetIsland >= 0 )
            {
                if (pEnemyBuilding->isCaptureOrMissileBuilding(missileTarget))
                {
                    checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands, unitIslandIdx, unitIsland,
                                            loadedUnitIslandIdx, targetIsland, pUnloadArea.get(), targets);
                    if (targets.size() > 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    if (targets.size() > 0)
    {
        return true;
    }
    return false;
}

float NormalAi::calcBuildScore(QVector<float>& data)
{
    float score = 0;
    // used index 0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11
    if (data[IndirectUnit] == 1.0f)
    {
        // indirect unit
        if (data[DirectUnitRatio] > m_directIndirectRatio)
        {
            score += m_lowIndirectUnitBonus * (data[DirectUnitRatio] - m_directIndirectRatio);
        }
        else if (data[DirectUnitRatio] < m_directIndirectRatio / 2)
        {
            score -= m_highIndirectMalus * (m_directIndirectRatio - data[DirectUnitRatio]);
        }
        else if (data[DirectUnitRatio] < m_directIndirectRatio)
        {
            score -= m_lowIndirectMalus * (m_directIndirectRatio - data[DirectUnitRatio]);
        }
    }
    else if (data[DirectUnit] == 1.0f)
    {
        // direct unit
        if (data[DirectUnitRatio] < m_directIndirectRatio)
        {
            score += m_lowDirectUnitBonus * (m_directIndirectRatio - data[DirectUnitRatio]);
        }
        else if (data[DirectUnitRatio] > m_directIndirectRatio * 2)
        {
            score -= m_highDirectMalus * (data[DirectUnitRatio] - m_directIndirectRatio);
        }
        else if (data[DirectUnitRatio] > m_directIndirectRatio)
        {
            score -= m_lowDirectMalus * (data[DirectUnitRatio] - m_directIndirectRatio);
        }
    }
    if (data[UnitCount] > m_minUnitCountForDamageBonus)
    {
        // apply damage bonus
        score += data[DamageData] / Unit::DAMAGE_100;
    }
    // infantry bonus
    if (data[InfantryUnit] == 1.0f)
    {
        if (data[InfantryCount] <= m_minInfantryCount && data[BuildingEnemyRatio] < m_lowOwnBuildingEnemyBuildingRatio)
        {
            score += (m_minInfantryCount - data[InfantryCount]) * m_minInfantryCount + (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_lowIncomeInfantryBonusMultiplier;
        }
        else if (data[InfantryUnitRatio] < m_lowInfantryRatio)
        {
            score += (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_buildingBonusMultiplier;
        }
        else
        {
            score += (m_lowOwnBuildingEnemyBuildingRatio - data[BuildingEnemyRatio]) * m_buildingBonusMultiplier;
        }
        if (data[InfantryCount] >= m_minInfantryCount)
        {
            score -= data[InfantryCount];
        }
    }
    score += calcCostScore(data);
    // apply movement bonus
    score += data[Movementpoints] * m_movementpointBonus;
    if (data[UnitCount] > m_minUnitCountForDamageBonus)
    {
        // apply not attackable unit bonus
        score += data[NotAttackableCount] * m_currentlyNotAttackableScoreBonus;
    }
    if (data[UnitCost] > 0)
    {
        score += data[DamageData] * m_damageToUnitCostRatioBonus / data[UnitCost];
    }
    // apply co buff bonus
    score += data[COBonus] * m_coUnitBuffBonus;

    if (data[ReachDistance] > 0 && data[Movementpoints] > 0)
    {
        score += m_nearEnemyBonus * data[Movementpoints] / GlobalUtils::roundUp(data[ReachDistance]);
    }
    return score;
}

float NormalAi::calcCostScore(QVector<float>& data)
{
    float score = 0;
    // funds bonus;
    if (data[FundsFactoryRatio] > m_superiorityRatio + data[UnitEnemyRatio])
    {
        score -= (data[FundsFactoryRatio] - m_superiorityRatio + data[UnitEnemyRatio]) * m_expensiveUnitBonusMultiplier;
    }
    else if (data[FundsFactoryRatio] < m_cheapUnitRatio)
    {
        score += (m_cheapUnitRatio - data[FundsFactoryRatio]) * m_cheapUnitBonusMultiplier;
    }
    else
    {
        score += (m_superiorityRatio + data[UnitEnemyRatio] - data[FundsFactoryRatio]) * m_normalUnitBonusMultiplier;
    }
    return score;
}
