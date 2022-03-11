#include "qvector4d.h"

#include "resource_management/movementtablemanager.h"

#include "coreengine/mainapp.h"

#include "game/player.h"
#include "game/co.h"
#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"

UnitPathFindingSystem::UnitPathFindingSystem(GameMap* pMap, Unit* pUnit, Player* pPlayer)
    : PathFindingSystem(pUnit->Unit::getX(), pUnit->Unit::getY(),
                        pMap->getMapWidth(),
                        pMap->getMapHeight()),
      m_pUnit(pUnit),
      m_pPlayer(pPlayer),
      m_pMap(pMap)
{
    setObjectName("UnitPathFindingSystem");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    if (m_pPlayer == nullptr)
    {
        m_pPlayer = m_pUnit->getOwner();
    }
    setMovepoints(m_pUnit->getMovementpoints(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY())));
    m_supportsShortCuts = MovementTableManager::getInstance()->getSupportsFastPfs(pUnit->getMovementType());
}

qint32 UnitPathFindingSystem::getRemainingCost(qint32 x, qint32 y, qint32 currentCost)
{    
    if (m_pMap != nullptr && m_pMap->onMap(x, y) && m_Movepoints > 0)
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

qint32 UnitPathFindingSystem::getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY)
{
    qint32 direction = getMoveDirection(curX, curY, x, y);
    if (direction == Directions::Unknown)
    {
        return m_pUnit->getMovementCosts(x, y, x, y);
    }
    else if (m_movecosts[index][direction] == infinite)
    {
        
        if (m_pMap != nullptr && m_pMap->onMap(x, y))
        {
            Terrain* pTerrain = m_pMap->getTerrain(x, y);
            Unit* pUnit = pTerrain->getUnit();
            if (!m_useBasecosts)
            {
                // check for an enemy on the field
                if (pUnit != nullptr)
                {
                    // ignore unit if it's not an enemy unit or if it's stealthed
                    if (m_pUnit->getOwner()->isEnemyUnit(pUnit) &&
                        (!pUnit->isStealthed(m_pPlayer)))
                    {
                        if (!m_pUnit->getIgnoreUnitCollision() &&
                            (m_ignoreEnemies == CollisionIgnore::Off ||
                             (m_ignoreEnemies == CollisionIgnore::OnlyNotMovedEnemies && !pUnit->getHasMoved())))
                        {
                            m_movecosts[index][direction] = -1;
                            return m_movecosts[index][direction];
                        }
                    }
                }
            }
            bool found = false;
            if (m_fast &&
                m_supportsShortCuts &&
                pTerrain->getBuilding() != nullptr)
            {
                QString id = m_pMap->getTerrain(curX, curY)->getID() + m_pMap->getTerrain(x, y)->getID();
                found = m_costInfo.contains(id);
                if (found)
                {
                    qint32 cost = m_costInfo[id];
                    m_movecosts[index][direction] = cost;
                }
                else
                {
                    qint32 cost = m_pUnit->getMovementCosts(x, y, curX, curY);
                    m_costInfo.insert(id, cost);
                    m_movecosts[index][direction] = cost;
                }
            }
            else
            {
                m_movecosts[index][direction] = m_pUnit->getMovementCosts(x, y, curX, curY);
            }
            return m_movecosts[index][direction];
        }
        else
        {
            m_movecosts[index][direction] = -1;
            return m_costs[index];
        }
    }
    return m_movecosts[index][direction];
}

qint32 UnitPathFindingSystem::getCosts(const std::vector<QPoint> & path)
{
    qint32 totalCosts = 0;
    for (qint32 i = path.size() - 2; i >= 0; i--)
    {
        totalCosts += UnitPathFindingSystem::getCosts(getIndex(path[i].x(), path[i].y()), path[i].x(), path[i].y(),
                                                      path[i + 1].x(), path[i + 1].y());
    }
    return totalCosts;
}

std::vector<QPoint> UnitPathFindingSystem::getClosestReachableMovePath(QPoint target, qint32 movepoints, bool direct)
{    
    if (m_pMap != nullptr)
    {
        std::vector<QVector4D> usedNodes;
        std::vector<QVector4D> nextNodes;
        std::vector<QVector4D> currentNodes;
        currentNodes.push_back(QVector4D(target.x(), target.y(), target.x(), target.y()));
        while (currentNodes.size() > 0 || nextNodes.size() > 0)
        {
            if (currentNodes.size() == 0)
            {
                // swap nodes
                currentNodes.insert(currentNodes.end(), nextNodes.begin(), nextNodes.end());
                nextNodes.clear();
            }
            QVector4D currentNode = currentNodes.front();
            currentNodes.erase(currentNodes.cbegin());
            usedNodes.push_back(currentNode);
            qint32 currentCost = getTargetCosts(currentNode.x(), currentNode.y());
            Unit* pNodeUnit = m_pMap->getTerrain(currentNode.x(), currentNode.y())->getUnit();
            // empty field or unit ignores collision and can move on the field
            // or we are on this field
            if (isCrossable(pNodeUnit, currentNode.x(), currentNode.y(), currentNode.z(), currentNode.w(),
                            getTargetCosts(currentNode.x(), currentNode.y()), movepoints))
            {

                return getPathFast(currentNode.x(), currentNode.y());
            }
            else
            {
                // check surrounding nodes
                for (qint32 i = 0; i < 4; i++)
                {
                    QVector4D test;
                    switch (i)
                    {
                        case 0:
                        {
                            test = QVector4D(currentNode.x() + 1, currentNode.y(),
                                             currentNode.x(), currentNode.y());
                            break;
                        }
                        case 1:
                        {
                            test = QVector4D(currentNode.x() - 1, currentNode.y(),
                                             currentNode.x(), currentNode.y());
                            break;
                        }
                        case 2:
                        {
                            test = QVector4D(currentNode.x(), currentNode.y() + 1,
                                             currentNode.x(), currentNode.y());
                            break;
                        }
                        default:
                        {
                            test = QVector4D(currentNode.x(), currentNode.y() - 1,
                                             currentNode.x(), currentNode.y());
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
                        nextNodes.push_back(test);
                    }
                    // add previous nodes if it's a previous one and not used yet
                    else if (testCost >= 0 &&
                             testCost <= currentCost &&
                             !usedNodes.contains(test) &&
                             !nextNodes.contains(test))
                    {
                        nextNodes.push_back(test);
                    }
                }
            }
        }
    }
    return std::vector<QPoint>();
}

std::vector<QPoint> UnitPathFindingSystem::getClosestReachableMovePath(std::vector<QPoint>& path, qint32 movepoints)
{
    if (movepoints > 0 && path.size() > 0)
    {
        std::vector<QPoint> ret;
        std::vector<QPoint> buffer;
        qint32 currentCosts = 0;
        
        QPoint lastValidPoint = path[path.size() - 1];
        ret.push_back(lastValidPoint);
        if (m_pMap != nullptr)
        {
            for (qint32 i = path.size() - 2; i >= 0; i--)
            {
                Unit* pNodeUnit = m_pMap->getTerrain(path[i].x(), path[i].y())->getUnit();
                currentCosts += getCosts(getIndex(path[i].x(), path[i].y()), path[i].x(), path[i].y(),
                                         path[i + 1].x(), path[i + 1].y());
                if (isCrossable(pNodeUnit, path[i].x(), path[i].y(), path[i + 1].x(), path[i + 1].y(), currentCosts, movepoints))
                {
                    lastValidPoint = path[i];
                    for (qint32 i2 = 0; i2 < buffer.size(); i2++)
                    {
                        ret.insert(ret.cbegin(), buffer[i2]);
                    }
                    ret.insert(ret.cbegin(), lastValidPoint);
                    buffer.clear();
                }
                else if (currentCosts > movepoints ||
                         blockedByEnemy(pNodeUnit))
                {
                    break;
                }
                else
                {
                    buffer.push_back(path[i]);
                }
            }
        }
        return ret;
    }
    else if (movepoints == 0 && path.size() > 0)
    {
        return std::vector<QPoint>(1, path[path.size() - 1]);
    }
    else
    {
        return path;
    }
}

bool UnitPathFindingSystem::isCrossable(Unit* pNodeUnit, qint32 x, qint32 y, qint32 curX, qint32 curY, qint32 movementCosts, qint32 movepoints)
{
    if ((pNodeUnit == nullptr || // empty field
         (pNodeUnit == m_pUnit) || // current field
         blockedByEnemy(pNodeUnit)) &&
        (movepoints < 0 || movementCosts <= movepoints) && // inside given cost limits
        (getCosts(getIndex(x, y), x, y, curX, curY) > 0))
    {
        return true;
    }
    return false;
}

bool UnitPathFindingSystem::blockedByEnemy(Unit* pNodeUnit)
{
    if ((m_pUnit->getIgnoreUnitCollision() && pNodeUnit != nullptr && m_pUnit->getOwner()->isEnemyUnit(pNodeUnit)) || // oozium move
        (pNodeUnit != nullptr && !pNodeUnit->isStealthed(m_pPlayer) && m_pUnit->getOwner()->isEnemyUnit(pNodeUnit)))
    {
        return true;
    }
    return false;
}

bool UnitPathFindingSystem::getFast() const
{
    return m_fast;
}

void UnitPathFindingSystem::setFast(bool fast)
{
    m_fast = fast;
}

UnitPathFindingSystem::CollisionIgnore UnitPathFindingSystem::getIgnoreEnemies() const
{
    return m_ignoreEnemies;
}

void UnitPathFindingSystem::setIgnoreEnemies(CollisionIgnore ignoreEnemies)
{
    m_ignoreEnemies = ignoreEnemies;
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

bool UnitPathFindingSystem::getUseBasecosts() const
{
    return m_useBasecosts;
}

void UnitPathFindingSystem::setUseBasecosts(bool useBasecosts)
{
    m_useBasecosts = useBasecosts;
}
