#include <QCoreApplication>

#include "ai/coreai.h"
#include "ai/targetedunitpathfindingsystem.h"

#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#include "game/unitpathfindingsystem.h"
#include "game/player.h"
#include "game/co.h"
#include "game/unit.h"
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/gamescript.h"


bool CoreAI::moveFlares(spQmlVectorUnit & pUnits)
{
    CONSOLE_PRINT("moveFlares()", GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        pInterpreter->threadProcessEvents();
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_FLARE))
            {
                UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                turnPfs.explore();
                spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FLARE, m_pMap);
                pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                QPoint moveTarget;
                QPoint flareTarget;
                CoreAI::getBestFlareTarget(pUnit, pAction, &turnPfs, flareTarget, moveTarget);
                // found something?
                if (moveTarget.x() >= 0)
                {
                    std::vector<QPoint> path = turnPfs.getPathFast(moveTarget.x(), moveTarget.y());
                    pAction->setMovepath(path, turnPfs.getCosts(path));
                    if (pAction->canBePerformed())
                    {
                        addSelectedFieldData(pAction, flareTarget);
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CoreAI::moveOoziums(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits)
{
    CONSOLE_PRINT("moveOoziums()", GameConsole::eDEBUG);
    std::vector<QVector3D> targets;
    for (auto & pUnit : pEnemyUnits->getVector())
    {
        targets.push_back(QVector3D(pUnit->Unit::getX(), pUnit->Unit::getY(), 1));
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        pInterpreter->threadProcessEvents();
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_HOELLIUM_WAIT))
            {
                TargetedUnitPathFindingSystem pfs(m_pMap, pUnit, targets, &m_MoveCostMap);
                pfs.explore();
                qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                QPoint targetFields = pfs.getReachableTargetField(movepoints);
                if (targetFields.x() >= 0)
                {
                    UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                    turnPfs.explore();
                    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_HOELLIUM_WAIT, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    auto path = turnPfs.getClosestReachableMovePath(targetFields, movepoints);
                    pAction->setMovepath(path, turnPfs.getCosts(path));
                    if (pAction->canBePerformed())
                    {
                        emit sigPerformAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CoreAI::moveBlackBombs(spQmlVectorUnit & pUnits, spQmlVectorUnit & pEnemyUnits)
{
    CONSOLE_PRINT("moveBlackBombs()", GameConsole::eDEBUG);
    
    if (m_pMap != nullptr)
    {
        std::vector<QVector3D> enemyTargets;
        spQmlVectorPoint enemyFields = GlobalUtils::getSpCircle(1, 1);
        for (auto & spUnit : pEnemyUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            for (auto & field : enemyFields->getVector())
            {
                if (m_pMap->onMap(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y()))
                {
                    QVector3D point = QVector3D(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y(), 1);
                    if (!GlobalUtils::contains(enemyTargets, point))
                    {
                        enemyTargets.push_back(point);
                    }
                }
            }
        }
        Interpreter* pInterpreter = Interpreter::getInstance();
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            pInterpreter->threadProcessEvents();
            if (!pUnit->getHasMoved())
            {
                if (pUnit->getActionList().contains(ACTION_EXPLODE))
                {
                    UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                    turnPfs.explore();
                    spQmlVectorPoint pPoints = GlobalUtils::getSpCircle(1, 3);
                    auto targets = turnPfs.getAllNodePointsFast();
                    qint32 maxDamage = 0;
                    std::vector<QPoint> bestTargets;
                    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_EXPLODE, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    for (auto & target : targets)
                    {
                        qint32 damageDone = m_pPlayer->getRocketTargetDamage(target.x(), target.y(), pPoints.get(), 5, 1.2f, GameEnums::RocketTarget_Money, true);
                        if (damageDone > maxDamage)
                        {
                            bestTargets.clear();
                            bestTargets.push_back(target);
                            maxDamage = damageDone;
                        }
                        else if (damageDone == maxDamage)
                        {
                            bestTargets.push_back(target);
                            maxDamage = damageDone;
                        }
                    }
                    if (bestTargets.size() > 0 && maxDamage > 0)
                    {
                        QPoint target = bestTargets[GlobalUtils::randIntBase(0, bestTargets.size() - 1)];
                        auto path = turnPfs.getPathFast(target.x(), target.y());
                        pAction->setMovepath(path, turnPfs.getCosts(path));
                        addSelectedFieldData(pAction, target);
                        if (pAction->canBePerformed())
                        {
                            emit sigPerformAction(pAction);
                            return true;
                        }
                    }
                    else
                    {
                        TargetedUnitPathFindingSystem pfs(m_pMap, pUnit, enemyTargets, &m_MoveCostMap);
                        pfs.explore();
                        qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                        QPoint targetFields = pfs.getReachableTargetField(movepoints);
                        if (targetFields.x() >= 0)
                        {
                            pAction->setActionID(ACTION_WAIT);
                            auto path = turnPfs.getClosestReachableMovePath(targetFields, movepoints);
                            pAction->setMovepath(path, turnPfs.getCosts(path));
                            if (pAction->canBePerformed())
                            {
                                emit sigPerformAction(pAction);
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

bool CoreAI::moveSupport(AISteps step, spQmlVectorUnit & pUnits, bool useTransporters)
{
    CONSOLE_PRINT("CoreAI::moveSupport", GameConsole::eDEBUG);
    m_aiStep = step;
    
    if (m_pMap != nullptr)
    {
        std::vector<QVector3D> unitTargets;
        std::vector<QPoint> unitPos;
        spQmlVectorPoint unitFields = GlobalUtils::getSpCircle(1, 1);
        Interpreter* pInterpreter = Interpreter::getInstance();
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            pInterpreter->threadProcessEvents();
            if (pUnit->getHpRounded() < Unit::MAX_UNIT_HP && pUnit->getUnitCosts() / Unit::MAX_UNIT_HP <= m_pPlayer->getFunds())
            {
                for (auto & field : unitFields->getVector())
                {
                    if (m_pMap->onMap(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y()) &&
                        m_pMap->getTerrain(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y())->getUnit() == nullptr)
                    {
                        QVector3D point = QVector3D(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y(), 1);
                        if (!GlobalUtils::contains(unitTargets, point))
                        {
                            unitTargets.push_back(point);
                            unitPos.push_back(pUnit->getPosition());
                        }
                    }
                }
            }
        }
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            for (auto & field : unitFields->getVector())
            {
                if (m_pMap->onMap(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y()) &&
                    m_pMap->getTerrain(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y())->getUnit() == nullptr)
                {
                    QVector3D point = QVector3D(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y(), 1);
                    if (!GlobalUtils::contains(unitTargets, point))
                    {
                        unitTargets.push_back(point);
                        unitPos.push_back(pUnit->getPosition());
                    }
                }
            }
        }
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            if (!pUnit->getHasMoved() &&
                (pUnit->getLoadedUnitCount() == 0) &&
                (pUnit->getLoadingPlace() == 0 || useTransporters))
            {
                QStringList actions = pUnit->getActionList();
                for (const auto& action : actions)
                {
                    spGameAction pAction = MemoryManagement::create<GameAction>(action, m_pMap);
                    if (action.startsWith(ACTION_SUPPORTSINGLE) ||
                        action.startsWith(ACTION_SUPPORTALL))
                    {
                        UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                        turnPfs.explore();
                        auto targets = turnPfs.getAllNodePointsFast();
                        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                        for (auto & target : targets)
                        {
                            qint32 index = CoreAI::index(unitTargets, target);
                            if (index >= 0 && pUnit->getPosition() != unitPos[index])
                            {
                                if (action.startsWith(ACTION_SUPPORTSINGLE))
                                {
                                    addSelectedFieldData(pAction, unitPos[index]);
                                }
                                auto path = turnPfs.getPathFast(target.x(), target.y());
                                pAction->setMovepath(path, turnPfs.getCosts(path));
                                if (pAction->canBePerformed())
                                {
                                    emit sigPerformAction(pAction);
                                    return true;
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

bool CoreAI::processPredefinedAi()
{
    if (!m_usedPredefinedAi)
    {
        CONSOLE_PRINT("processPredefinedAi()", GameConsole::eDEBUG);
        spQmlVectorUnit pUnits = m_pPlayer->getSpUnits();
        pUnits->randomize();
        spQmlVectorUnit pEnemyUnits = m_pPlayer->getSpEnemyUnits();
        pEnemyUnits->randomize();
        spQmlVectorBuilding pEnemyBuildings = m_pPlayer->getSpEnemyBuildings();
        pEnemyBuildings->randomize();
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->threadProcessEvents();
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            if (pUnit!= nullptr && !pUnit->getHasMoved())
            {
                switch (pUnit->getAiMode())
                {
                    case GameEnums::GameAi_Hold:
                    {
                        if (processPredefinedAiHold(pUnit))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_Offensive:
                    {
                        if (processPredefinedAiOffensive(pUnit, pEnemyUnits))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_Defensive:
                    {
                        if (processPredefinedAiDefensive(pUnit))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_Patrol:
                    case GameEnums::GameAi_PatrolLoop:
                    {
                        if (processPredefinedAiPatrol(pUnit))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_TargetEnemyHq:
                    {
                        if (processPredefinedAiTargetEnemyHq(pUnit, pEnemyBuildings))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_Scripted:
                    {
                        if (processPredefinedGenericScripted(pUnit, pEnemyUnits, pEnemyBuildings))
                        {
                            return true;
                        }
                        break;
                    }
                    case GameEnums::GameAi_Normal:
                    {
                        if (processPredefinedGenericScripted(pUnit, pEnemyUnits, pEnemyBuildings))
                        {
                            return true;
                        }
                        break;
                    }
                }
            }
        }
        m_usedPredefinedAi = true;
    }
    return false;    
}

bool CoreAI::processPredefinedGenericScripted(Unit* pUnit, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    const QString function1 = "GenericPredefinedUnitMapScriptBehaviour";
    if (pInterpreter->exists(GameScript::m_scriptName, function1))
    {
        spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_WAIT, m_pMap);
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(pAction.get()),
                           JsThis::getJsThis(pUnit),
                           JsThis::getJsThis(pEnemyUnits.get()),
                           JsThis::getJsThis(pEnemyBuildings.get()),
                           JsThis::getJsThis(m_pMap)});
        QJSValue erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
        if (erg.isBool() && erg.toBool())
        {
            if (pAction->canBePerformed())
            {
                emit sigPerformAction(pAction);
                return true;
            }
        }
    }
    if (pInterpreter->exists(m_aiName, function1))
    {
        spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_WAIT, m_pMap);
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(pAction.get()),
                           JsThis::getJsThis(pUnit),
                           JsThis::getJsThis(pEnemyUnits.get()),
                           JsThis::getJsThis(pEnemyBuildings.get()),
                           JsThis::getJsThis(m_pMap)});
        QJSValue erg = pInterpreter->doFunction(m_aiName, function1, args);
        if (erg.isBool() && erg.toBool())
        {
            if (pAction->canBePerformed())
            {
                emit sigPerformAction(pAction);
                return true;
            }
        }
    }

    return false;

}

bool CoreAI::processPredefinedMapScripted(Unit* pUnit, spQmlVectorUnit & pEnemyUnits, spQmlVectorBuilding & pEnemyBuildings)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    const QString function1 = "PredefinedUnitMapScriptBehaviour";
    if (pInterpreter->exists(GameScript::m_scriptName, function1))
    {
        spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_WAIT, m_pMap);
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(pAction.get()),
                           JsThis::getJsThis(pUnit),
                           JsThis::getJsThis(pEnemyUnits.get()),
                           JsThis::getJsThis(pEnemyBuildings.get()),
                           JsThis::getJsThis(m_pMap)});
        QJSValue erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
        if (erg.isBool() && erg.toBool())
        {
            if (pAction->canBePerformed())
            {
                emit sigPerformAction(pAction);
                return true;
            }
        }
    }
    return false;
}

bool CoreAI::processPredefinedAiHold(Unit* pUnit)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiHold", GameConsole::eDEBUG);
    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FIRE, m_pMap);
    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    std::vector<QVector3D> ret;
    std::vector<QVector3D> moveTargetFields;
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())), 0);
    getBestAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (ret.size() > 0)
    {
        qint32 selection = GlobalUtils::randIntBase(0, ret.size() - 1);
        QVector3D target = ret[selection];
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep())
        {
            emit sigPerformAction(pAction);
            return true;
        }
        else
        {
            pAction->setActionID(ACTION_WAIT);
            emit sigPerformAction(pAction);
            return true;
        }
    }
    else
    {
        pAction->setActionID(ACTION_WAIT);
        emit sigPerformAction(pAction);
        return true;
    }
    return false;
}

bool CoreAI::processPredefinedAiDefensive(Unit* pUnit)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiDefensive", GameConsole::eDEBUG);
    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FIRE, m_pMap);
    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    UnitPathFindingSystem pfs(m_pMap, pUnit);
    pfs.explore();
    std::vector<QVector3D> ret;
    std::vector<QVector3D> moveTargetFields;
    getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    float minDamage = -static_cast<float>(pUnit->getCoUnitValue()) / 4.0f;
    if (minDamage > - 500.0f)
    {
        minDamage = -500.0f;
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
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep())
        {
            emit sigPerformAction(pAction);
            return true;
        }
        else
        {
            pAction->setActionID(ACTION_WAIT);            
            emit sigPerformAction(pAction);
            return true;
        }
    }
    else
    {
        pAction->setActionID(ACTION_WAIT);
        emit sigPerformAction(pAction);
        return true;
    }    
    return false;
}

bool CoreAI::processPredefinedAiOffensive(Unit* pUnit, spQmlVectorUnit & pEnemyUnits)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiOffensive", GameConsole::eDEBUG);
    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FIRE, m_pMap);
    UnitPathFindingSystem pfs(m_pMap, pUnit);
    pfs.explore();
    bool performed = processPredefinedAiAttack(pUnit, pAction,  pfs);
    if (!performed)
    {
        // no target move aggressive to the target field
        std::vector<QVector3D> targets;
        pAction->setActionID(ACTION_WAIT);
        appendAttackTargets(pUnit, pEnemyUnits, targets);
        TargetedUnitPathFindingSystem targetPfs(m_pMap, pUnit, targets, &m_MoveCostMap);
        targetPfs.explore();
        qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QPoint targetFields = targetPfs.getReachableTargetField(movepoints);
        if (targetFields.x() >= 0)
        {
            std::vector<QPoint> path = pfs.getClosestReachableMovePath(targetFields);
            pAction->setMovepath(path, pfs.getCosts(path));
            emit sigPerformAction(pAction);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

bool CoreAI::processPredefinedAiAttack(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem & pfs)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiAttack", GameConsole::eDEBUG);
    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())), 0);

    std::vector<QVector3D> ret;
    std::vector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    bool performed = false;
    if (ret.size() > 0)
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
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep())
        {
            emit sigPerformAction(pAction);
            performed = true;
        }
    }
    return performed;
}

bool CoreAI::processPredefinedAiPatrol(Unit* pUnit)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiPatrol", GameConsole::eDEBUG);
    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FIRE, m_pMap);
    UnitPathFindingSystem pfs(m_pMap, pUnit);
    pfs.explore();
    bool performed = processPredefinedAiAttack(pUnit, pAction,  pfs);
    if (!performed)
    {
        std::vector<QVector3D> targets;
        pAction->setActionID(ACTION_WAIT);
        auto path = pUnit->getAiMovePath();
        if (path.size() > 0)
        {
            QPoint nextTarget = path[0];
            targets.push_back(QVector3D(nextTarget.x(), nextTarget.y(), 1));
            TargetedUnitPathFindingSystem targetPfs(m_pMap, pUnit, targets, &m_MoveCostMap);
            targetPfs.explore();
            qint32 movepoints = pUnit->getMovementpoints(pUnit->getPosition());
            QPoint targetFields = targetPfs.getReachableTargetField(movepoints);
            if (targetFields.x() >= 0)
            {
                auto path = pfs.getClosestReachableMovePath(targetFields, movepoints);
                pAction->setMovepath(path, pfs.getCosts(path));
                if (path[0] == nextTarget)
                {
                    pUnit->removeFirstAiMovePathPoint();
                    if (pUnit->getAiMode() == GameEnums::GameAi_PatrolLoop)
                    {
                        pUnit->addAiMovePathPoint(nextTarget);
                    }
                }
            }
            else
            {
                pAction->setMovepath(QVector<QPoint>(), 0);
            }
        }
        else
        {
            pAction->setMovepath(QVector<QPoint>(), 0);
        }
        emit sigPerformAction(pAction);
        return true;
    }
    else
    {
        return true;
    }
}

bool CoreAI::processPredefinedAiTargetEnemyHq(Unit* pUnit, spQmlVectorBuilding & pEnemyBuildings)
{
    CONSOLE_PRINT("CoreAI::processPredefinedAiOffensive", GameConsole::eDEBUG);
    spGameAction pAction = MemoryManagement::create<GameAction>(ACTION_FIRE, m_pMap);
    UnitPathFindingSystem pfs(m_pMap, pUnit);
    pfs.explore();
    bool performed = processPredefinedAiAttack(pUnit, pAction,  pfs);
    if (!performed)
    {
        // no target move aggressive to the target field
        std::vector<QVector3D> targets;
        pAction->setActionID(ACTION_CAPTURE);
        for (const auto & pBuilding : pEnemyBuildings->getVector())
        {
            if (pBuilding->getBuildingID() == CoreAI::BUILDING_HQ)
            {
                targets.push_back(QVector3D(pBuilding->getX(), pBuilding->getY(), 1));
            }
        }
        TargetedUnitPathFindingSystem targetPfs(m_pMap, pUnit, targets, &m_MoveCostMap);
        targetPfs.explore();
        qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QPoint targetFields = targetPfs.getReachableTargetField(movepoints);
        if (targetFields.x() >= 0)
        {
            std::vector<QPoint> path = pfs.getClosestReachableMovePath(targetFields);
            pAction->setMovepath(path, pfs.getCosts(path));

            if (pAction->canBePerformed())
            {
                emit sigPerformAction(pAction);
            }
            else
            {
                pAction->setActionID(ACTION_WAIT);
                emit sigPerformAction(pAction);
            }
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}
