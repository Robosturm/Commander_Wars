#include "game/unitpathfindingsystem.h"

#include "resource_management/movementtablemanager.h"

#include "game/gamemap.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/co.h"

UnitPathFindingSystem::UnitPathFindingSystem(Unit* pUnit, Player* pPlayer)
    : PathFindingSystem(pUnit->getX(), pUnit->getY(),
                        GameMap::getInstance()->getMapWidth(),
                        GameMap::getInstance()->getMapHeight()),
      m_pUnit(pUnit),
      m_pPlayer(pPlayer)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (m_pPlayer == nullptr)
    {
        m_pPlayer = m_pUnit->getOwner();
    }
    setMovepoints(m_pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY())));
}

qint32 UnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y) && m_Movepoints > 0)
    {
        return m_Movepoints - currentCost;
    }
    else if (m_Movepoints == -2)
    {
        return 1;
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

qint32 UnitPathFindingSystem::getCosts(qint32 index, qint32 x, qint32 y)
{
    if (movecosts[index] == infinite)
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
                        movecosts[index] = -1;
                        return movecosts[index];
                    }
                }
            }
            movecosts[index] = m_pUnit->getMovementCosts(x, y);
            return movecosts[index];
        }
        else
        {
            movecosts[index] = -1;
            return costs[index];
        }
    }
    return movecosts[index];
}

qint32 UnitPathFindingSystem::getCosts(QVector<QPoint> path)
{
    qint32 totalCosts = 0;
    for (qint32 i = 0; i < path.size() - 1; i++)
    {
        totalCosts += UnitPathFindingSystem::getCosts(getIndex(path[i].x(), path[i].y()), path[i].x(), path[i].y());
    }
    return totalCosts;
}

QVector<QPoint> UnitPathFindingSystem::getClosestReachableMovePath(QPoint target, qint32 movepoints, bool direct)
{
    GameMap* pMap = GameMap::getInstance();
    QList<QPoint> usedNodes;
    QList<QPoint> nextNodes;
    QList<QPoint> currentNodes;
    currentNodes.append(target);
    while (currentNodes.size() > 0 || nextNodes.size() > 0)
    {
        if (currentNodes.size() == 0)
        {
            // swap nodes
            currentNodes.append(nextNodes);
            nextNodes.clear();
        }
        QPoint currentNode = currentNodes.first();
        currentNodes.removeFirst();
        usedNodes.append(currentNode);
        qint32 currentCost = getTargetCosts(currentNode.x(), currentNode.y());
        Unit* pNodeUnit = pMap->getTerrain(currentNode.x(), currentNode.y())->getUnit();
        // empty field or unit ignores collision and can move on the field
        // or we are on this field
        if (isCrossable(pNodeUnit, currentNode.x(), currentNode.y(),
                        getTargetCosts(currentNode.x(), currentNode.y()), movepoints))
        {

            return getPath(currentNode.x(), currentNode.y());
        }
        else
        {
            // check surrounding nodes
            for (qint32 i = 0; i < 4; i++)
            {
                QPoint test;
                switch (i)
                {
                    case 0:
                    {
                        test = QPoint(currentNode.x() + 1,
                                      currentNode.y());
                        break;
                    }
                    case 1:
                    {
                        test = QPoint(currentNode.x() - 1,
                                      currentNode.y());
                        break;
                    }
                    case 2:
                    {
                        test = QPoint(currentNode.x(),
                                      currentNode.y() + 1);
                        break;
                    }
                    default:
                    {
                        test = QPoint(currentNode.x(),
                                      currentNode.y() - 1);
                        break;
                    }
                }
                qint32 testCost = getTargetCosts(test.x(), test.y());
                // add next node if it's more expensive and not added
                if (!direct &&
                    testCost > currentCost &&
                    !usedNodes.contains(test) &&
                    !nextNodes.contains(test))
                {
                    nextNodes.append(test);
                }
                // add previous nodes if it's a previous one and not used yet
                else if (testCost >= 0 &&
                         testCost <= currentCost &&
                         !usedNodes.contains(test) &&
                         !nextNodes.contains(test))
                {
                    nextNodes.append(test);
                }
            }
        }
    }
    return QVector<QPoint>();
}

QVector<QPoint> UnitPathFindingSystem::getClosestReachableMovePath(QVector<QPoint>& path, qint32 movepoints)
{
    if (movepoints > 0 && path.size() > 0)
    {
        QVector<QPoint> ret;
        QVector<QPoint> buffer;
        qint32 currentCosts = 0;
        GameMap* pMap = GameMap::getInstance();
        QPoint lastValidPoint = path[path.size() - 1];
        ret.append(lastValidPoint);
        for (qint32 i = path.size() - 2; i >= 0; i--)
        {
            Unit* pNodeUnit = pMap->getTerrain(path[i].x(), path[i].y())->getUnit();
            currentCosts += getCosts(getIndex(path[i].x(), path[i].y()), path[i].x(), path[i].y());
            if (isCrossable(pNodeUnit, path[i].x(), path[i].y(), currentCosts, movepoints))
            {
                lastValidPoint = path[i];
                for (qint32 i2 = 0; i2 < buffer.size(); i2++)
                {
                    ret.push_front(buffer[i2]);
                }
                ret.push_front(lastValidPoint);
                buffer.clear();
            }
            else if (currentCosts > movepoints)
            {
                break;
            }
            else
            {
                buffer.append(path[i]);
            }
        }
        return ret;
    }
    else if (movepoints == 0 && path.size() > 0)
    {
        return QVector<QPoint>(1, path[path.size() - 1]);
    }
    else
    {
        return path;
    }
}

bool UnitPathFindingSystem::isCrossable(Unit* pNodeUnit, qint32 x, qint32 y, qint32 movementCosts, qint32 movepoints)
{
    if ((pNodeUnit == nullptr || // empty field
         (m_pUnit->getIgnoreUnitCollision() && pNodeUnit != nullptr && m_pUnit->getOwner()->isEnemyUnit(pNodeUnit)) || // oozium move
         (pNodeUnit == m_pUnit)) && // current field
        (getCosts(getIndex(x, y), x, y) > 0) &&
        (movepoints < 0 || movementCosts <= movepoints)) // inside given cost limits
    {
        return true;
    }
    return false;
}

void UnitPathFindingSystem::setMovepoints(const qint32 &movepoints)
{
    m_Movepoints = movepoints;
    if (m_Movepoints > 0)
    {
        qint32 fuel = m_pUnit->getFuel();
        if (fuel >= 0)
        {
            qint32 fuelCosts = m_pUnit->getMovementFuelCostModifier(m_Movepoints) + m_Movepoints;
            while (fuelCosts > fuel && m_Movepoints > 0)
            {
                m_Movepoints--;
                fuelCosts = m_pUnit->getMovementFuelCostModifier(m_Movepoints) + m_Movepoints;
            }
        }
    }
}
