#include "veryeasyai.h"

#include "game/gameaction.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unitpathfindingsystem.h"

VeryEasyAI::VeryEasyAI()
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
}

void VeryEasyAI::process()
{
    QmlVectorUnit* pUnits = m_pPlayer->getUnits();
    // make the ai
    pUnits->randomize();
    if (captureBuildings(pUnits)){}
    else if (fireWithIndirectUnits(pUnits)){}
    else if (fireWithDirectUnits(pUnits)){}
    else
    {
        finishTurn();
    }
    delete pUnits;
}

void VeryEasyAI::finishTurn()
{
    GameAction* pAction = new GameAction(ACTION_NEXT_PLAYER);
    emit performAction(pAction);
}

bool VeryEasyAI::captureBuildings(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_CAPTURE))
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
                    GameAction* pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    UnitPathFindingSystem pfs(pUnit);
                    pfs.explore();
                    QVector<QPoint> targets = pfs.getAllNodePoints();
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        pAction->setMovepath(pfs.getPath(targets[i2].x(), targets[i2].y()));
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

bool VeryEasyAI::fireWithIndirectUnits(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getMaxRange() > 1 &&
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
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getMaxRange() == 1 &&
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
    if (pUnit->getActionList().contains(ACTION_FIRE))
    {
        // try to perform an attack
        GameAction* pAction = new GameAction(ACTION_FIRE);
        pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
        UnitPathFindingSystem pfs(pUnit);
        pfs.explore();
        QVector<QVector3D> ret;
        QVector<QPoint> moveTargetFields;
        CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
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
            pAction->writeDataInt32(static_cast<qint32>(target.x()));
            pAction->writeDataInt32(static_cast<qint32>(target.y()));
            pAction->setInputStep(pAction->getInputStep() + 1);
            if (pAction->isFinalStep())
            {
                emit performAction(pAction);
                return true;
            }
            else
            {
                delete pAction;
            }
        }
        else
        {
            delete pAction;
        }
    }
    return false;
}

void VeryEasyAI::serialize(QDataStream&)
{
}
void VeryEasyAI::deserialize(QDataStream&)
{
}

