#include <QSettings>

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

VeryEasyAI::VeryEasyAI()
    : CoreAI(GameEnums::AiTypes_VeryEasy),
      m_GeneralBuildingTree("resources/aidata/very_easy/generalbuilding.tree", "resources/aidata/very_easy/generalbuilding.txt"),
      m_AirportBuildingTree("resources/aidata/very_easy/airportbuilding.tree", "resources/aidata/very_easy/airportbuilding.txt"),
      m_HarbourBuildingTree("resources/aidata/very_easy/harbourbuilding.tree", "resources/aidata/very_easy/harbourbuilding.txt")
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    loadIni("very_easy/very_easy.ini");
}

void VeryEasyAI::readIni(QString name)
{
    if (QFile::exists(name))
    {
        QSettings settings(name, QSettings::IniFormat);
        settings.beginGroup("general");
        bool ok = false;
        m_ownUnitValue = settings.value("OwnUnitValue", 1.0f).toFloat(&ok);
        if(!ok)
        {
            m_ownUnitValue = 1.0f;
        }
        m_buildingValue = settings.value("BuildingValue", 1.0f).toFloat(&ok);
        if(!ok)
        {
            m_buildingValue = 1.0f;
        }
        m_minDamage = settings.value("MinDamage", -500).toInt(&ok);
        if(!ok)
        {
            m_minDamage = -500;
        }
        m_minSiloDamage = settings.value("MinSiloDamage", 4000).toInt(&ok);
        if(!ok)
        {
            m_minSiloDamage = 4000;
        }
        m_ownUnitDamageDivider = settings.value("OwnUnitDamageDivider", 4).toFloat(&ok);
        if(!ok || m_ownUnitDamageDivider < 0.0f)
        {
            m_ownUnitDamageDivider = 4;
        }
        m_minAllBuildingFunds = settings.value("MinAllBuildingFunds", 8000).toInt(&ok);
        if(!ok)
        {
            m_minAllBuildingFunds = 8000;
        }
        m_maxTreeDecisionTries = settings.value("MaxTreeDecisionTries", 10).toInt(&ok);
        if(!ok)
        {
            m_maxTreeDecisionTries = 8000;
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
    }
}

void VeryEasyAI::process()
{
    spQmlVectorBuilding pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    spQmlVectorUnit pUnits = m_pPlayer->getUnits();
    pUnits->randomize();

    spQmlVectorUnit pEnemyUnits = m_pPlayer->getEnemyUnits();
    pEnemyUnits->randomize();
    spQmlVectorBuilding pEnemyBuildings = m_pPlayer->getEnemyBuildings();
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
    rebuildIsland(pUnits.get());

    // make the ai do stuff
    if (useCOPower(pUnits.get(), pEnemyUnits.get())){}
    else
    {
        turnMode = GameEnums::AiTurnMode_DuringDay;
        if (useBuilding(pBuildings.get())){}
        else if (performActionSteps(pUnits.get(), pEnemyUnits.get(), pBuildings.get(), pEnemyBuildings.get())){}
        else
        {
            aiStep = AISteps::moveUnits;
            if (performActionSteps(pUnits.get(), pEnemyUnits.get(), pBuildings.get(), pEnemyBuildings.get())){}
            else
            {
                aiStep = AISteps::moveUnits;
                turnMode = GameEnums::AiTurnMode_EndOfDay;
                if (useCOPower(pUnits.get(), pEnemyUnits.get()))
                {
                    usedTransportSystem = false;
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

bool VeryEasyAI::performActionSteps(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits,
                                    QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    if (aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveFlares(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && captureBuildings(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    else if (aiStep <= AISteps::moveUnits && fireWithIndirectUnits(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && fireWithDirectUnits(pUnits)){}
    else if (aiStep <= AISteps::moveToTargets && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings)){}
    else if (aiStep <= AISteps::loadUnits  && !usedTransportSystem && loadUnits(pUnits)){}
    else if (aiStep <= AISteps::moveTransporters  && !usedTransportSystem && moveTransporters(pUnits, pEnemyUnits, pEnemyBuildings)){}
    else
    {
        if (!usedTransportSystem)
        {
            usedTransportSystem = true;
            aiStep = AISteps::moveUnits;
            return performActionSteps(pUnits, pEnemyUnits,  pBuildings, pEnemyBuildings);
        }
        else if (aiStep <= AISteps::loadUnits && loadUnits(pUnits)){}
        else if (aiStep <= AISteps::moveSupportUnits && moveSupport(AISteps::moveSupportUnits, pUnits, true)){}
        else if (aiStep <= AISteps::moveSupportUnits && moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, true)){}
        else if (aiStep <= AISteps::moveAway && moveAwayFromProduction(pUnits)){}
        else if (aiStep <= AISteps::buildUnits && buildUnits(pBuildings, pUnits)){}
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

bool VeryEasyAI::captureBuildings(QmlVectorUnit* pUnits)
{
    Console::print("captureBuildings()", Console::eDEBUG);
    qint32 cost = 0;
    QPoint rocketTarget = m_pPlayer->getSiloRockettarget(2, 3, cost);
    bool fireSilos = (cost >= m_minSiloDamage);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                if (pUnit->getCapturePoints() > 0)
                {
                    spGameAction pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                }
                else
                {
                    spGameAction pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    UnitPathFindingSystem pfs(pUnit);
                    pfs.explore();
                    QVector<QPoint> targets = pfs.getAllNodePoints();
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        if (pUnit->getX() == targets[i2].x() &&
                            pUnit->getY() == targets[i2].y())
                        {
                            pAction->setMovepath(QVector<QPoint>(), 0);
                        }
                        else
                        {
                            QVector<QPoint> path = pfs.getPath(targets[i2].x(), targets[i2].y());
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
                        for (qint32 i2 = 0; i2 < targets.size(); i2++)
                        {
                            if (pUnit->getX() == targets[i2].x() &&
                                pUnit->getY() == targets[i2].y())
                            {
                                pAction->setMovepath(QVector<QPoint>(), 0);
                            }
                            else
                            {
                                QVector<QPoint> path = pfs.getPath(targets[i2].x(), targets[i2].y());
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

bool VeryEasyAI::fireWithIndirectUnits(QmlVectorUnit* pUnits)
{
    Console::print("fireWithIndirectUnits()", Console::eDEBUG);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getBaseMaxRange() > 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0))
        {
            if (attack(pUnit))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::fireWithDirectUnits(QmlVectorUnit* pUnits)
{
    Console::print("fireWithDirectUnits()", Console::eDEBUG);
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getBaseMaxRange() == 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0))
        {
            if (attack(pUnit))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::attack(Unit* pUnit)
{
    if (pUnit->hasAction(CoreAI::ACTION_FIRE))
    {
        // try to perform an attack
        spGameAction pAction = new GameAction(ACTION_FIRE);
        pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
        UnitPathFindingSystem pfs(pUnit);
        pfs.explore();
        QVector<QVector3D> ret;
        QVector<QVector3D> moveTargetFields;
        CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
        float minDamage = -pUnit->getUnitValue() / m_ownUnitDamageDivider;
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
                QVector<QPoint> path = pfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                                   static_cast<qint32>(moveTargetFields[selection].y()));
                pAction->setMovepath(path, pfs.getCosts(path));
            }
            else
            {
                pAction->setMovepath(QVector<QPoint>(), 0);
            }
            CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
            // attacing none unit targets may modify the islands for a unit -> rebuild all for the love of god
            if (GameMap::getInstance()->getTerrain(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()))->getUnit() == nullptr)
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

bool VeryEasyAI::moveUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings,
                           QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                           bool supportUnits )
{
    Console::print("moveUnits()", Console::eDEBUG);
    if (aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    aiStep = AISteps::moveToTargets;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved())
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            spGameAction pAction = new GameAction(ACTION_WAIT);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));

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


bool VeryEasyAI::moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    Console::print("moveTransporters()", Console::eDEBUG);
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
                spGameAction pAction = new GameAction(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
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
                if (moveUnit(pAction, pUnit, actions, targets, targets, true))
                {
                    return true;
                }
            }
            else
            {
                spGameAction pAction = new GameAction(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                // we need to move to a loading place
                QVector<QVector3D> targets;
                QVector<QVector3D> transporterTargets;
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


bool VeryEasyAI::loadUnits(QmlVectorUnit* pUnits)
{
    Console::print("loadUnits()", Console::eDEBUG);
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
            spGameAction pAction = new GameAction(ACTION_LOAD);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));

            // find some cool targets
            appendTransporterTargets(pUnit, pUnits, transporterTargets);
            targets.append(transporterTargets);
            // till now the selected targets are a little bit lame cause we only search for reachable transporters
            // but not for reachable loading places.
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::moveUnit(spGameAction pAction, Unit* pUnit, QStringList& actions,
                          QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets, bool unload)
{
    Console::print("moveUnit()", Console::eDEBUG);
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();

        if (CoreAI::contains(transporterTargets, targetFields))
        {
            if (unload)
            {
                QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
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
                QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
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
            QVector<QPoint> path = turnPfs.getClosestReachableMovePath(targetFields);
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

bool VeryEasyAI::buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits)
{
    Console::print("buildUnits()", Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    QVector<float> data;
    qint32 productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pMap->getTerrain(pBuilding->getX(), pBuilding->getY())->getUnit() == nullptr)
        {
            productionBuildings++;
        }
    }
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    qint32 transporterUnits = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
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
    data.append(m_pPlayer->getFunds());
    data.append(m_pPlayer->getFunds() / static_cast<float>(productionBuildings));
    data.append(pUnits->size());
    if (indirectUnits > 0)
    {
        data.append(directUnits / static_cast<float>(indirectUnits));
    }
    else
    {
        data.append(1.0f);
    }
    data.append(infantryUnits);
    data.append(transporterUnits);

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i2 = 0; i2 < m_maxTreeDecisionTries; i2++)
    {
        if (i2 == 0 || m_pPlayer->getFunds() >= m_minAllBuildingFunds)
        {
            for (qint32 i = 0; i < pBuildings->size(); i++)
            {
                Building* pBuilding = pBuildings->at(i);
                if (pBuilding->isProductionBuilding() &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    spGameAction pAction = new GameAction(ACTION_BUILD_UNITS);
                    pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
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
    return false;
}
