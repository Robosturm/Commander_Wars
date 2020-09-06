#include "normalai.h"

#include "coreengine/qmlvector.h"

#include "game/player.h"
#include "game/unit.h"
#include "game/co.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/building.h"
#include "game/unitpathfindingsystem.h"
#include "ai/targetedunitpathfindingsystem.h"
#include "resource_management/weaponmanager.h"

#include "QElapsedTimer"

const float NormalAi::notAttackableDamage = 25.0f;
const float NormalAi::midDamage = 55.0f;
const float NormalAi::highDamage = 65.0f;
const float NormalAi::directIndirectRatio = 1.75f;

NormalAi::NormalAi(float initMinMovementDamage, float initMinAttackFunds, float initMinSuicideDamage,
                   float spamingFunds)
    : CoreAI (GameEnums::AiTypes_Normal),
      minMovementDamage(initMinMovementDamage),
      minAttackFunds(initMinAttackFunds),
      minSuicideDamage(initMinSuicideDamage),
      m_spamingFunds(spamingFunds)
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    buildingValue = 1.0f;
    ownUnitValue = 2.0f;
}

void NormalAi::process()
{
    QmlVectorBuilding* pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    QmlVectorUnit* pUnits = nullptr;
    QmlVectorUnit* pEnemyUnits = nullptr;
    QmlVectorBuilding* pEnemyBuildings = nullptr;

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
                aiStep = AISteps::moveUnits;
                if (performActionSteps(pUnits, pEnemyUnits, pBuildings, pEnemyBuildings)){}
                else
                {
                    aiStep = AISteps::moveUnits;
                    clearEnemyData();
                    m_IslandMaps.clear();
                    turnMode = GameEnums::AiTurnMode_EndOfDay;
                    if (useCOPower(pUnits, pEnemyUnits))
                    {
                        clearEnemyData();
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

    delete pBuildings;
    delete pUnits;
    delete pEnemyBuildings;
    delete pEnemyUnits;
}

void NormalAi::finishTurn()
{
    m_TransporterScores.clear();
    CoreAI::finishTurn();
}

bool NormalAi::performActionSteps(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits,
                                  QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    if (aiStep <= AISteps::moveUnits && buildCOUnit(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
    else if (aiStep <= AISteps::moveUnits && captureBuildings(pUnits)){}
    else if (aiStep <= AISteps::moveUnits && moveSupport(AISteps::moveUnits, pUnits, false)){}
    // indirect units
    else if (aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 2, std::numeric_limits<qint32>::max(), pBuildings, pEnemyBuildings)){}
    // direct units
    else if (aiStep <= AISteps::moveUnits && fireWithUnits(pUnits, 1, 1, pBuildings, pEnemyBuildings)){}
    else if (aiStep <= AISteps::moveUnits && repairUnits(pUnits, pBuildings, pEnemyBuildings)){}
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

bool NormalAi::buildCOUnit(QmlVectorUnit* pUnits)
{
    GameAction* pAction = new GameAction();
    for (quint8 i2 = 0; i2 < 2; i2++)
    {
        if (i2 == 0)
        {
            pAction->setActionID(ACTION_CO_UNIT_0);
        }
        else
        {
            pAction->setActionID(ACTION_CO_UNIT_1);
        }
        CO* pCO = m_pPlayer->getCO(i2);
        qint32 bestScore = 0;
        qint32 unitIdx = -1;
        if (pCO != nullptr &&
            pCO->getCOUnit() == nullptr)
        {
            qint32 active = 0;
            bool expensive = false;
            for (qint32 i = 0; i < pUnits->size(); i++)
            {
                Unit* pUnit = pUnits->at(i);

                if (pUnit->getUnitValue() >= 6000 && pUnit->getUnitRank() >= GameEnums::UnitRank_None)
                {
                    active++;
                }
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                if (pAction->canBePerformed())
                {
                    if (!pUnit->getHasMoved())
                    {
                        if (pUnit->hasWeapons())
                        {
                            qint32 score = 0;
                            if (pCO->getOffensiveBonus(pUnit, QPoint(-1, -1), nullptr, QPoint(-1, -1), false) > 0 ||
                                pCO->getDeffensiveBonus(nullptr, QPoint(-1, -1), pUnit, QPoint(-1, -1), false) > 0 ||
                                pCO->getFirerangeModifier(pUnit, QPoint(-1, -1)) > 0)
                            {
                                score += pUnit->getUnitValue() * 1.1;
                            }
                            score += pUnit->getUnitValue();
                            if (pUnit->getUnitValue() >= 6000)
                            {
                                expensive = true;
                            }
                            score -= 1000 * pUnit->getUnitRank();
                            if (score > bestScore)
                            {
                                bestScore = score;
                                unitIdx = i;
                            }
                        }
                    }
                }
            }
            if (unitIdx >= 0 && bestScore > 5000 && (active > 5 || expensive))
            {
                Unit* pUnit = pUnits->at(unitIdx);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                emit performAction(pAction);
                return true;
            }
        }
    }
    delete pAction;
    return false;
}

bool NormalAi::isUsingUnit(Unit* pUnit)
{
    if (needsRefuel(pUnit))
    {
        return false;
    }
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(pUnit->getX(), pUnit->getY()))
    {
        Building* pBuilding = pMap->getTerrain(pUnit->getX(), pUnit->getY())->getBuilding();
        if (pBuilding == nullptr && pUnit->getHpRounded() < 3)
        {
            return false;
        }
        else if (pBuilding != nullptr && pBuilding->getOwner() == m_pPlayer &&
                 pUnit->getHpRounded() < 7)
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

bool NormalAi::captureBuildings(QmlVectorUnit* pUnits)
{
    QVector<QVector3D> captureBuildings;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() && pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            if (pUnit->getCapturePoints() > 0)
            {
                GameAction* pAction = new GameAction(ACTION_CAPTURE);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                emit performAction(pAction);
                return true;
            }
            else
            {
                GameAction action(ACTION_CAPTURE);
                action.setTarget(QPoint(pUnit->getX(), pUnit->getY()));
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
                        if (action.canBePerformed())
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
                    GameAction* pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    QVector<QPoint> path = pfs.getPath(static_cast<qint32>(captures[targetIndex].x()), static_cast<qint32>(captures[targetIndex].y()));
                    pAction->setMovepath(path, pfs.getCosts(path));
                    updatePoints.append(pUnit->getPosition());
                    updatePoints.append(pAction->getActionTarget());
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
                        return true;
                    }
                    else
                    {
                        QPoint rocketTarget = m_pPlayer->getRockettarget(2, 3);
                        CoreAI::addSelectedFieldData(pAction, rocketTarget);
                        pAction->setActionID(ACTION_MISSILE);
                        emit performAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool NormalAi::fireWithUnits(QmlVectorUnit* pUnits, qint32 minfireRange, qint32 maxfireRange,
                             QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() &&
            pUnit->getBaseMaxRange() >= minfireRange &&
            pUnit->getBaseMaxRange() <= maxfireRange &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0) &&
            pUnit->hasAction(CoreAI::ACTION_FIRE))
        {
            GameAction* pAction = new GameAction(ACTION_FIRE);
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
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
                if (pAction->isFinalStep())
                {
                    updatePoints.append(pUnit->getPosition());
                    updatePoints.append(pAction->getActionTarget());
                    updatePoints.append(QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
                    emit performAction(pAction);
                    return true;
                }
            }
            delete pAction;
        }
    }
    return false;
}

bool NormalAi::moveUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings,
                         QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                         qint32 minfireRange, qint32 maxfireRange, bool supportUnits)
{
    if (aiStep < AISteps::moveToTargets)
    {
        createMovementMap(pBuildings, pEnemyBuildings);
    }
    aiStep = AISteps::moveToTargets;
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
            GameAction* pAction = new GameAction(ACTION_WAIT);
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

bool NormalAi::loadUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
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
            GameAction* pAction = new GameAction(ACTION_LOAD);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));

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

bool NormalAi::moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
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
                GameAction* pAction = new GameAction(ACTION_WAIT);
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
                if (moveToUnloadArea(pAction, pUnit, pUnits, actions, targets, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
            }
            else
            {
                GameAction* pAction = new GameAction(ACTION_WAIT);
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
                if (moveUnit(pAction, pUnit, pUnits, actions, targets, transporterTargets, true, pBuildings, pEnemyBuildings))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool NormalAi::moveToUnloadArea(GameAction* pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
                                QVector<QVector3D>& targets,
                                QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    spGameMap pMap = GameMap::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    TargetedUnitPathFindingSystem pfs(pUnit, targets, &m_MoveCostMap);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
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
                    MenuData* pDataMenu = pAction->getMenuStepData();
                    if (pDataMenu->validData())
                    {
                        QStringList actions = pDataMenu->getActionIDs();
                        QVector<qint32> unitIDx = pDataMenu->getCostList();
                        QVector<QList<QVariant>> unloadFields;
                        for (qint32 i = 0; i < unitIDx.size() - 1; i++)
                        {
                            QString function1 = "getUnloadFields";
                            QJSValueList args1;
                            QJSValue obj1 = pInterpreter->newQObject(pAction);
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
                                            MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                                            addSelectedFieldData(pAction, pFields->getPoints()->at(0));
                                            delete pFields;
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
                                MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                                qint32 field = Mainapp::randIntBase(0, pFields->getPoints()->size() - 1);
                                addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                                delete pFields;
                            }
                        }
                    }
                    else
                    {
                        delete pDataMenu;
                        return false;
                    }
                    delete pDataMenu;
                }
                while (unloaded);
                addMenuItemData(pAction, ACTION_WAIT, 0);
                updatePoints.append(pUnit->getPosition());
                updatePoints.append(pAction->getActionTarget());
                emit performAction(pAction);
                return true;
            }
        }
        else
        {
            return moveUnit(pAction, pUnit, pUnits, actions, targets, targets, true, pBuildings, pEnemyBuildings);
        }
    }
    return false;
}

bool NormalAi::repairUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    aiStep = AISteps::moveUnits;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!isUsingUnit(pUnit) && !pUnit->getHasMoved())
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            GameAction* pAction = new GameAction(ACTION_WAIT);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
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
                pAction = new GameAction(ACTION_WAIT);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                UnitPathFindingSystem turnPfs(pUnit);
                turnPfs.explore();
                if (suicide(pAction, pUnit, turnPfs))
                {
                    return true;
                }
                delete pAction;
            }
        }
    }
    return false;
}

bool NormalAi::moveUnit(GameAction* pAction, Unit* pUnit, QmlVectorUnit* pUnits, QStringList& actions,
                        QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets,
                        bool shortenPathForTarget,
                        QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
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
            QVector<QPoint> path = turnPfs.getPath(targetFields.x(), targetFields.y());
            pAction->setMovepath(path, turnPfs.getCosts(path));
            pAction->setActionID(ACTION_LOAD);
            updatePoints.append(pUnit->getPosition());
            updatePoints.append(pAction->getActionTarget());
            emit performAction(pAction);
            return true;
        }
        else if (!shortenPathForTarget && CoreAI::contains(targets, targetFields))
        {
            QVector<QPoint> movePath = turnPfs.getClosestReachableMovePath(targetFields);
            pAction->setMovepath(movePath, turnPfs.getCosts(movePath));
            pAction->setActionID(ACTION_WAIT);
            updatePoints.append(pUnit->getPosition());
            updatePoints.append(pAction->getActionTarget());
            emit performAction(pAction);
            return true;
        }
        else
        {
            QVector<QPoint> movePath = turnPfs.getClosestReachableMovePath(targetFields);
            if (movePath.size() == 0)
            {
                movePath.append(QPoint(pUnit->getX(), pUnit->getY()));
            }
            qint32 idx = getMoveTargetField(pUnit, pUnits, turnPfs, movePath, pBuildings, pEnemyBuildings);
            if (idx < 0)
            {
                std::tuple<QPoint, float, bool> target = moveToSafety(pUnit, pUnits, turnPfs, movePath[0], pBuildings, pEnemyBuildings);
                QPoint ret = std::get<0>(target);
                float minDamage = std::get<1>(target);
                bool allEqual = std::get<2>(target);
                if (((ret.x() == pUnit->getX() && ret.y() == pUnit->getY()) ||
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
            // when we don't move try to attack if possible
            if ((pUnit->getHp() > 3.5f))
            {
                pAction->setActionID(ACTION_FIRE);
                QVector<QVector3D> ret;
                QVector<QVector3D> moveTargetFields;
                getBestAttacksFromField(pUnit, pAction, ret, moveTargetFields);
                if (ret.size() > 0 && ret[0].z() >= -pUnit->getUnitValue()  * minSuicideDamage)
                {
                    qint32 selection = Mainapp::randIntBase(0, ret.size() - 1);
                    QVector3D target = ret[selection];
                    CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                 static_cast<qint32>(target.y())));
                    if (pAction->isFinalStep())
                    {
                        updatePoints.append(pUnit->getPosition());
                        updatePoints.append(pAction->getActionTarget());
                        updatePoints.append(QPoint(static_cast<qint32>(target.x()),
                                                   static_cast<qint32>(target.y())));
                        emit performAction(pAction);
                        return true;
                    }
                }
            }
            if (pAction->getMovePath().size() > 0)
            {
                updatePoints.append(pUnit->getPosition());
                updatePoints.append(pAction->getActionTarget());
                for (const auto action : actions)
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
                for (const auto action : actions)
                {
                    if (action.startsWith(ACTION_PLACE))
                    {
                        pAction->setActionID(action);
                        if (pAction->canBePerformed())
                        {
                            MarkedFieldData* pData = pAction->getMarkedFieldStepData();
                            QPoint point = pData->getPoints()->at(Mainapp::randIntBase(0, pData->getPoints()->size() - 1));
                            CoreAI::addSelectedFieldData(pAction, point);
                            delete pData;
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
                    if (ret.size() > 0 && ret[0].z() >= -pUnit->getUnitValue()  * minSuicideDamage)
                    {
                        qint32 selection = Mainapp::randIntBase(0, ret.size() - 1);
                        QVector3D target = ret[selection];
                        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                     static_cast<qint32>(target.y())));
                        if (pAction->isFinalStep())
                        {
                            updatePoints.append(pUnit->getPosition());
                            updatePoints.append(pAction->getActionTarget());
                            updatePoints.append(QPoint(static_cast<qint32>(target.x()),
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
    delete pAction;
    return false;
}

bool NormalAi::suicide(GameAction* pAction, Unit* pUnit, UnitPathFindingSystem& turnPfs)
{
    // we don't have a good option do the best that we can attack with an all in attack :D
    pAction->setActionID(ACTION_FIRE);
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &turnPfs, ret, moveTargetFields);
    if (ret.size() > 0 && ret[0].z() >= -pUnit->getUnitValue() * minSuicideDamage)
    {
        qint32 selection = Mainapp::randIntBase(0, ret.size() - 1);
        QVector3D target = ret[selection];
        QVector<QPoint> path = turnPfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                               static_cast<qint32>(moveTargetFields[selection].y()));
        pAction->setMovepath(path, turnPfs.getCosts(path));
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep())
        {
            updatePoints.append(pUnit->getPosition());
            updatePoints.append(pAction->getActionTarget());
            updatePoints.append(QPoint(static_cast<qint32>(target.x()),
                                       static_cast<qint32>(target.y())));
            emit performAction(pAction);
            return true;
        }
    }
    return false;
}

std::tuple<QPoint, float, bool> NormalAi::moveToSafety(Unit* pUnit, QmlVectorUnit* pUnits,
                                                       UnitPathFindingSystem& turnPfs, QPoint target,
                                                       QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> targets = turnPfs.getAllNodePoints();
    QPoint ret(pUnit->getX(), pUnit->getY());
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
            qint32 distance = Mainapp::getDistance(target, targets[i]);
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

qint32 NormalAi::getMoveTargetField(Unit* pUnit, QmlVectorUnit* pUnits, UnitPathFindingSystem& turnPfs,
                                    QVector<QPoint>& movePath, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
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
            if (counterDamage < pUnit->getUnitValue() * minMovementDamage)
            {
                return i;
            }
        }
    }
    return -1;
}

qint32 NormalAi::getBestAttackTarget(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QVector4D>& ret,
                                     QVector<QVector3D>& moveTargetFields,
                                     QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 target = -1;
    qint32 currentDamage = std::numeric_limits<qint32>::min();
    qint32 deffense = 0;

    float minFundsDamage = -pUnit->getUnitValue() * minAttackFunds;

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
                fundsDamage *= 2.0f;
            }
            if (newHp <= 0)
            {
                fundsDamage *= 2.0f;
            }
            if (pEnemy->getMinRange(pEnemy->getPosition()) > 1)
            {
                fundsDamage *= 3.0f;
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
        qint32 newHp = Mainapp::roundUp(newLife);
        qint32 remainingDays = Mainapp::roundUp(restCapture / static_cast<float>(currentHp));
        if (newHp <= 0)
        {
            if (remainingDays > 0)
            {
                ret = 1 + (21.0f - currentHp) / currentHp;
            }
            else
            {
                ret = 22.0f;
            }
        }
        else
        {
            qint32 newRemainingDays = Mainapp::roundUp(restCapture / static_cast<float>(newHp));
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
                ret = 1+ (newRemainingDays - remainingDays) / remainingDays;
            }
            if (ret > 6.0f)
            {
                ret = ret / 2.0f + 3.0f;
            }
        }
    }
    if (pBuilding != nullptr &&
        pBuilding->getOwner() == m_pPlayer &&
        pBuilding->getBuildingID() == "HQ" &&
        pUnit->getActionList().contains(ACTION_CAPTURE))
    {
        ret *= 50;
    }
    return ret;
}

float NormalAi::calculateCounterDamage(Unit* pUnit, QmlVectorUnit* pUnits, QPoint newPosition,
                                       Unit* pEnemyUnit, float enemyTakenDamage,
                                       QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings,
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
            QPoint enemyPos = QPoint(pNextEnemy->getX(), pNextEnemy->getY());
            qint32 minFireRange = pNextEnemy->getMinRange(enemyPos);
            qint32 maxFireRange = pNextEnemy->getMaxRange(enemyPos);
            qint32 moveRange = 0;
            qint32 distance = Mainapp::getDistance(newPosition, enemyPos);
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
                enemyDamage *= 10.0f;
                QRectF damageData;
                QVector<QPoint> targets = m_EnemyPfs[i]->getAllNodePoints();
                if (distance >= minFireRange && distance <= maxFireRange)
                {
                    damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, enemyPos, pUnit, 0, newPosition, ignoreOutOfVisionRange);
                    for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
                    {
                        distance = Mainapp::getDistance(QPoint(pUnits->at(i3)->getX(), pUnits->at(i3)->getY()), enemyPos);
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
                        distance = Mainapp::getDistance(newPosition, targets[i2]);
                        if (distance >= minFireRange && distance <= maxFireRange &&
                            (pMap->getTerrain(targets[i2].x(), targets[i2].y())->getUnit() == nullptr ||
                             (targets[i2].x() == pNextEnemy->getX() && targets[i2].y() == pNextEnemy->getY())))
                        {
                            damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, targets[i2], pUnit, 0, newPosition, ignoreOutOfVisionRange);
                            break;
                        }
                    }
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        for (qint32 i3 = 0; i3 < pUnits->size(); i3++)
                        {
                            distance = Mainapp::getDistance(QPoint(pUnits->at(i3)->getX(), pUnits->at(i3)->getY()), targets[i2]);
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

float NormalAi::calculateCounteBuildingDamage(Unit* pUnit, QPoint newPosition, QmlVectorBuilding* pBuildings, QmlVectorBuilding* pEnemyBuildings)
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
    QmlVectorPoint* pCircle = Mainapp::getCircle(1, 2);
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
                counterDamage +=  4;
            }
        }
    }
    delete pCircle;
    return counterDamage;
}

void NormalAi::updateEnemyData(QmlVectorUnit* pUnits)
{
    rebuildIsland(pUnits);
    if (m_EnemyUnits.size() == 0)
    {
        m_EnemyUnits = m_pPlayer->getSpEnemyUnits();
        for (qint32 i = 0; i < m_EnemyUnits.size(); i++)
        {
            m_EnemyPfs.append(new UnitPathFindingSystem(m_EnemyUnits[i].get()));
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
    for (qint32 i = 0; i < updatePoints.size(); i++)
    {
        for (qint32 i2 = 0; i2 < m_EnemyUnits.size(); i2++)
        {
            if (!updated.contains(i2))
            {
                if (m_EnemyUnits[i2]->getHp() > 0 && m_EnemyUnits[i2]->getTerrain() != nullptr)
                {
                    if (qAbs(updatePoints[i].x() - m_EnemyUnits[i2]->getX()) +
                        qAbs(updatePoints[i].y() - m_EnemyUnits[i2]->getY()) <=
                        m_EnemyUnits[i2]->getMovementpoints(QPoint(m_EnemyUnits[i2]->getX(), m_EnemyUnits[i2]->getY())) + 2)
                    {
                        m_EnemyPfs[i2] = new UnitPathFindingSystem(m_EnemyUnits[i2].get());
                        m_EnemyPfs[i2]->explore();
                    }
                    updated.push_back(i2);
                }
            }
        }
    }
    updatePoints.clear();
}

void NormalAi::calcVirtualDamage(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        QVector<QPoint> attackedUnits;
        QVector<float> damage;
        if (isUsingUnit(pUnit))
        {
            GameAction action(ACTION_FIRE);
            action.setTarget(QPoint(pUnit->getX(), pUnit->getY()));
            UnitPathFindingSystem pfs(pUnit);
            pfs.explore();
            QVector<QVector4D> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, &action, &pfs, ret, moveTargetFields);
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
                if (m_EnemyUnits[i3]->getX() == attackedUnits[i2].x() &&
                    m_EnemyUnits[i3]->getY() == attackedUnits[i2].y())
                {
                    m_VirtualEnemyData[i3].setX(m_VirtualEnemyData[i3].x() + static_cast<double>(damage[i2]) / (damage.size() * 2.0));
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

bool NormalAi::buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits,
                          QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    aiStep = AISteps::buildUnits;


    spGameMap pMap = GameMap::getInstance();
    WeaponManager* pWeaponManager = WeaponManager::getInstance();
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
            pMap->getTerrain(pBuilding->getX(), pBuilding->getY())->getUnit() == nullptr)
        {
            productionBuildings++;
        }
    }
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    QVector<QVector4D> attackCount(pEnemyUnits->size(), QVector4D(0, 0, 0, 0));
    QVector<std::tuple<Unit*, Unit*>> transportTargets;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            infantryUnits++;
        }
        else if (pUnit->hasWeapons())
        {
            if (pUnit->getBaseMaxRange() > 1)
            {
                indirectUnits++;
            }
            else
            {
                directUnits++;
            }
        }
        if (pUnit->getLoadingPlace() > 0)
        {
            QVector<QVector3D> ret;
            QVector<Unit*> transportUnits = appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, true, ret, true);
            for (qint32 i2 = 0; i2 < transportUnits.size(); i2++)
            {
                transportTargets.append(std::tuple<Unit*, Unit*>(pUnit, transportUnits[i2]));
            }
        }
    }

    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            Unit* pUnit = pUnits->at(i);
            float dmg1 = 0.0f;
            float hpValue = pUnit->getHpRounded() / 10.0f;
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

            if ((dmg1 > notAttackableDamage || dmg2 > notAttackableDamage) &&
                pEnemyUnit->getMovementpoints(QPoint(pEnemyUnit->getX(), pEnemyUnit->getY())) - pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY())) < 2)
            {
                if (onSameIsland(pUnit, pEnemyUnits->at(i2)))
                {
                    attackCount[i2].setY(attackCount[i2].y() + 1);
                }
                attackCount[i2].setX(attackCount[i2].x() + 1);
            }
            if (dmg1 > midDamage || dmg2 > midDamage)
            {
                attackCount[i2].setZ(attackCount[i2].z() + 1);
            }
            if (dmg1 > highDamage || dmg2 > highDamage)
            {
                attackCount[i2].setW(attackCount[i2].w() + 1);
            }
        }
    }
    float funds = m_pPlayer->getFunds();
    // calc average costs if we would build same cost units on every building
    float fundsPerFactory = funds / (static_cast<float>(productionBuildings));
    if (productionBuildings > 2)
    {
        // if we have a bigger number of buildings we wanna spam out units but not at an average costs overall buildings
        // but more a small amount of strong ones and a large amount of cheap ones
        // so we use a small (x - a) / (x - b) function here
        float test = funds * ((productionBuildings - 2.5f) / (static_cast<float>(productionBuildings) - 1.65f));
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
    data[UnitEnemyRatio] = (pUnits->size() + 10) / (pEnemyUnits->size() + 10);
    if (enemeyCount > 1)
    {
        data[UnitEnemyRatio] *= (enemeyCount - 1);
    }
    data[UnitCount] = pUnits->size();

    GameAction* pAction = new GameAction(ACTION_BUILD_UNITS);
    float bestScore = std::numeric_limits<float>::lowest();
    QVector<qint32> buildingIdx;
    QVector<qint32> unitIDx;
    QVector<float> scores;
    QVector<bool> transporters;
    float variance = pMap->getCurrentDay() - 1;
    if (variance > 10)
    {
        variance = 10.0f;
    }
    QmlVectorPoint* pFields = Mainapp::getCircle(1, 1);
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pBuilding->getTerrain()->getUnit() == nullptr)
        {
            pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
            if (pAction->canBePerformed())
            {
                // we're allowed to build units here
                MenuData* pData = pAction->getMenuStepData();
                if (pData->validData())
                {
                    auto enableList = pData->getEnabledList();
                    auto actionIds = pData->getActionIDs();
                    QVector<qint32> actions;
                    for (qint32 i2 = 0; i2 < pData->getActionIDs().size(); i2++)
                    {
                        if (enableList[i2])
                        {
                            Unit dummy(actionIds[i2], m_pPlayer, false);
                            qint32 buildingX = pBuilding->getX();
                            qint32 buildingY = pBuilding->getY();
                            dummy.setVirtuellX(buildingX);
                            dummy.setVirtuellY(buildingY);
                            data[UnitCost] = dummy.getUnitCosts();
                            createIslandMap(dummy.getMovementType(), dummy.getUnitID());
                            bool canMove = false;

                            for (qint32 i3 = 0; i3 < pFields->size(); i3++)
                            {
                                qint32 x = buildingX + pFields->at(i3).x();
                                qint32 y = buildingY + pFields->at(i3).y();
                                if (pMap->onMap(x, y) &&
                                    dummy.getBaseMovementCosts(x, y, x, y) > 0)
                                {
                                    canMove = true;
                                    break;
                                }
                            }

                            if (canMove)
                            {
                                float score = 0.0f;
                                bool isTransporter = false;
                                if (!dummy.getWeapon1ID().isEmpty() ||
                                    !dummy.getWeapon2ID().isEmpty())
                                {
                                    if (dummy.getBaseMaxRange() > 1)
                                    {
                                        data[IndirectUnit] = 1.0;
                                        data[DirectUnit] = 0.0;
                                    }
                                    else
                                    {
                                        data[IndirectUnit] = 0.0;
                                        data[DirectUnit] = 1.0;
                                    }
                                    if (dummy.getActionList().contains(ACTION_CAPTURE) &&
                                        dummy.getLoadingPlace() == 0)
                                    {
                                        data[InfantryUnit] = 1.0;
                                    }
                                    else
                                    {
                                        data[InfantryUnit] = 0.0;
                                    }
                                    data[FundsFactoryRatio] = dummy.getUnitCosts() / fundsPerFactory;
                                    if (pEnemyBuildings->size() > 0 && enemeyCount > 0)
                                    {
                                        data[BuildingEnemyRatio] = pBuildings->size() / (static_cast<float>(pEnemyBuildings->size()) / static_cast<float>(enemeyCount));
                                    }
                                    else
                                    {
                                        data[BuildingEnemyRatio] = 0.0;
                                    }
                                    float bonusFactor = 1.0f;
                                    if ((data[DirectUnitRatio] > directIndirectRatio && dummy.getBaseMaxRange() > 1) ||
                                        (data[DirectUnitRatio] < directIndirectRatio && dummy.getBaseMaxRange() == 1))
                                    {
                                        bonusFactor = 1.2f;
                                    }
                                    auto damageData = calcExpectedFundsDamage(pBuilding->getX(), pBuilding->getY(), dummy, pEnemyUnits, attackCount, bonusFactor);
                                    data[NotAttackableCount] = std::get<1>(damageData);
                                    data[DamageData] =  std::get<0>(damageData);

                                    if (dummy.getBonusOffensive(QPoint(-1, -1), nullptr, QPoint(-1, -1), false) > 0 ||
                                        dummy.getBonusDefensive(QPoint(-1, -1), nullptr, QPoint(-1, -1), false) > 0)
                                    {
                                        data[COBonus] = 1;
                                    }
                                    else
                                    {
                                        data[COBonus] = 0;
                                    }
                                    data[Movementpoints] = dummy.getMovementpoints(QPoint(pBuilding->getX(), pBuilding->getY()));
                                    data[ReachDistance] = getClosestTargetDistance(pBuilding->getX(), pBuilding->getY(), dummy, pEnemyUnits, pEnemyBuildings);
                                    score = calcBuildScore(data);
                                }
                                else
                                {
                                    bool found = false;
                                    for (auto & data : m_TransporterScores)
                                    {
                                        if (std::get<2>(data) == buildingX &&
                                            std::get<3>(data) == buildingY &&
                                            std::get<1>(data) == actionIds[i2])
                                        {
                                            score = std::get<0>(data);
                                            found = true;
                                            break;
                                        }
                                    }
                                    if (!found)
                                    {
                                        score = calcTransporterScore(dummy, pUnits, pEnemyUnits, pEnemyBuildings, transportTargets, data);
                                        m_TransporterScores.append(std::tuple<float, QString, qint32, qint32>(score, actionIds[i2], buildingX, buildingY));
                                    }
                                    isTransporter = true;
                                }
                                score *= BaseGameInputIF::getUnitBuildValue(dummy.getUnitID());
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
                delete pData;
            }

        }
    }
    delete pFields;

    if (buildingIdx.size() > 0)
    {
        qint32 item = Mainapp::randIntBase(0, buildingIdx.size() - 1);
        Building* pBuilding = pBuildings->at(buildingIdx[item]);
        pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
        if (transporters[item])
        {
            m_TransporterScores.clear();
        }
        MenuData* pData = pAction->getMenuStepData();
        if (pData->validData())
        {
            CoreAI::addMenuItemData(pAction, pData->getActionIDs()[unitIDx[item]], pData->getCostList()[unitIDx[item]]);
        }
        delete pData;
        // produce the unit
        if (pAction->isFinalStep())
        {
            updatePoints.append(pAction->getActionTarget());
            emit performAction(pAction);
            return true;
        }
    }
    delete pAction;
    return false;
}

qint32 NormalAi::getClosestTargetDistance(qint32 posX, qint32 posY, Unit& dummy, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    qint32 minDistance = std::numeric_limits<qint32>::max();
    QPoint pos(posX, posY);
    qint32 islandIdx = CoreAI::getIslandIndex(&dummy);
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemyUnit = pEnemyUnits->at(i);
        if (onSameIsland(islandIdx, posX, posY, pEnemyUnit->getX(), pEnemyUnit->getY()))
        {
            if (dummy.isAttackable(pEnemyUnit, true))
            {
                qint32 distance = Mainapp::getDistance(pos, pEnemyUnit->getPosition());
                if (minDistance > distance)
                {
                    minDistance = distance;
                }
            }
        }
    }
    if (dummy.getActionList().contains(ACTION_CAPTURE))
    {
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pBuilding = pEnemyBuildings->at(i);
            if (dummy.canMoveOver(pBuilding->getX(), pBuilding->getY()))
            {
                if (pBuilding->isCaptureOrMissileBuilding() &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    qint32 distance = Mainapp::getDistance(pos, pBuilding->getPosition());
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

std::tuple<float, qint32> NormalAi::calcExpectedFundsDamage(qint32 posX, qint32 posY, Unit& dummy, QmlVectorUnit* pEnemyUnits, QVector<QVector4D> attackCount, float bonusFactor)
{
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
    while (attacksCount  < 5 &&
           pEnemyUnits->size() > 5 &&
           counter <= maxCounter)
    {
        for (qint32 i3 = 0; i3 < pEnemyUnits->size(); i3++)
        {
            Unit* pEnemyUnit = pEnemyUnits->at(i3);
            QPoint enemyPosition = pEnemyUnit->getPosition();
            float distance = Mainapp::getDistance(position, enemyPosition);
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
                if (dmg > pEnemyUnit->getHp() * 10.0f)
                {
                    dmg = pEnemyUnit->getHp() * 10.0f;
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
                    if (counterDmg > 100.0f)
                    {
                        counterDmg = 100.0f;
                    }
                    else if (counterDmg < 7.5f)
                    {
                        counterDmg = 0.0f;
                    }
                    float resDamage = 0;

                    float enemyMovepoints = pEnemyUnit->getBaseMovementPoints();
                    float smoothing = 3;
                    if (myMovepoints + myFirerange >= enemyMovepoints)
                    {
                        float mult = (myMovepoints + myFirerange + smoothing) / (enemyMovepoints + enemyFirerange + smoothing);
                        if (mult > 1.5f)
                        {
                            mult = 1.5f;
                        }
                        if (myFirerange > 1)
                        {
                            // increased bonus for indirects since they should be attacked less often and attack more often
                            mult += 1.0f;
                        }
                        resDamage = dmg / (pEnemyUnit->getHp() * 10.0f) * pEnemyUnit->getUnitValue() * mult * bonusFactor -
                                    counterDmg / 100.0f * pEnemyUnit->getUnitValue();
                    }
                    else
                    {
                        float mult = (enemyMovepoints + enemyFirerange + smoothing) / (myMovepoints + myFirerange + smoothing);
                        if (mult > 1.5f)
                        {
                            mult = 1.5f;
                        }
                        resDamage = dmg / (pEnemyUnit->getHp() * 10.0f) * pEnemyUnit->getUnitValue() * bonusFactor -
                                    counterDmg / 100.0f * pEnemyUnit->getUnitValue() * mult;
                    }
                    if (resDamage > pEnemyUnit->getUnitValue())
                    {
                        resDamage = pEnemyUnit->getUnitValue();
                    }
                    float factor = 1.0f;
                    if (dmg > highDamage)
                    {
                        factor += (attackCount[i3].w() + smoothing) / (attackCount[i3].x() + smoothing);
                    }
                    else if (dmg > midDamage)
                    {
                        factor += (attackCount[i3].z() + smoothing) / (attackCount[i3].z() + smoothing);
                    }
                    if (onSameIsland(dummy.getMovementType(), posX, posY, pEnemyUnit->getX(), pEnemyUnit->getY()))
                    {
                        factor += (2.0f - (distance / static_cast<float>(myMovepoints) * (2.0f / 5.0f)));
                        if (pEnemyUnit->hasWeapons())
                        {
                            float notAttackableValue = 0.0f;
                            if (dmg > highDamage)
                            {
                                notAttackableValue = 2.0f;
                            }
                            else if (dmg > midDamage)
                            {
                                notAttackableValue = 1.5f;
                            }
                            else if (dmg > notAttackableDamage)
                            {
                                notAttackableValue = 1.0f;
                            }
                            else
                            {
                                factor /= 2.0f;
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
                                notAttackableCount  += notAttackableValue / 2.0f;
                            }
                        }
                        else
                        {
                            factor /= 8.0f;
                        }
                    }
                    else
                    {
                        factor += (1.0f - (distance / static_cast<float>(myMovepoints) * (1.0f / 3.0f)));
                        if (pEnemyUnit->hasWeapons())
                        {
                            float notAttackableValue = 0.0f;
                            if (dmg > highDamage)
                            {
                                notAttackableValue = 2.0f;
                            }
                            else if (dmg > midDamage)
                            {
                                notAttackableValue = 1.5f;
                            }
                            else if (dmg > notAttackableDamage)
                            {
                                notAttackableValue = 1.0f;
                            }
                            else
                            {
                                factor /= 2.0f;
                            }
                            if (attackCount[i3].y() == 0.0f &&
                                attackCount[i3].x() == 0.0f &&
                                attackCount[i3].z() == 0.0f &&
                                attackCount[i3].w() == 0.0f)
                            {
                                notAttackableCount += notAttackableValue / 2.0f;
                            }
                            else if (attackCount[i3].x() == 0.0f &&
                                     attackCount[i3].z() == 0.0f &&
                                     attackCount[i3].w() == 0.0f)
                            {
                                notAttackableCount  += notAttackableValue / 4.0f;
                            }
                        }
                        else
                        {
                            factor /= 8.0f;
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
    if (attacksCount <= 0)
    {
        attacksCount = 1;
    }
    float damage = damageCount / attacksCount;
    if (damage > 0)
    {
        float value = (attacksCount) / static_cast<float>(pEnemyUnits->size());
        if (attacksCount > 5)
        {
            damage *= (attacksCount + 5) / static_cast<float>(pEnemyUnits->size());
        }
        else
        {
            damage *= value;
        }
        // reduce effectiveness of units who can't attack a lot of units
        if (dummy.getMinRange(position) > 1)
        {
            if (value < 0.2f)
            {
                notAttackableCount /= 4;
                damage /= 4.0f;
            }
            else if (value < 0.35f)
            {
                notAttackableCount /= 3;
                damage /= 3.0f;
            }
            else if (value < 0.5f)
            {
                notAttackableCount /= 2;
                damage /= 2.0f;
            }
        }
    }
    return std::tuple<float, qint32>(damage, notAttackableCount);
}

float NormalAi::calcTransporterScore(Unit& dummy, QmlVectorUnit* pUnits,
                                     QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings,
                                     QVector<std::tuple<Unit*, Unit*>>& transportTargets,
                                     QVector<float>& data)
{
    float score = 0.0f;
    QVector<QVector3D> targets;
    QmlVectorUnit relevantUnits;
    QPoint position = dummy.getPosition();
    qint32 movement = dummy.getBaseMovementPoints();
    if (movement == 0)
    {
        movement = 1;
    }
    spGameMap pMap = GameMap::getInstance();
    qint32 loadingPlace = dummy.getLoadingPlace();
    qint32 smallTransporterCount = 0;
    qint32 maxCounter = pMap->getMapWidth() * pMap->getMapHeight() / (movement * 2);
    qint32 counter = 1;
    while (relevantUnits.size()  < loadingPlace * 3 &&
           pUnits->size() > loadingPlace * 2 &&
           counter <= maxCounter)
    {
        for (qint32 i = 0; i < pUnits->size(); i++)
        {
            qint32 distance = Mainapp::getDistance(position, pUnits->at(i)->getPosition());
            if (distance / movement <= counter * maxDayDistance &&
                distance / movement >= (counter - 1) * maxDayDistance)
            {
                relevantUnits.append(pUnits->at(i));
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
    QVector<Unit*> loadingUnits = appendLoadingTargets(&dummy, &relevantUnits, pEnemyUnits, pEnemyBuildings, false, true, targets, true);
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
                score += 70.0f;
            }
            i++;
        }
        else
        {
            loadingUnits.removeAt(i);
        }
    }
    if (score == 0.0f && pUnits->size() / (smallTransporterCount + 1) > 5 && dummy.getLoadingPlace() == 1)
    {
        spGameMap pMap = GameMap::getInstance();
        if (smallTransporterCount > 0)
        {
            score += ((pMap->getMapWidth() * pMap->getMapHeight()) / 200.0f) / smallTransporterCount * 10;
        }
        else if (pMap->getMapWidth() * pMap->getMapHeight() >= 200.0f)
        {
            score += 30.0f;
        }
        // give a bonus to t-heli's or similar units cause they are mostlikly much faster
        if (dummy.useTerrainDefense() == false && score > 15.0f)
        {
            score += 15.0f;
        }
    }
    if (transporterUnits.size() > 0 && loadingUnits.size() > 0)
    {
        score += (loadingUnits.size() / static_cast<float>(transporterUnits.size() * 6.0f)) * 10;
    }
    else
    {
        score += loadingUnits.size() * 10;
    }
    if (loadingUnits.size() > 0 && score > 20)
    {
        score += dummy.getLoadingPlace() * 5;
        score += calcCostScore(data);
        score += loadingUnits.size() * 5;
    }
    // avoid building transporters if the score is low
    if (score <= 10)
    {
        score = std::numeric_limits<float>::lowest();
    }
    return score;
}

bool NormalAi::canTransportToEnemy(Unit* pUnit, Unit* pLoadedUnit, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    QmlVectorPoint* pUnloadArea = Mainapp::getCircle(1, 1);
    // check for enemis
    qint32 loadedUnitIslandIdx = getIslandIndex(pLoadedUnit);
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    QVector<qint32> checkedIslands;
    QVector<QVector3D> targets;
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemy = pEnemyUnits->at(i);
        qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx]->getIsland(pEnemy->getX(), pEnemy->getY());
        // check if we could reach the enemy if we would be on his island
        // and we didn't checked this island yet -> improves the speed
        if (targetIsland >= 0 )
        {
            // could we beat his ass? -> i mean can we attack him
            // if so this is a great island
            if (pLoadedUnit->isAttackable(pEnemy, true))
            {
                checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands, unitIslandIdx, unitIsland,
                                        loadedUnitIslandIdx, targetIsland, pUnloadArea, targets);
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
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pEnemyBuilding = pEnemyBuildings->at(i);

            qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx]->getIsland(pEnemyBuilding->getX(), pEnemyBuilding->getY());
            // check if we could reach the enemy if we would be on his island
            // and we didn't checked this island yet -> improves the speed
            if (targetIsland >= 0 )
            {
                if (pEnemyBuilding->isCaptureOrMissileBuilding())
                {
                    checkIslandForUnloading(pUnit, pLoadedUnit, checkedIslands, unitIslandIdx, unitIsland,
                                            loadedUnitIslandIdx, targetIsland, pUnloadArea, targets);
                    if (targets.size() > 0)
                    {
                        break;
                    }
                }
            }
        }
    }
    delete pUnloadArea;
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
        if (data[DirectUnitRatio] > directIndirectRatio)
        {
            score += 3 * (data[DirectUnitRatio] - directIndirectRatio) / 0.1f;
        }
        else if (data[DirectUnitRatio] < directIndirectRatio / 2)
        {
            score -= 6 * (directIndirectRatio - data[DirectUnitRatio]) / 0.1f;
        }
        else if (data[DirectUnitRatio] < directIndirectRatio)
        {
            score -= 5 * (directIndirectRatio - data[DirectUnitRatio]) / 0.1f;
        }
    }
    else if (data[DirectUnit] == 1.0f)
    {
        // direct unit
        if (data[DirectUnitRatio] < directIndirectRatio)
        {
            score += 3.5f * (directIndirectRatio - data[DirectUnitRatio]) / 0.1f;
        }
        else if (data[DirectUnitRatio] > directIndirectRatio * 2)
        {
            score -= 6 * (data[DirectUnitRatio] - directIndirectRatio) / 0.1f;
        }
        else if (data[DirectUnitRatio] > directIndirectRatio)
        {
            score -= 3 * (data[DirectUnitRatio] - directIndirectRatio) / 0.1f;
        }
    }
    if (data[UnitCount] > 3)
    {
        // apply damage bonus
        score += data[DamageData] / 100.0f;
    }
    // infantry bonus
    if (data[InfantryUnit] == 1.0f)
    {
        if (data[InfantryCount] < 6 && data[BuildingEnemyRatio] < 1.25f)
        {
            score += (5 - data[InfantryCount]) * 5 + (1.25f - data[BuildingEnemyRatio]) / 0.02f;
        }
        else if (data[InfantryUnitRatio] < 0.4f)
        {
            score += (1.25f - data[BuildingEnemyRatio]) / 0.15f * 4;
        }
        else
        {
            score += (1.0f - data[BuildingEnemyRatio]) / 0.15f * 4;
        }
        if (data[InfantryCount] >= 6)
        {
            score -= data[InfantryCount];
        }
    }
    score += calcCostScore(data);
    // apply movement bonus
    score += data[Movementpoints] * 0.33f;
    if (data[UnitCount] > 3)
    {
        // apply not attackable unit bonus
        score += data[NotAttackableCount] * 30.0f;
    }
    if (data[UnitCost] > 0)
    {
        score += data[DamageData] * 20 / data[UnitCost];
    }
    // apply co buff bonus
    score += data[COBonus] * 17;

    if (data[ReachDistance] > 0 && data[Movementpoints] > 0)
    {
        score += 10.0f / Mainapp::roundUp(data[ReachDistance] / data[Movementpoints]);
    }
    return score;
}

float NormalAi::calcCostScore(QVector<float>& data)
{
    float score = 0;
    // funds bonus;
    if (data[FundsFactoryRatio] > 2.5f + data[UnitEnemyRatio])
    {
        score -= (data[FundsFactoryRatio] - 2.5f + data[UnitEnemyRatio]) * 5;
    }
    else if (data[FundsFactoryRatio] < 0.8f)
    {
        score += (0.9f - data[FundsFactoryRatio]) / 0.05f * 2;
    }
    else
    {
        score += (2.5f + data[UnitEnemyRatio] - data[FundsFactoryRatio]) / 0.10f;
    }
    return score;
}
