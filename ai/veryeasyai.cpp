#include "veryeasyai.h"

#include "game/gameaction.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unitpathfindingsystem.h"

VeryEasyAI::VeryEasyAI()
{

}

void VeryEasyAI::process()
{
    QmlVectorUnit* pUnits = m_pPlayer->getUnits();
    if (!captureBuildings(pUnits))
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

void VeryEasyAI::serialize(QDataStream&)
{
}
void VeryEasyAI::deserialize(QDataStream&)
{
}

