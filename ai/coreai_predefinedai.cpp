#include <QApplication>

#include "ai/coreai.h"
#include "ai/targetedunitpathfindingsystem.h"

#include "coreengine/console.h"
#include "coreengine/globalutils.h"

#include "game/unitpathfindingsystem.h"
#include "game/player.h"
#include "game/co.h"
#include "game/unit.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

bool CoreAI::moveFlares(spQmlVectorUnit & pUnits)
{
    AI_CONSOLE_PRINT("moveFlares()", Console::eDEBUG);
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        QApplication::processEvents();
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_FLARE))
            {
                UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                turnPfs.explore();
                spGameAction pAction = spGameAction::create(ACTION_FLARE, m_pMap);
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
                        emit performAction(pAction);
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
    AI_CONSOLE_PRINT("moveOoziums()", Console::eDEBUG);
    std::vector<QVector3D> targets;
    for (auto & pUnit : pEnemyUnits->getVector())
    {
        targets.push_back(QVector3D(pUnit->Unit::getX(), pUnit->Unit::getY(), 1));
    }
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        QApplication::processEvents();
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
                    spGameAction pAction = spGameAction::create(ACTION_HOELLIUM_WAIT, m_pMap);
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    auto path = turnPfs.getClosestReachableMovePath(targetFields);
                    pAction->setMovepath(path, turnPfs.getCosts(path));
                    if (pAction->canBePerformed())
                    {
                        emit performAction(pAction);
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
    AI_CONSOLE_PRINT("moveBlackBombs()", Console::eDEBUG);
    
    if (m_pMap != nullptr)
    {
        std::vector<QVector3D> enemyTargets;
        spQmlVectorPoint enemyFields = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        for (auto & spUnit : pEnemyUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            for (auto & field : enemyFields->getVector())
            {
                if (m_pMap->onMap(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y()))
                {
                    QVector3D point = QVector3D(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y(), 1);
                    if (!enemyTargets.contains(point))
                    {
                        enemyTargets.push_back(point);
                    }
                }
            }
        }
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            QApplication::processEvents();
            if (!pUnit->getHasMoved())
            {
                if (pUnit->getActionList().contains(ACTION_EXPLODE))
                {
                    UnitPathFindingSystem turnPfs(m_pMap, pUnit);
                    turnPfs.explore();
                    spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(1, 3));
                    auto targets = turnPfs.getAllNodePointsFast();
                    qint32 maxDamage = 0;
                    std::vector<QPoint> bestTargets;
                    spGameAction pAction = spGameAction::create(ACTION_EXPLODE, m_pMap);
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
                            emit performAction(pAction);
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
                            auto path = turnPfs.getClosestReachableMovePath(targetFields);
                            pAction->setMovepath(path, turnPfs.getCosts(path));
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
    return false;
}

bool CoreAI::moveSupport(AISteps step, spQmlVectorUnit & pUnits, bool useTransporters)
{
    AI_CONSOLE_PRINT("CoreAI::moveSupport", Console::eDEBUG);
    m_aiStep = step;
    
    if (m_pMap != nullptr)
    {
        std::vector<QVector3D> unitTargets;
        std::vector<QPoint> unitPos;
        spQmlVectorPoint unitFields = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
        for (auto & spUnit : pUnits->getVector())
        {
            Unit* pUnit = spUnit.get();
            QApplication::processEvents();
            if (pUnit->getHpRounded() < Unit::MAX_UNIT_HP && pUnit->getUnitCosts() / Unit::MAX_UNIT_HP <= m_pPlayer->getFunds())
            {
                for (auto & field : unitFields->getVector())
                {
                    if (m_pMap->onMap(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y()) &&
                        m_pMap->getTerrain(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y())->getUnit() == nullptr)
                    {
                        QVector3D point = QVector3D(pUnit->Unit::getX() + field.x(), pUnit->Unit::getY() + field.y(), 1);
                        if (!unitTargets.contains(point) )
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
                    if (!unitTargets.contains(point) )
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
                    spGameAction pAction = spGameAction::create(action, m_pMap);
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
    return false;
}

bool CoreAI::processPredefinedAi()
{
    AI_CONSOLE_PRINT("processPredefinedAi()", Console::eDEBUG);
    spQmlVectorUnit pUnits = spQmlVectorUnit(m_pPlayer->getUnits());
    pUnits->randomize();
    spQmlVectorUnit pEnemyUnits = spQmlVectorUnit(m_pPlayer->getEnemyUnits());
    pEnemyUnits->randomize();
    for (auto & spUnit : pUnits->getVector())
    {
        Unit* pUnit = spUnit.get();
        QApplication::processEvents();
        if (!pUnit->getHasMoved())
        {
            switch (pUnit->getAiMode())
            {
                case GameEnums::GameAi_Hold:
                {
                    processPredefinedAiHold(pUnit);
                    return true;
                }
                case GameEnums::GameAi_Offensive:
                {
                    processPredefinedAiOffensive(pUnit, pEnemyUnits);
                    return true;
                }
                case GameEnums::GameAi_Defensive:
                {
                    processPredefinedAiDefensive(pUnit);
                    return true;
                }
                case GameEnums::GameAi_Patrol:
                case GameEnums::GameAi_PatrolLoop:
                {
                    processPredefinedAiPatrol(pUnit);
                    return true;
                }
                case GameEnums::GameAi_Normal:
                {
                    break;
                }
            }
        }
    }
    return false;
}

void CoreAI::processPredefinedAiHold(Unit* pUnit)
{
    AI_CONSOLE_PRINT("CoreAI::processPredefinedAiHold", Console::eDEBUG);
    spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
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
            emit performAction(pAction);
        }
        else
        {
            pAction->setActionID(ACTION_WAIT);
            emit performAction(pAction);
        }
    }
    else
    {
        pAction->setActionID(ACTION_WAIT);
        emit performAction(pAction);
    }
}

void CoreAI::processPredefinedAiDefensive(Unit* pUnit)
{
    AI_CONSOLE_PRINT("CoreAI::processPredefinedAiDefensive", Console::eDEBUG);
    spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
    UnitPathFindingSystem pfs(m_pMap, pUnit);
    pfs.explore();
    std::vector<QVector3D> ret;
    std::vector<QVector3D> moveTargetFields;
    getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    float minDamage = -pUnit->getCoUnitValue() / 4.0f;
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
            emit performAction(pAction);
        }
        else
        {
            pAction->setActionID(ACTION_WAIT);
            emit performAction(pAction);
        }
    }
    else
    {
        pAction->setActionID(ACTION_WAIT);
        emit performAction(pAction);
    }

}

void CoreAI::processPredefinedAiOffensive(Unit* pUnit, spQmlVectorUnit & pEnemyUnits)
{
    AI_CONSOLE_PRINT("CoreAI::processPredefinedAiOffensive", Console::eDEBUG);
    spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
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
            emit performAction(pAction);
        }
    }
}

bool CoreAI::processPredefinedAiAttack(Unit* pUnit, spGameAction & pAction, UnitPathFindingSystem & pfs)
{
    AI_CONSOLE_PRINT("CoreAI::processPredefinedAiAttack", Console::eDEBUG);
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
            emit performAction(pAction);
            performed = true;
        }
    }
    return performed;
}

void CoreAI::processPredefinedAiPatrol(Unit* pUnit)
{
    AI_CONSOLE_PRINT("CoreAI::processPredefinedAiPatrol", Console::eDEBUG);
    spGameAction pAction = spGameAction::create(ACTION_FIRE, m_pMap);
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
            qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
            QPoint targetFields = targetPfs.getReachableTargetField(movepoints);
            if (targetFields.x() >= 0)
            {
                auto path = pfs.getClosestReachableMovePath(targetFields);
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
        emit performAction(pAction);
    }
}
