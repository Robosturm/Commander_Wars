#include <algorithm>
#include "QTime"

#include "coreengine/interpreter.h"
#include "coreengine/pathfindingsystem.h"
#include "coreengine/mainapp.h"

const qint32 PathFindingSystem::infinite = std::numeric_limits<qint32>::max();

bool operator<(const PathFindingSystem::Node& pNodeLhs, const PathFindingSystem::Node& pNodeRhs)
{
    return (pNodeLhs.totalCost < pNodeRhs.totalCost) || // cheaper end cost
            // same end cost but currently cheaper cost
           (pNodeLhs.currentCosts < pNodeRhs.currentCosts && pNodeLhs.totalCost == pNodeRhs.totalCost) ||
            // same
           (pNodeLhs.distance < pNodeRhs.distance && pNodeLhs.currentCosts == pNodeRhs.currentCosts && pNodeLhs.totalCost == pNodeRhs.totalCost);
}

PathFindingSystem::PathFindingSystem(qint32 startX, qint32 startY,
                                     qint32 width, qint32 heigth)
    : m_StartPoint(startX, startY),
      m_width(width),
      m_heigth(heigth),
      m_costs(new qint32[static_cast<quint32>(width * heigth)]),
      m_DirectionMap(new Directions[static_cast<quint32>(width * heigth)]),
      m_movecosts(new std::array<qint32, Directions::Max>[static_cast<quint32>(width * heigth)])
{
    setObjectName("PathFindingSystem");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    qint32 count = m_width * m_heigth;
    for (qint32 i = 0; i < count; ++i)
    {
        m_costs[i] = infinite;
        m_DirectionMap[i] = Directions::Unknown;
        for (quint32 i2 = 0; i2 < Directions::Max; i2++)
        {
            m_movecosts[i][i2] = infinite;
        }
    }
}

PathFindingSystem::~PathFindingSystem()
{
    delete[] m_costs;
    delete[] m_DirectionMap;
    delete[] m_movecosts;
}

void PathFindingSystem::setStartPoint(qint32 startX, qint32 startY)
{
    m_StartPoint = QPoint(startX, startY);
}

void PathFindingSystem::setFinishNode(qint32 x, qint32 y)
{
    m_FinishNode = x + y  * m_width;
    m_FinishNodeX = x;
    m_FinishNodeY = y;
}

void PathFindingSystem::explore()
{
    Q_ASSERT(m_StartPoint.x() >= 0 && m_StartPoint.y() >= 0
             && m_StartPoint.x() < m_width && m_StartPoint.y() < m_heigth);
    qint32 neighboursIndex = getIndex(m_StartPoint.x(), m_StartPoint.y());
    m_OpenList.append(Node(m_StartPoint.x(), m_StartPoint.y(), neighboursIndex, 0, 0,
                           m_StartPoint.x(), m_StartPoint.y(), 0));
    qint32 remainingCosts;
    qint32 neighboursX = -1;
    qint32 neighboursY = -1;
    qint32 fieldCost = -1;
    qint32 neighboursCosts = -1;
    // explore till we reached the end
    while (!m_OpenList.empty())
    {
        // get current node and pop it
        Node pCurrent = m_OpenList.takeFirst();
        if (pCurrent.index < 0 || m_costs[pCurrent.index] != infinite)
        {
            Q_ASSERT(pCurrent.index >= 0);
            // already searched item
            continue;
        }
        // still the best node?
        qint32 currentCost = pCurrent.currentCosts;
        m_costs[pCurrent.index] = currentCost;
        m_DirectionMap[pCurrent.index] = static_cast<Directions>(getMoveDirection(pCurrent.prevNodeX, pCurrent.prevNodeY,
                                                                                  pCurrent.x, pCurrent.y));
        if (finished(pCurrent.x, pCurrent.y, pCurrent.totalCost))
        {
            setFinishNode(pCurrent.x, pCurrent.y);
            break;
        }
        // right

        for (qint32 i = 0; i < 4; i++)
        {
            // calculate neighbour node data
            if (i == 0)
            {
                if (pCurrent.x + 1 < m_width)
                {
                    neighboursIndex = pCurrent.index + 1;
                    fieldCost = m_costs[neighboursIndex];
                    if (fieldCost != infinite)
                    {
                        // skip searched fields
                        continue;
                    }
                    neighboursX = pCurrent.x + 1;
                    neighboursY = pCurrent.y;
                }
                else
                {
                    continue;
                }
            }
            else if (i == 1)
            {
                // left
                if (pCurrent.x > 0)
                {
                    neighboursIndex = pCurrent.index - 1;
                    fieldCost = m_costs[neighboursIndex];
                    if (fieldCost != infinite)
                    {
                        // skip searched fields
                        continue;
                    }
                    neighboursX = pCurrent.x - 1;
                    neighboursY = pCurrent.y;
                }
                else
                {
                    continue;
                }
            }
            else if (i == 2)
            {
                // bottom
                if (pCurrent.y + 1 < m_heigth)
                {
                    neighboursIndex = pCurrent.index + m_width;
                    fieldCost = m_costs[neighboursIndex];
                    if (fieldCost != infinite)
                    {
                        // skip searched fields
                        continue;
                    }
                    neighboursX = pCurrent.x;
                    neighboursY = pCurrent.y + 1;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                // top
                if (pCurrent.y > 0)
                {
                    neighboursIndex = pCurrent.index - m_width;
                    fieldCost = m_costs[neighboursIndex];
                    if (fieldCost != infinite)
                    {
                        // skip searched fields
                        continue;
                    }
                    neighboursX = pCurrent.x;
                    neighboursY = pCurrent.y - 1;
                }
                else
                {
                    continue;
                }
            }
            // get field costs from index
            neighboursCosts = getCosts(neighboursIndex, neighboursX, neighboursY, pCurrent.x, pCurrent.y);
            if (neighboursCosts >= 0) // passable?
            {
                // costs to reach this field
                qint32 newCosts = neighboursCosts + currentCost;
                remainingCosts = getRemainingCost(neighboursX, neighboursY, newCosts);
                // usable path to target?
                if (remainingCosts >= 0)
                {
                    qint32 totalCost = newCosts + remainingCosts;
                    // node we want to insert
                    Node workNode = Node(neighboursX, neighboursY, neighboursIndex, totalCost, newCosts,
                                         pCurrent.x, pCurrent.y,
                                         qAbs(neighboursX - m_StartPoint.x()) + qAbs(neighboursY - m_StartPoint.y()));
                    m_OpenList.insert(std::upper_bound(m_OpenList.begin(), m_OpenList.end(), workNode), workNode);
                }
            }
        }
    }
}

QVector<QPoint> PathFindingSystem::getFields(qint32 startX, qint32 startY, qint32 min, qint32 max)
{
    QVector<QPoint> points;
    for (qint32 x = -max; x <= max; x++)
    {
        for (qint32 y = -max; y <= max; y++)
        {
            if ((qAbs(x) + qAbs(y) >= min) && (qAbs(x) + qAbs(y) <= max))
            {
                points.append(QPoint(startX + x, startY + y));
            }
        }
    }
    return points;
}

QVector<QPoint> PathFindingSystem::getAllNodePoints(qint32 maxRange)
{
    QVector<QPoint> points;
    for (qint32 x = 0; x < m_width; x++)
    {
        for (qint32 y = 0; y < m_heigth; y++)
        {
            if (m_costs[getIndex(x, y)] >= 0 && m_costs[getIndex(x, y)] < maxRange)
            {
                points.append(QPoint(x, y));
            }
        }
    }
    return points;
}

QmlVectorPoint* PathFindingSystem::getAllQmlVectorPoints()
{
    QmlVectorPoint* ret = new QmlVectorPoint();
    for (qint32 x = 0; x < m_width; x++)
    {
        for (qint32 y = 0; y < m_heigth; y++)
        {
            if (m_costs[getIndex(x, y)] >= 0 && m_costs[getIndex(x, y)] < infinite)
            {
                ret->append(QPoint(x, y));
            }
        }
    }
    return ret;
}

QVector<QPoint> PathFindingSystem::getTargetPath()
{
    if (m_FinishNode >= 0)
    {
        return getPath(m_FinishNodeX, m_FinishNodeY);
    }
    return QVector<QPoint>();
}

QVector<QPoint> PathFindingSystem::getPath(qint32 x, qint32 y)
{
    QVector<QPoint> points;
    qint32 startCost = getTargetCosts(x, y);
    if (startCost >= 0)
    {
        qint32 curX = x;
        qint32 curY = y;
        points.append(QPoint(curX, curY));
        while (curX != m_StartPoint.x() ||
               curY != m_StartPoint.y())
        {
            switch (m_DirectionMap[getIndex(curX, curY)])
            {
                case Directions::East:
                {
                    curX -= 1;
                    break;
                }
                case Directions::West:
                {
                    curX += 1;
                    break;
                }
                case Directions::South:
                {
                    curY -= 1;
                    break;
                }
                case Directions::North:
                {
                    curY += 1;
                    break;
                }
                default:
                {
                    return points;
                }
            }
            points.append(QPoint(curX, curY));
        }
    }
    return points;
}

qint32 PathFindingSystem::getTargetCosts(qint32 x, qint32 y)
{
    if (x >= 0 && x < m_width &&
        y >= 0 && y < m_heigth)
    {
        qint32 cost = m_costs[getIndex(x, y)];
        if (cost < infinite)
        {
            return cost;
        }
    }
    return -1;
}

bool PathFindingSystem::isReachable(qint32 x, qint32 y)
{
    return getTargetCosts(x, y) >= 0;
}
