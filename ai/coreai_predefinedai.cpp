#include "ai/coreai.h"

#include "game/unitpathfindingsystem.h"

#include "ai/targetedunitpathfindingsystem.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "game/gameaction.h"

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
    QVector<QPoint> moveTargetFields;
    pAction->setMovepath(QVector<QPoint>(1, QPoint(pUnit->getX(), pUnit->getY())));
    getAttacksFromField(pUnit, pAction, ret, moveTargetFields);
    if (ret.size() > 0)
    {
        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
        QVector3D target = ret[selection];
        CoreAI::addSelectedFieldData(pAction, QPoint(target.x(), target.y()));
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
    QVector<QPoint> moveTargetFields;
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
        if (moveTargetFields[selection] != pAction->getTarget())
        {
            pAction->setMovepath(pfs.getPath(moveTargetFields[selection].x(), moveTargetFields[selection].y()));
        }
        CoreAI::addSelectedFieldData(pAction, QPoint(target.x(), target.y()));
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
    QVector<QPoint> moveTargetFields;
    CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
    bool performed = false;
    if (ret.size() > 0)
    {
        qint32 selection = Mainapp::randInt(0, ret.size() - 1);
        QVector3D target = ret[selection];
        if (moveTargetFields[selection] != pAction->getTarget())
        {
            pAction->setMovepath(pfs.getPath(moveTargetFields[selection].x(), moveTargetFields[selection].y()));
        }
        else
        {
            pAction->setMovepath(QVector<QPoint>());
        }
        CoreAI::addSelectedFieldData(pAction, QPoint(target.x(), target.y()));
        if (pAction->isFinalStep())
        {
            emit performAction(pAction);
            performed = true;
        }
    }
    if (!performed)
    {
        // no target move aggressive to the target field
        QVector<QPoint> targets;
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
