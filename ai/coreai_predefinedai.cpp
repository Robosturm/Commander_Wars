#include "ai/coreai.h"

#include "game/unitpathfindingsystem.h"

#include "ai/targetedunitpathfindingsystem.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "game/gamemap.h"


bool CoreAI::moveOoziums(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits)
{
    //
    QVector<QVector3D> targets;
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pUnit = pEnemyUnits->at(i);
        targets.append(QVector3D(pUnit->getX(), pUnit->getY(), 1));
    }

    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_HOELLIUM_WAIT))
            {
                TargetedUnitPathFindingSystem pfs(pUnit, targets);
                pfs.explore();
                qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
                QPoint targetFields = pfs.getReachableTargetField(movepoints);
                if (targetFields.x() >= 0)
                {
                    UnitPathFindingSystem turnPfs(pUnit);
                    turnPfs.explore();
                    GameAction* pAction = new GameAction(ACTION_HOELLIUM_WAIT);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    pAction->setMovepath(turnPfs.getClosestReachableMovePath(targetFields));
                    emit performAction(pAction);
                    return true;
                }
            }
        }
    }
    return false;
}

bool CoreAI::moveBlackBombs(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits)
{
    GameMap* pMap = GameMap::getInstance();
    QVector<QVector3D> enemyTargets;
    QmlVectorPoint* enemyFields = Mainapp::getCircle(1, 1);
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pUnit = pEnemyUnits->at(i);
        for (qint32 i2 = 0; i2 < enemyFields->size(); i2++)
        {
            if (pMap->onMap(pUnit->getX() + enemyFields->at(i2).x(), pUnit->getY() + enemyFields->at(i2).y()))
            {
                QVector3D point = QVector3D(pUnit->getX() + enemyFields->at(i2).x(), pUnit->getY() + enemyFields->at(i2).y(), 1);
                if (!enemyTargets.contains(point))
                {
                    enemyTargets.append(point);
                }
            }
        }
    }
    delete enemyFields;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_EXPLODE))
            {
                UnitPathFindingSystem turnPfs(pUnit);
                turnPfs.explore();
                QmlVectorPoint* pPoints = Mainapp::getCircle(1, 3);
                QVector<QPoint> targets = turnPfs.getAllNodePoints();
                qint32 maxDamage = 0;
                QVector<QPoint> bestTargets;
                GameAction* pAction = new GameAction(ACTION_EXPLODE);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                for (qint32 i2 = 0; i2 < targets.size(); i2++)
                {
                    qint32 damageDone = m_pPlayer->getRocketTargetDamage(targets[i2].x(), targets[i2].y(), pPoints, 5, 1.2f, GameEnums::RocketTarget_Money);
                    if (damageDone > maxDamage)
                    {
                        bestTargets.clear();
                        bestTargets.append(targets[i2]);
                        maxDamage = damageDone;
                    }
                    else if (damageDone == maxDamage)
                    {
                        bestTargets.append(targets[i2]);
                        maxDamage = damageDone;
                    }
                }
                delete pPoints;
                if (bestTargets.size() > 0 && maxDamage > 0)
                {
                    QPoint target = bestTargets[Mainapp::randInt(0, bestTargets.size() - 1)];
                    pAction->setMovepath(turnPfs.getPath(target.x(), target.y()));
                    addSelectedFieldData(pAction, target);
                    emit performAction(pAction);
                    return true;
                }
                else
                {
                    TargetedUnitPathFindingSystem pfs(pUnit, enemyTargets);
                    pfs.explore();
                    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
                    QPoint targetFields = pfs.getReachableTargetField(movepoints);
                    if (targetFields.x() >= 0)
                    {
                        pAction->setActionID(ACTION_WAIT);
                        pAction->setMovepath(turnPfs.getClosestReachableMovePath(targetFields));
                        emit performAction(pAction);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool CoreAI::processPredefinedAi()
{
    QmlVectorUnit* pUnits = m_pPlayer->getUnits();
    pUnits->randomize();

    QmlVectorUnit* pEnemyUnits = m_pPlayer->getEnemyUnits();
    pEnemyUnits->randomize();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            switch (pUnit->getAiMode())
            {
                case GameEnums::GameAi_Hold:
                {
                    processPredefinedAiHold(pUnit);
                    delete pEnemyUnits;
                    delete pUnits;
                    return true;
                }
                case GameEnums::GameAi_Offensive:
                {
                    processPredefinedAiOffensive(pUnit, pEnemyUnits);
                    delete pEnemyUnits;
                    delete pUnits;
                    return true;
                }
                case GameEnums::GameAi_Defensive:
                {
                    processPredefinedAiDefensive(pUnit);
                    delete pEnemyUnits;
                    delete pUnits;
                    return true;
                }
                case GameEnums::GameAi_Normal:
                {
                    break;
                }
            }
        }
    }
    delete pEnemyUnits;
    delete pUnits;
    return false;
}

void CoreAI::processPredefinedAiHold(Unit* pUnit)
{
    GameAction* pAction = new GameAction(ACTION_FIRE);
    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->getX(), pUnit->getY())));
    getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (ret.size() > 0)
    {
        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
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
    GameAction* pAction = new GameAction(ACTION_FIRE);
    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
    UnitPathFindingSystem pfs(pUnit);
    pfs.explore();
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    float minDamage = -pUnit->getUnitValue() / 4.0f;
    if (minDamage > - 500.0f)
    {
        minDamage = -500.0f;
    }
    if (ret.size() > 0 && ret[0].z() >= minDamage)
    {
        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
        QVector3D target = ret[selection];
        QPoint point = pAction->getTarget();
        if (static_cast<qint32>(moveTargetFields[selection].x()) != point.x() ||
            static_cast<qint32>(moveTargetFields[selection].y()) != point.y())
        {
            pAction->setMovepath(pfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                             static_cast<qint32>(moveTargetFields[selection].y())));
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

void CoreAI::processPredefinedAiOffensive(Unit* pUnit, QmlVectorUnit* pEnemyUnits)
{
    GameAction* pAction = new GameAction(ACTION_FIRE);
    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->getX(), pUnit->getY())));
    UnitPathFindingSystem pfs(pUnit);
    pfs.explore();
    QVector<QVector3D> ret;
    QVector<QVector3D> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    bool performed = false;
    if (ret.size() > 0)
    {
        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
        QVector3D target = ret[selection];
        QPoint point = pAction->getTarget();
        if (static_cast<qint32>(moveTargetFields[selection].x()) != point.x() ||
            static_cast<qint32>(moveTargetFields[selection].y()) != point.y())
        {
            pAction->setMovepath(pfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                             static_cast<qint32>(moveTargetFields[selection].y())));
        }
        else
        {
            pAction->setMovepath(QVector<QPoint>());
        }
        CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()),
                                                     static_cast<qint32>(target.y())));
        if (pAction->isFinalStep())
        {
            emit performAction(pAction);
            performed = true;
        }
    }
    if (!performed)
    {
        // no target move aggressive to the target field
        QVector<QVector3D> targets;
        pAction->setActionID(ACTION_WAIT);
        appendAttackTargets(pUnit, pEnemyUnits, targets);
        TargetedUnitPathFindingSystem targetPfs(pUnit, targets);
        targetPfs.explore();
        qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
        QPoint targetFields = targetPfs.getReachableTargetField(movepoints);
        if (targetFields.x() >= 0)
        {
            pAction->setMovepath(pfs.getClosestReachableMovePath(targetFields));
        }
        else
        {
            pAction->setMovepath(QVector<QPoint>());
        }
        emit performAction(pAction);
    }
}
