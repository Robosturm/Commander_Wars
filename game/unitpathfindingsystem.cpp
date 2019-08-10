#include "game/unitpathfindingsystem.h"

#include "resource_management/movementtablemanager.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/co.h"

UnitPathFindingSystem::UnitPathFindingSystem(Unit* pUnit, Player* pPlayer)
    : PathFindingSystem(pUnit->getX(), pUnit->getY()),
      m_pUnit(pUnit),
      m_pPlayer(pPlayer),
      m_Movepoints(m_pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY())))
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (m_pPlayer == nullptr)
    {
        m_pPlayer = m_pUnit->getOwner();
    }
}

qint32 UnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        if (m_Movepoints >= 0)
        {
            return m_Movepoints - currentCost;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
}

bool UnitPathFindingSystem::finished(qint32, qint32, qint32)
{
    return false;
}

qint32 UnitPathFindingSystem::getCosts(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        if (!m_pUnit->getIgnoreUnitCollision())
        {
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            // check for an enemy on the field
            if (pUnit != nullptr)
            {
                // ignore unit if it's not an enemy unit or if it's stealthed
                if (m_pUnit->getOwner()->isEnemyUnit(pUnit) &&
                    (!pUnit->isStealthed(m_pPlayer)))
                {
                    return -1;
                }
            }
        }
        return m_pUnit->getMovementCosts(x, y);
    }
    else
    {
        return -1;
    }
}

qint32 UnitPathFindingSystem::getCosts(QVector<QPoint> path)
{
    qint32 totalCosts = 0;
    for (qint32 i = 0; i < path.size(); i++)
    {
        totalCosts += getCosts(path[i].x(), path[i].y());
    }
    return totalCosts;
}

QVector<QPoint> UnitPathFindingSystem::getClosestReachableMovePath(QPoint target)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_ClosedList.size(); i++)
    {
        if ((m_ClosedList[i]->x == target.x()) && (m_ClosedList[i]->y == target.y()))
        {
            QList<Node*> usedNodes;
            QList<Node*> nextNodes;
            QList<Node*> currentNodes;
            currentNodes.append(m_ClosedList[i]);
            while (currentNodes.size() > 0 || nextNodes.size() > 0)
            {
                if (currentNodes.size() == 0)
                {
                    // swap nodes
                    currentNodes.append(nextNodes);
                    nextNodes.clear();
                }
                Node* pCurrentNode = currentNodes.first();
                currentNodes.removeFirst();
                usedNodes.append(pCurrentNode);
                Unit* pNodeUnit = pMap->getTerrain(pCurrentNode->x, pCurrentNode->y)->getUnit();
                // empty field or unit ignores collision and can move on the field
                // or we are on this field
                if (pNodeUnit == nullptr ||
                    (m_pUnit->getIgnoreUnitCollision() && pNodeUnit != nullptr && m_pUnit->getOwner()->isEnemyUnit(pNodeUnit)) ||
                    (pNodeUnit == m_pUnit))
                {
                    return getPath(pCurrentNode->x, pCurrentNode->y);
                }
                else
                {
                    // add previous nodes
                    for (qint32 i2 = 0; i2 < pCurrentNode->previousNodes.size(); i2++)
                    {
                        nextNodes.append(pCurrentNode->previousNodes[i2]);
                    }
                    // add next nodes which we didn't added yet.
                    for (qint32 i2 = 0; i2 < pCurrentNode->nextNodes.size(); i2++)
                    {
                        if (!usedNodes.contains(pCurrentNode->nextNodes[i2]))
                        {
                            nextNodes.append(pCurrentNode->nextNodes[i2]);
                        }
                    }
                }
            }
            break;
        }
    }
    return QVector<QPoint>();
}

void UnitPathFindingSystem::setMovepoints(const qint32 &movepoints)
{
    m_Movepoints = movepoints;
}
