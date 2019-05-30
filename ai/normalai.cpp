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

const float NormalAi::minMovementDamage = 0.3f;

NormalAi::NormalAi()
    : CoreAI (BaseGameInputIF::AiTypes::Normal)
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
        // pUnits->sortShortestMovementRange();
        pEnemyUnits = m_pPlayer->getEnemyUnits();
        pEnemyUnits->randomize();
        pEnemyBuildings = m_pPlayer->getEnemyBuildings();
        pEnemyBuildings->randomize();
        updateEnemyData(pUnits);
        if (useCOPower(pUnits, pEnemyUnits))
        {
            turnMode = TurnTime::onGoingTurn;
        }
        // todo build co unit
        else if (CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
        else if (CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
        else if (captureBuildings(pUnits)){}
        // indirect units
        else if (fireWithUnits(pUnits, 2, std::numeric_limits<qint32>::max())){}
        // direct units
        else if (fireWithUnits(pUnits, 1, 1)){}
        else if (moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 1, 1)){}
        else if (moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings, 2, std::numeric_limits<qint32>::max())){}
        else if (loadUnits(pUnits)){}
        else if (moveTransporters(pUnits, pEnemyUnits, pEnemyBuildings)){}
        else if (moveAwayFromProduction(pUnits)){}
        // todo build units
        else
        {
            clearEnemyData();
            m_IslandMaps.clear();
            turnMode = TurnTime::endOfTurn;
            if (useCOPower(pUnits, pEnemyUnits))
            {
                turnMode = TurnTime::onGoingTurn;
            }
            else
            {
                turnMode = TurnTime::startOfTurn;
                finishTurn();
            }
        }
    }



    delete pBuildings;
    delete pUnits;
    delete pEnemyBuildings;
    delete pEnemyUnits;
}

bool NormalAi::isUsingUnit(Unit* pUnit)
{
    if (pUnit->getMaxFuel() > 0 &&
        pUnit->getFuel() / static_cast<float>(pUnit->getMaxFuel()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getMaxAmmo1() > 0 &&
        pUnit->getAmmo1() / static_cast<float>(pUnit->getMaxAmmo1()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getMaxAmmo2() > 0 &&
        pUnit->getAmmo2() / static_cast<float>(pUnit->getMaxAmmo2()) < 1.0f / 3.0f)
    {
        return false;
    }
    if (pUnit->getHpRounded() < 5)
    {
        return false;
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
                    action.setMovepath(QVector<QPoint>(1, targets[i2]));
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
        GameMap* pMap = GameMap::getInstance();
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
                                if (captureBuildings[i3].x() == captures[i2].x() &&
                                    captureBuildings[i3].y() == captures[i2].y())
                                {
                                    captureCount++;
                                }
                            }
                            if (captureCount == 1)
                            {
                                targetIndex = i2;
                                perform = true;
                                break;
                            }
                        }
                        // check if there unique captures open
                        bool skipUnit = false;
                        for (qint32 i2 = 0; i2 < captureBuildings.size(); i2++)
                        {
                            qint32 captureCount = 0;
                            for (qint32 i3 = 0; i3 < captureBuildings.size(); i3++)
                            {
                                if (captureBuildings[i3].x() == captureBuildings[i2].x() &&
                                    captureBuildings[i3].y() == captureBuildings[i2].y())
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
                            // priorities production buildings over over captures
                            for (qint32 i2 = 0; i2 < captures.size(); i2++)
                            {
                                if (pMap->getTerrain(captures[i2].x(), captures[i2].y())->getBuilding()->getActionList().contains(ACTION_BUILD_UNITS))
                                {
                                    targetIndex = i2;
                                    break;
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
                    pAction->setMovepath(pfs.getPath(captures[targetIndex].x(), captures[targetIndex].y()));
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

bool NormalAi::fireWithUnits(QmlVectorUnit* pUnits, qint32 minfireRange, qint32 maxfireRange)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved() &&
            pUnit->getBaseMaxRange() >= minfireRange &&
            pUnit->getBaseMaxRange() <= maxfireRange &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0))
        {
            GameAction* pAction = new GameAction(ACTION_FIRE);
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
            UnitPathFindingSystem pfs(pUnit);
            pfs.explore();
            QVector<QVector4D> ret;
            QVector<QVector3D> moveTargetFields;
            CoreAI::getAttackTargets(pUnit, pAction, &pfs, ret, moveTargetFields);
            qint32 targetIdx = getBestAttackTarget(pUnit, ret, moveTargetFields);
            if (targetIdx >= 0)
            {
                QVector4D target = ret[targetIdx];
                pAction->setMovepath(pfs.getPath(static_cast<qint32>(moveTargetFields[targetIdx].x()),
                                                 static_cast<qint32>(moveTargetFields[targetIdx].y())));
                CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
                if (GameMap::getInstance()->getTerrain(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()))->getUnit() == nullptr)
                {
                    m_IslandMaps.clear();
                }
                if (pAction->isFinalStep())
                {
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
                         qint32 minfireRange, qint32 maxfireRange)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (isUsingUnit(pUnit) &&
            pUnit->getBaseMaxRange() >= minfireRange &&
            pUnit->getBaseMaxRange() <= maxfireRange)
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            GameAction* pAction = new GameAction(ACTION_CAPTURE);
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
            if (targets.size() == 0)
            {
                appendRepairTargets(pUnit, pBuildings, targets);
            }
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}

bool NormalAi::loadUnits(QmlVectorUnit* pUnits)
{
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
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}

bool NormalAi::moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
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
                if (targets.size() == 0)
                {
                    appendNearestUnloadTargets(pUnit, pEnemyUnits, pEnemyBuildings, targets);
                }
                if (moveToUnloadArea(pAction, pUnit, actions, targets))
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
                appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, targets);
                if (targets.size() == 0)
                {
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, targets);
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

bool NormalAi::moveToUnloadArea(GameAction* pAction, Unit* pUnit, QStringList& actions,
                          QVector<QVector3D>& targets)
{
    GameMap* pMap = GameMap::getInstance();
    Mainapp* pApp = Mainapp::getInstance();
    TargetedUnitPathFindingSystem pfs(pUnit, targets);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        if (CoreAI::contains(targets, targetFields))
        {
            UnitPathFindingSystem turnPfs(pUnit);
            turnPfs.explore();
            pAction->setMovepath(turnPfs.getPath(targetFields.x(), targetFields.y()));
            pAction->setActionID(ACTION_UNLOAD);
            bool unloaded = false;
            QVector<qint32> unloadedUnits;
            do
            {
                unloaded = false;
                QVector<QList<QVariant>> unloadFields;
                for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
                {
                    QString function1 = "getUnloadFields";
                    QJSValueList args1;
                    QJSValue obj1 = pApp->getInterpreter()->newQObject(pAction);
                    args1 << obj1;
                    args1 << i;
                    QJSValue ret = pApp->getInterpreter()->doFunction("ACTION_UNLOAD", function1, args1);
                    unloadFields.append(ret.toVariant().toList());
                }
                MenuData* pDataMenu = pAction->getMenuStepData();
                QStringList actions = pDataMenu->getActionIDs();
                if (actions.size() > 1)
                {
                    for (qint32 i = 0; i < unloadFields.size(); i++)
                    {
                        if (!unloadedUnits.contains(i))
                        {
                            if (unloadFields[i].size() == 1)
                            {
                                qint32 costs = pDataMenu->getCostList()[i];
                                addMenuItemData(pAction, actions[i], costs);
                                MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                                addSelectedFieldData(pAction, pFields->getPoints()->at(0));
                                delete pFields;
                                unloaded = true;
                                unloadedUnits.append(i);
                                break;
                            }
                            else if (unloadFields[i].size() > 0 &&
                                     pUnit->getLoadedUnit(i)->getActionList().contains(ACTION_CAPTURE))
                            {
                                MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                                for (qint32 i2 = 0; i2 < pFields->getPoints()->size(); i2++)
                                {
                                    Building* pBuilding = pMap->getTerrain(pFields->getPoints()->at(i2).x(),
                                                                           pFields->getPoints()->at(i2).y())->getBuilding();
                                    if (pBuilding != nullptr && m_pPlayer->isEnemy(pBuilding->getOwner()))
                                    {
                                        qint32 costs = pDataMenu->getCostList()[i];
                                        addMenuItemData(pAction, actions[i], costs);
                                        addSelectedFieldData(pAction, pFields->getPoints()->at(i2));
                                        unloaded = true;
                                        unloadedUnits.append(i);
                                        break;
                                    }
                                }
                                delete pFields;
                                break;
                            }
                        }
                    }
                    if (unloaded == false)
                    {
                        qint32 costs = pDataMenu->getCostList()[0];
                        addMenuItemData(pAction, actions[0], costs);
                        unloaded = true;
                        MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                        qint32 field = Mainapp::randInt(0, pFields->getPoints()->size() - 1);
                        addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                        delete pFields;
                    }
                }
                delete pDataMenu;
            }
            while (unloaded);
            addMenuItemData(pAction, ACTION_WAIT, 0);
            emit performAction(pAction);
            return true;
        }
        else
        {
            return moveUnit(pAction, pUnit, actions, targets, targets);
        }
    }
    return false;
}

bool NormalAi::moveUnit(GameAction* pAction, Unit* pUnit, QStringList& actions,
                          QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets)
{
    TargetedUnitPathFindingSystem pfs(pUnit, targets);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();
        if (CoreAI::contains(transporterTargets, targetFields))
        {
            pAction->setMovepath(turnPfs.getPath(targetFields.x(), targetFields.y()));
            pAction->setActionID(ACTION_LOAD);
            emit performAction(pAction);
            return true;
        }
        else
        {
           QVector<QPoint> movePath = turnPfs.getClosestReachableMovePath(targetFields);
           qint32 idx = getMoveTargetField(pUnit, movePath);
           if (idx < 0)
           {
                std::tuple<QPoint, float, bool> target = moveToSafety(pUnit, turnPfs, movePath[0]);
                QPoint ret = std::get<0>(target);
                float minDamage = std::get<1>(target);
                bool allEqual = std::get<2>(target);
                if (((ret.x() == pUnit->getX() && ret.y() == pUnit->getY()) ||
                     minDamage > pUnit->getUnitValue() / 2 ||
                     allEqual) && minDamage > 0.0f)
                {
                    // we don't have a good option do the best that we can attack with an all in attack :D
                    pAction->setActionID(ACTION_FIRE);
                    QVector<QVector3D> ret;
                    QVector<QVector3D> moveTargetFields;
                    CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
                    if (ret.size() > 0)
                    {
                        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
                        QVector3D target = ret[selection];
                        pAction->setMovepath(pfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                                             static_cast<qint32>(moveTargetFields[selection].y())));
                        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                                     static_cast<qint32>(target.y())));
                        if (pAction->isFinalStep())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                    pAction->setMovepath(turnPfs.getPath(movePath[0].x(), movePath[0].y()));
                }
                else
                {
                    pAction->setMovepath(turnPfs.getPath(ret.x(), ret.y()));
                }
           }
           else
           {
               pAction->setMovepath(turnPfs.getPath(movePath[idx].x(), movePath[idx].y()));
           }
           if (pAction->getMovePath().size() > 0)
           {

               if (actions.contains(ACTION_RATION))
               {
                   pAction->setActionID(ACTION_RATION);
                   if (pAction->canBePerformed())
                   {
                       emit performAction(pAction);
                       return true;
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
               pAction->setActionID(ACTION_WAIT);
               emit performAction(pAction);
               return true;
           }
        }
    }
    return false;
}

std::tuple<QPoint, float, bool> NormalAi::moveToSafety(Unit* pUnit, UnitPathFindingSystem& turnPfs, QPoint target)
{
    GameMap* pMap = GameMap::getInstance();
    QVector<QPoint> targets = turnPfs.getAllNodePoints();
    QPoint ret(pUnit->getX(), pUnit->getY());
    float leastDamageField = std::numeric_limits<float>::max();
    qint32 shortestDistance = std::numeric_limits<qint32>::max();
    bool allFieldsEqual = true;
    for (qint32 i = 0; i < targets.size(); i++)
    {
        if (pMap->getTerrain(targets[i].x(), targets[i].y())->getUnit() == nullptr)
        {
            float currentDamage = calculateCounterDamage(pUnit, targets[i], nullptr, 0.0f);
            if (leastDamageField < std::numeric_limits<float>::max() &&
                leastDamageField != currentDamage)
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
            else if (currentDamage == leastDamageField && distance < shortestDistance)
            {
                ret = targets[i];
                leastDamageField = currentDamage;
                shortestDistance = distance;
            }
        }
    }
    return std::tuple<QPoint, float, bool>(ret, leastDamageField, allFieldsEqual);
}

qint32 NormalAi::getMoveTargetField(Unit* pUnit, QVector<QPoint>& movePath)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < movePath.size(); i++)
    {
        if (pMap->getTerrain(movePath[i].x(), movePath[i].y())->getUnit() == nullptr)
        {
            float counterDamage = calculateCounterDamage(pUnit, movePath[i], nullptr, 0.0f);
            if (counterDamage < pUnit->getUnitValue() * minMovementDamage)
            {
                return i;
            }
        }
    }
    return -1;
}

qint32 NormalAi::getBestAttackTarget(Unit* pUnit, QVector<QVector4D>& ret, QVector<QVector3D>& moveTargetFields)
{
    GameMap* pMap = GameMap::getInstance();
    qint32 target = -1;
    qint32 currentDamage = 0;
    qint32 deffense = 0;
    qint32 minfireRange = pUnit->getMinRange();
    for (qint32 i = 0; i < ret.size(); i++)
    {
       Unit* pEnemy = pMap->getTerrain(ret[i].x(), ret[i].y())->getUnit();

       qint32 fondsDamage = 0;
       qint32 newHp = 0.0f;
       if (pEnemy != nullptr)
       {
           newHp = pEnemy->getHp() - ret[i].w();
           fondsDamage = ret[i].z() * calculateCaptureBonus(pEnemy, newHp);
           if (minfireRange > 1)
           {
               fondsDamage *= 2.0f;
           }
           if (newHp <= 0)
           {
               fondsDamage *= 2.0f;
           }
           if (pEnemy->getMinRange() > 1)
           {
               fondsDamage *= 2.0f;
           }

       }
       else
       {
           fondsDamage = ret[i].z();
       }
       QPoint moveTarget(moveTargetFields[i].x(), moveTargetFields[i].y());
       fondsDamage -= calculateCounterDamage(pUnit, moveTarget, pEnemy, ret[i].w());
       qint32 targetDefense = pMap->getTerrain(ret[i].x(), ret[i].y())->getDefense(pUnit);
       if (fondsDamage >= 0)
       {
           if (fondsDamage > currentDamage)
           {
               currentDamage = fondsDamage;
               target = i;
               deffense = targetDefense;
           }
           else if (fondsDamage == currentDamage && targetDefense > deffense)
           {
               currentDamage = fondsDamage;
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
    return ret;
}

float NormalAi::calculateCounterDamage(Unit* pUnit, QPoint newPosition, Unit* pEnemyUnit, float enemyTakenDamage)
{
    float counterDamage = 0;
    for (qint32 i = 0; i < m_EnemyUnits.size(); i++)
    {
        spUnit pNextEnemy = m_EnemyUnits[i];
        if (pNextEnemy->getHp() > 0)
        {
            qint32 maxFireRange = pNextEnemy->getMaxRange();
            qint32 minFireRange = pNextEnemy->getMinRange();
            QPoint enemyPos = QPoint(pNextEnemy->getX(), pNextEnemy->getY());
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
                float enemyDamage = m_VirtualEnemyData[i].x();
                if (pNextEnemy.get() == pEnemyUnit)
                {
                    enemyDamage += enemyTakenDamage;
                }
                enemyDamage *= 10.0f;
                QRectF damageData;
                if (distance >= minFireRange && distance <= maxFireRange)
                {
                    damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, enemyPos, pUnit, 0, newPosition);
                }
                else if (canMoveAndFire)
                {
                    QVector<QPoint> targets = m_EnemyPfs[i]->getAllNodePoints();
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        distance = Mainapp::getDistance(newPosition, targets[i2]);
                        if (distance >= minFireRange && distance <= maxFireRange)
                        {
                            damageData = CoreAI::calcVirtuelUnitDamage(pNextEnemy.get(), enemyDamage, targets[i2], pUnit, 0, newPosition);
                            break;
                        }
                    }
                }
                counterDamage += calcFondsDamage(damageData, pNextEnemy.get(), pUnit).y();
            }
        }
    }
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
    for (qint32 i = 0; i < updatePoints.size(); i++)
    {
        for (qint32 i2 = 0; i2 < m_EnemyUnits.size(); i2++)
        {
            if (m_EnemyUnits[i2]->getHp() > 0)
            {
                if (qAbs(updatePoints[i].x() - m_EnemyUnits[i2]->getX()) +
                    qAbs(updatePoints[i].y() - m_EnemyUnits[i2]->getY()) <=
                    m_EnemyUnits[i2]->getMovementpoints(QPoint(m_EnemyUnits[i2]->getX(), m_EnemyUnits[i2]->getY())))
                {
                    m_EnemyPfs[i2] = new UnitPathFindingSystem(m_EnemyUnits[i].get());
                    m_EnemyPfs[i2]->explore();
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
                    m_VirtualEnemyData[i3].setX(m_VirtualEnemyData[i3].x() + damage[i2] / (damage.size() * 2));
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

void NormalAi::serializeObject(QDataStream&)
{
}
void NormalAi::deserializeObject(QDataStream&)
{
}
