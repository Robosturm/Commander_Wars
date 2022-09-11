#include <QSettings>
#include <QCoreApplication>

#include "ai/veryeasyai.h"
#include "ai/islandmap.h"
#include "ai/targetedunitpathfindingsystem.h"

#include "game/gameaction.h"
#include "game/player.h"
#include "game/co.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"

#include "coreengine/globalutils.h"
#include "coreengine/console.h"

VeryEasyAI::VeryEasyAI(GameMap* pMap)
    : CoreAI(pMap, GameEnums::AiTypes_VeryEasy),
      m_GeneralBuildingTree("resources/aidata/very_easy/generalbuilding.tree", "resources/aidata/very_easy/generalbuilding.txt"),
      m_AirportBuildingTree("resources/aidata/very_easy/airportbuilding.tree", "resources/aidata/very_easy/airportbuilding.txt"),
      m_HarbourBuildingTree("resources/aidata/very_easy/harbourbuilding.tree", "resources/aidata/very_easy/harbourbuilding.txt")
{
#ifdef GRAPHICSUPPORT
    setObjectName("VeryEasyAI");
#endif
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

    m_iniData = { // General
                  {"OwnUnitValue", "General", &m_ownUnitValue, 1.0f, -10.0f, 10.0f},
                  {"BuildingValue", "General", &m_buildingValue, 1.0f, 1.0f, 1.0f},
                  {"MinDamage", "General", &m_minDamage, -500, -5000.0f, 5000.0f},
                  {"MinSiloDamage", "General", &m_minDamage, 4000, 1000.0f, 4000.0f},
                  {"OwnUnitDamageDivider", "General", &m_ownUnitDamageDivider, 4, 0.0f, 10.0f},
                  {"MinAllBuildingFunds", "General", &m_minAllBuildingFunds, 8000, 8000.0f, 8000.0f},
                  {"MaxTreeDecisionTries", "General", &m_maxTreeDecisionTries, 10, 1.0f, 20.0f},
                  {"FuelResupply", "General", &m_fuelResupply, 0.33f, 0.33f, 0.33f},
                  {"AmmoResupply", "General", &m_ammoResupply, 0.25f, 0.25f, 0.25f},                  // CO Unit
                  {"CoUnitValue", "CoUnit", &m_coUnitValue, 6000.0f, 5000.0f, 10000.0f},
                  {"MinCoUnitScore", "CoUnit", &m_minCoUnitScore, 5000.0f, 3000.0f, 10000.0f},
                  {"CoUnitRankReduction", "CoUnit", &m_coUnitRankReduction, 1000.0f, 0.0f, 5000.0f},
                  {"CoUnitScoreMultiplier", "CoUnit", &m_coUnitScoreMultiplier, 1.1f, 1.0f, 3.0f},
                  {"MinCoUnitCount", "CoUnit", &m_minCoUnitCount, 5.0f, 1.0f, 10.0f},
                  {"MinSameIslandDistance", "General", &m_minSameIslandDistance, 3.0f, 3.0f, 3.0f},
                  {"SlowUnitSpeed", "General", &m_slowUnitSpeed, 2.0f, 2.0f, 2.0f},
                };
    
    if (m_pMap != nullptr &&
        !m_pMap->getSavegame())
    {
        loadIni("very_easy/very_easy.ini");
    }
}

void VeryEasyAI::process()
{
    AI_CONSOLE_PRINT("VeryEasyAI::process()", Console::eDEBUG);
    spQmlVectorBuilding pBuildings = spQmlVectorBuilding(m_pPlayer->getBuildings());
    pBuildings->randomize();
    spQmlVectorUnit pUnits = spQmlVectorUnit(m_pPlayer->getUnits());
    pUnits->randomize();

    spQmlVectorUnit pEnemyUnits = spQmlVectorUnit(m_pPlayer->getEnemyUnits());
    pEnemyUnits->randomize();
    spQmlVectorBuilding pEnemyBuildings = spQmlVectorBuilding(m_pPlayer->getEnemyBuildings());
    pEnemyBuildings->randomize();

    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= m_minSiloDamage);
    // create island maps at the start of turn
    if (rebuildIslandMaps)
    {
        rebuildIslandMaps = false;
        // remove island maps of the last turn
        m_IslandMaps.clear();
    }
    rebuildIsland(pUnits);

    // make the ai do stuff
    if (useCOPower(pUnits, pEnemyUnits))
    {
        m_usedPredefinedAi = false;
    }
    else
    {
        turnMode = GameEnums::AiTurnMode_DuringDay;
        if (useBuilding(pBuildings)){}
        else if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
        else
        {
            m_aiStep = AISteps::moveUnits;
            if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
            else
            {
                m_aiStep = AISteps::moveUnits;
                turnMode = GameEnums::AiTurnMode_EndOfDay;
                if (useCOPower(pUnits, pEnemyUnits))
                {
                    m_usedTransportSystem = false;
                    m_usedPredefinedAi = false;
                    turnMode = GameEnums::AiTurnMode_DuringDay;
                }
                else
                {
                    finishTurn();
                }
            }
        }
    }
}

bool VeryEasyAI::performActionSteps(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits,
                                    spQmlVectorBuilding & pBuildings, spQmlVectorBuilding & pEnemyBuildings)
{
    if (m_aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveFlares(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && captureBuildings(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    else if (m_aiStep <= AISteps::moveUnits && fireWithIndirectUnits(pUnits)){}
    else if (m_aiStep <= AISteps::moveUnits && fireWithDirectUnits(pUnits)){}
    else if (m_aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings)){}
    else if (m_aiStep <= AISteps::loadUnits  && !m_usedTransportSystem && loadUnits(pUnits)){}
    else if (m_aiStep <= AISteps::moveTransporters  && !m_usedTransportSystem && moveTransporters(pUnits, pEnemyUnits, pEnemyBuildings)){}
    else
    {
        if (!m_usedTransportSystem)
        {
            m_usedTransportSystem = true;
            m_aiStep = AISteps::moveUnits;
            return performActionSteps(pUnits, pEnemyUnits,  pBuildings, pEnemyBuildings);
        }
        else if (m_aiStep <= AISteps::loadUnits && loadUnits(pUnits)){}
        else if (m_aiStep <= AISteps::moveSupportUnits && moveSupport(AISteps::moveSupportUnits, pUnits, true)){}
        else if (m_aiStep <= AISteps::moveSupportUnits && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, true)){}
        else if (m_aiStep <= AISteps::moveAway && moveAwayFromProduction(pUnits)){}
        else if (m_aiStep <= AISteps::buildUnits && buildUnits(pBuildings, pUnits)){}
        else
        {
            return false;
        }
    }
    return true;
}

void VeryEasyAI::finishTurn()
{
    turnMode = GameEnums::AiTurnMode_StartOfDay;
    rebuildIslandMaps = true;
    CoreAI::finishTurn();
}

bool VeryEasyAI::captureBuildings(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("VeryEasyAI::captureBuildings()", Console::eDEBUG);
    qint32 cost = 0;
    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
    bool fireSilos = (cost >= m_minSiloDamage);

    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        QCoreApplication::processEvents();
        if (!pUnit->getHasMoved() &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            if (pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                if (pUnit->getCapturePoints() > 0)
                {
                    spGameAction pAction = spGameAction::create(ACTION_CAPTURE, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                }
                else
                {
                    spGameAction pAction = spGameAction::create(ACTION_CAPTURE, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    UnitPathFindingSystem pfs(m_pMap, pUnit);
                    pfs.explore();
                    auto targets = pfs.getAllNodePointsFast();
                    for (auto & target : targets)
                    {
                        if (pUnit->Unit::getX() == target.x() &&
                            pUnit->Unit::getY() == target.y())
                        {
                            pAction->setMovepath(QVector<QPoint>(), 0);
                        }
                        else
                        {
                            auto path = pfs.getPathFast(target.x(), target.y());
                            pAction->setMovepath(path, pfs.getCosts(path));
                        }
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                    // try to fire a missile instead
                    pAction->setActionID(ACTION_MISSILE);
                    if (fireSilos)
                    {
                        for (auto & target : targets)
                        {
                            if (pUnit->Unit::getX() == target.x() &&
                                pUnit->Unit::getY() == target.y())
                            {
                                pAction->setMovepath(QVector<QPoint>(), 0);
                            }
                            else
                            {
                                auto path = pfs.getPathFast(target.x(), target.y());
                                pAction->setMovepath(path, pfs.getCosts(path));
                            }
                            if (pAction->canBePerformed())
                            {
                                // select a target for the rocket and launch it
                                CoreAI::addSelectedFieldData(pAction, rocketTarget);
                                emit performAction(pAction);
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool VeryEasyAI::fireWithIndirectUnits(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("VeryEasyAI::fireWithIndirectUnits()", Console::eDEBUG);
    for (auto & pUnit : pUnits->getVector())
    {
        QCoreApplication::processEvents();
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getBaseMaxRange() > 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0) &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            if (attack(pUnit.get()))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::fireWithDirectUnits(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("VeryEasyAI::fireWithDirectUnits()", Console::eDEBUG);
    for (auto & pUnit : pUnits->getVector())
    {
        QCoreApplication::processEvents();
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getBaseMaxRange() == 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0) &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            if (attack(pUnit.get()))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::attack(Unit* pUnit)
{
    AI_CONSOLE_PRINT("VeryEasyAI::attack", Console::eDEBUG);
    if (pUnit->hasAction(CoreAI::ACTION_FIRE))
    {
        // try to perform an attack
        spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        UnitPathFindingSystem pfs(m_pMap, pUnit);
        pfs.explore();
        std::vector<QVector3D> ret;
        std::vector<QVector3D> moveTargetFields;
        CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
        float minDamage = -pUnit->getCoUnitValue() / m_ownUnitDamageDivider;
        if (minDamage > m_minDamage)
        {
            minDamage = m_minDamage;
        }
        if (ret.size() > 0 && ret[0].z() >= minDamage)
        {
            qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
            QVector3D target = ret[selection];
            QPoint point = pAction->getTarget();
            if (static_cast<qint32>(moveTargetFields[selection].x()) != point.x() ||
                static_cast<qint32>(moveTargetFields[selection].y()) != point.y())
            {
                auto path = pfs.getPathFast(static_cast<qint32>(moveTargetFields[selection].x()),
                                                   static_cast<qint32>(moveTargetFields[selection].y()));
                pAction->setMovepath(path, pfs.getCosts(path));
            }
            else
            {
                pAction->setMovepath(QVector<QPoint>(), 0);
            }
            CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
            // attacing none unit targets may modify the islands for a unit -> rebuild all for the love of god
            if (m_pMap->getTerrain(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()))->getUnit() == nullptr)
            {
                rebuildIslandMaps = true;
            }

            if (pAction->isFinalStep() && pAction->canBePerformed())
            {
                emit performAction(pAction);
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::moveUnits(spQmlVectorUnit & pUnits, spQmlVectorBuilding & pBuildings,
                           spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings,
                           bool supportUnits )
{
    AI_CONSOLE_PRINT("VeryEasyAI::moveUnits()", Console::eDEBUG);
    if (m_aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    m_aiStep = AISteps::moveToTargets;
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        QCoreApplication::processEvents();
        // can we use the unit?
        if (!pUnit->getHasMoved() &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            std::vector<QVector3D> targets;
            std::vector<QVector3D> transporterTargets;
            spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));

            // find some cool targets
            appendCaptureTargets(actions, pUnit, pEnemyBuildings, targets);
            if (targets.size() > 0)
            {
                appendCaptureTransporterTargets(pUnit, pUnits, pEnemyBuildings, transporterTargets);
                targets.insert(targets.cend(), transporterTargets.cbegin(), transporterTargets.cend());
            }
            appendAttackTargets(pUnit, pEnemyUnits, targets);
            appendAttackTargetsIgnoreOwnUnits(pUnit, pEnemyUnits, targets);
            appendTerrainBuildingAttackTargets(pUnit, pEnemyBuildings, targets);
            if (targets.size() == 0)
            {
                if ((pUnit->getMaxAmmo1() > 0 && !pUnit->hasAmmo1()) ||
                    (pUnit->getMaxAmmo2() > 0 && !pUnit->hasAmmo2()) ||
                    (pUnit->getMaxFuel() > 0 && static_cast<float>(pUnit->getFuel()) / static_cast<float>(pUnit->getMaxFuel()) < m_fuelResupply))
                {
                    appendRepairTargets(pUnit, pBuildings, targets);
                }
            }
            // force resupply when low on fuel
            else if (static_cast<float>(pUnit->getFuel()) / static_cast<float>(pUnit->getMaxFuel()) < m_fuelResupply)
            {
                targets.clear();
                appendRepairTargets(pUnit, pBuildings, targets);
            }
            if (supportUnits)
            {
                appendSupportTargets(actions, pUnit, pUnits, pEnemyUnits, targets);
            }
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}


bool VeryEasyAI::moveTransporters(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    AI_CONSOLE_PRINT("VeryEasyAI::moveTransporters()", Console::eDEBUG);
    m_aiStep = AISteps::moveTransporters;
    for (auto & spUnit : pUnits->getVector())
    {
        QCoreApplication::processEvents();
        Unit* pUnit = spUnit.get();
        // can we use the unit?
        if (!pUnit->getHasMoved() &&
            pUnit->getLoadingPlace() > 0 &&
            pUnit->getAiMode() == GameEnums::GameAi_Normal)
        {
            // wooohooo it's a transporter
            if (pUnit->getLoadedUnitCount() > 0)
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                QStringList actions = pUnit->getActionList();
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
                        appendUnloadTargetsForCapturing(pUnit, pEnemyBuildings, targets);
                        captureFound = true;
                    }
                    if (!attackFound && actions.contains(ACTION_FIRE))
                    {
                        appendUnloadTargetsForAttacking(pUnit, pEnemyUnits, targets, 1);
                        attackFound = true;
                    }
                    if (attackFound && captureFound)
                    {
                        break;
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
                if (moveUnit(pAction, pUnit, actions, targets, targets, true))
                {
                    return true;
                }
            }
            else
            {
                spGameAction pAction = spGameAction::create(ACTION_WAIT, m_pMap);
                QStringList actions = pUnit->getActionList();
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                // we need to move to a loading place
                std::vector<QVector3D> targets;
                std::vector<QVector3D> transporterTargets;
                appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, false, targets);
                if (targets.size() == 0)
                {
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, false, targets);
                }
                if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
                {
                    return true;
                }
            }
        }
    }
    return false;
}


bool VeryEasyAI::loadUnits(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("VeryEasyAI::loadUnits()", Console::eDEBUG);
    m_aiStep = AISteps::loadUnits;
    for (auto & pUnit : pUnits->getVector())
    {
        QCoreApplication::processEvents();
        // can we use the unit?
        if (!pUnit->getHasMoved())
        {
            std::vector<QVector3D> targets;
            std::vector<QVector3D> transporterTargets;
            spGameAction pAction = spGameAction::create(ACTION_LOAD, m_pMap);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));

            // find some cool targets
            appendTransporterTargets(pUnit.get(), pUnits, transporterTargets);
            targets.insert(targets.end(), transporterTargets.begin(), transporterTargets.end());
            // till now the selected targets are a little bit lame cause we only search for reachable transporters
            // but not for reachable loading places.
            if (moveUnit(pAction, pUnit.get(), actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::moveUnit(spGameAction & pAction, Unit* pUnit, QStringList& actions,
                          std::vector<QVector3D>& targets, std::vector<QVector3D>& transporterTargets, bool unload)
{
    AI_CONSOLE_PRINT("VeryEasyAI::moveUnit()", Console::eDEBUG);
    TargetedUnitPathFindingSystem pfs(m_pMap, pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(m_pMap, pUnit);
        turnPfs.explore();

        if (CoreAI::contains(transporterTargets, targetFields))
        {
            if (unload)
            {
                auto path = turnPfs.getPathFast(targetFields.x(), targetFields.y());
                pAction->setMovepath(path, turnPfs.getCosts(path));
                pAction->setActionID(ACTION_UNLOAD);
                // this isn't ideal since we unload the units in a random order and not the best fitting order
                // but we're lazy here for the very easy ai
                while (true)
                {
                    spMenuData pDataMenu = pAction->getMenuStepData();
                    if (pDataMenu->validData())
                    {
                        QStringList actions = pDataMenu->getActionIDs();
                        qint32 costs = pDataMenu->getCostList()[0];
                        if (actions.size() == 1)
                        {
                            break;
                        }
                        else
                        {
                            addMenuItemData(pAction, actions[0], costs);
                        }
                        spMarkedFieldData pFields = pAction->getMarkedFieldStepData();
                        qint32 field = GlobalUtils::randIntBase(0, pFields->getPoints()->size() - 1);
                        addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                    }
                    else
                    {
                        return false;
                    }
                }
                addMenuItemData(pAction, ACTION_WAIT, 0);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            else
            {
                auto path = turnPfs.getPathFast(targetFields.x(), targetFields.y());
                pAction->setMovepath(path, turnPfs.getCosts(path));
                pAction->setActionID(ACTION_LOAD);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
        }
        else
        {
            auto path = turnPfs.getClosestReachableMovePath(targetFields);
            pAction->setMovepath(path, turnPfs.getCosts(path));
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
                    emit performAction(pAction);
                    return true;
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
            if (pAction->canBePerformed())
            {
                pAction->setActionID(ACTION_WAIT);
                emit performAction(pAction);
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::buildUnits(spQmlVectorBuilding & pBuildings, spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("VeryEasyAI::buildUnits()", Console::eDEBUG);
    
    if (m_pMap != nullptr)
    {
        std::vector<float> data;
        qint32 productionBuildings = 0;
        for (auto & pBuilding : pBuildings->getVector())
        {
            if (pBuilding->isProductionBuilding() &&
                m_pMap->getTerrain(pBuilding->Building::getX(), pBuilding->Building::getY())->getUnit() == nullptr)
            {
                productionBuildings++;
            }
        }
        qint32 infantryUnits = 0;
        qint32 indirectUnits = 0;
        qint32 directUnits = 0;
        qint32 transporterUnits = 0;
        for (auto & pUnit : pUnits->getVector())
        {
            QCoreApplication::processEvents();
            if (pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                infantryUnits++;
            }
            if (pUnit->getBaseMaxRange() > 1)
            {
                indirectUnits++;
            }
            else
            {
                directUnits++;
            }
            if (pUnit->getLoadingPlace() > 0 &&
                pUnit->getWeapon1ID().isEmpty() &&
                pUnit->getWeapon2ID().isEmpty())
            {
                transporterUnits++;
            }
        }
        data.push_back(m_pPlayer->getFunds());
        data.push_back(m_pPlayer->getFunds() / static_cast<float>(productionBuildings));
        data.push_back(pUnits->size());
        if (indirectUnits > 0)
        {
            data.push_back(directUnits / static_cast<float>(indirectUnits));
        }
        else
        {
            data.push_back(1.0f);
        }
        data.push_back(infantryUnits);
        data.push_back(transporterUnits);

        UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
        for (qint32 i = 0; i < m_maxTreeDecisionTries; i++)
        {
            QCoreApplication::processEvents();
            if (i == 0 || m_pPlayer->getFunds() >= m_minAllBuildingFunds)
            {
                for (auto & pBuilding : pBuildings->getVector())
                {
                    if (pBuilding->isProductionBuilding() &&
                        pBuilding->getTerrain()->getUnit() == nullptr)
                    {
                        spGameAction pAction = spGameAction::create(ACTION_BUILD_UNITS, m_pMap);
                        pAction->setTarget(QPoint(pBuilding->Building::getX(), pBuilding->Building::getY()));
                        if (pAction->canBePerformed())
                        {
                            // we're allowed to build units here
                            spMenuData pData = pAction->getMenuStepData();
                            if (pData->validData())
                            {
                                qint32 selectedUnit = -1;
                                if (pBuilding->getBuildingID() == "AIRPORT")
                                {
                                    selectedUnit = static_cast<qint32>(m_AirportBuildingTree.getDecision(data));
                                }
                                else if (pBuilding->getBuildingID() == "HARBOUR")
                                {
                                    selectedUnit = static_cast<qint32>(m_HarbourBuildingTree.getDecision(data));
                                }
                                else
                                {
                                    selectedUnit = static_cast<qint32>(m_GeneralBuildingTree.getDecision(data));
                                }
                                if (selectedUnit >= 0)
                                {
                                    QString unitID = pUnitSpriteManager->getID(selectedUnit);
                                    qint32 menuIndex = pData->getActionIDs().indexOf(unitID);
                                    if (menuIndex >= 0 && pData->getEnabledList()[menuIndex])
                                    {
                                        CoreAI::addMenuItemData(pAction, unitID, pData->getCostList()[menuIndex]);
                                        // produce the unit
                                        if (pAction->isFinalStep())
                                        {
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
                    }
                }
            }
        }
    }
    return false;
}
