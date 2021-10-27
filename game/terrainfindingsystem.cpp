#include "game/terrainfindingsystem.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"

TerrainFindingSystem::TerrainFindingSystem(QString terrainID, qint32 startX, qint32 startY)
    : PathFindingSystem(startX, startY,
                        GameMap::getInstance()->getMapWidth(),
                        GameMap::getInstance()->getMapHeight()),
      m_terrainIDs(terrainID)
{
    setObjectName("TerrainFindingSystem");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

TerrainFindingSystem::TerrainFindingSystem(QStringList terrainIDs, qint32 startX, qint32 startY)
    : PathFindingSystem(startX, startY,
                        GameMap::getInstance()->getMapWidth(),
                        GameMap::getInstance()->getMapHeight()),
      m_terrainIDs(terrainIDs)
{
    setObjectName("TerrainFindingSystem");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

qint32 TerrainFindingSystem::getRemainingCost(qint32, qint32, qint32)
{
    return 1;
}

bool TerrainFindingSystem::finished(qint32, qint32, qint32)
{
    return false;
}

qint32 TerrainFindingSystem::getCosts(qint32 index, qint32 x, qint32 y, qint32, qint32)
{
    if (m_movecosts[index][0] == infinite)
    {
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr && pMap->onMap(x, y))
        {
            if (m_terrainIDs.contains(pMap->getTerrain(x, y)->getID()))
            {
                m_movecosts[index][0] = 1;
                return m_movecosts[index][0];
            }
        }
        m_movecosts[index][0] = -1;
        return m_movecosts[index][0];
    }
    else
    {
        return m_movecosts[index][0];
    }
}

void TerrainFindingSystem::killTerrainFindingSystem()
{
    deleteLater();
}

TerrainFlowData* TerrainFindingSystem::getFlowData()
{
    CONSOLE_PRINT("getFlowData() starting at X=" + QString::number(m_StartPoint.x()) +
                  " Y=" + QString::number(m_StartPoint.y()), Console::eDEBUG);
    spQmlVectorPoint circle(GlobalUtils::getCircle(1, 1));
    const qint32 size = circle->size();
    QVector<PositionFlowData> flowList;
    // determine start search pattern
    addStartFlows(circle, size, flowList);
    while (flowList.size() > 0)
    {
        // add current to flow list
        auto current = flowList.front();
        flowList.pop_front();
        qint32 flowSource = m_data.size();
        m_data.addFlowDirection(current.flowListSource, current.flowDirection);        
        bool exists = m_data.addData(current.next, current.cost, current.flowDirection);
        if (exists)
        {
            continue;
        }
        // remove items with the same target field as us from the list
        qint32 i2 = 0;
        while (i2 < flowList.size())
        {
            if (flowList[i2].next == current.next)
            {
                flowList.removeAt(i2);
            }
            else
            {
                ++i2;
            }
        }
        // add new neighbours
        bool initialAdded = false;
        qint32 offset = 0;
        switch (getDirection(current.current, current.next))
        {
            case GameEnums::FlowDirections_East:
            {
                offset = 1;
                break;
            }
            case GameEnums::FlowDirections_West:
            {
                offset = 3;
                break;
            }
            case GameEnums::FlowDirections_South:
            {
                offset = 0;
                break;
            }
            case GameEnums::FlowDirections_North:
            {
                offset = 2;
                break;
            }
            default:
            {
                break;
            }
        }

        for (qint32 i = 0; i < size; ++i)
        {
            QPoint pos = current.next + circle->at((i + offset) % size);
            qint32 cost = getTargetCosts(pos.x(), pos.y());
            if (cost > current.cost)
            {
                PositionFlowData nextData;
                nextData.current = current.next;
                nextData.next = pos;
                nextData.cost = cost;
                nextData.flowListSource = flowSource;
                nextData.flowDown = current.flowDown;
                nextData.flowDirection = getDirection(current.next, pos, current.flowDirection, current.flowDown);
                if (initialAdded)
                {
                    flowList.append(nextData);
                }
                else
                {
                    flowList.push_front(nextData);
                    initialAdded = true;
                }
            }
        }
    }
    m_data.print();
    return &m_data;
}

void TerrainFindingSystem::addStartFlows(const spQmlVectorPoint & circle, const qint32 size,
                                         QVector<PositionFlowData> & flowList)
{
    for (qint32 i = 0; i < size; ++i)
    {
        QPoint pos = m_StartPoint + circle->at(i);
        qint32 cost = getTargetCosts(pos.x(), pos.y());
        if (cost >= 0)
        {
            PositionFlowData startData;
            startData.current = m_StartPoint;
            startData.next = pos;
            startData.cost = cost;
            startData.flowListSource = 0;
            if (flowList.size() == 0)
            {
                startData.flowDirection = getDirection(startData.current, startData.next);
                m_data.addData(m_StartPoint, 0, startData.flowDirection);
            }
            else
            {
                if (pos.x() != m_StartPoint.x())
                {
                    if (getTargetCosts(m_StartPoint.x(), m_StartPoint.y() - 1) > 0)
                    {
                        startData.flowDirection = GameEnums::FlowDirections_East;
                    }
                    else if (pos.x() < m_StartPoint.x())
                    {
                        startData.flowDirection = GameEnums::FlowDirections_East;
                        startData.flowDown = false;
                    }
                    else
                    {
                        startData.flowDirection = GameEnums::FlowDirections_West;
                        startData.flowDown = false;
                    }
                }
                else if (pos.y() < m_StartPoint.y())
                {
                    startData.flowDirection = GameEnums::FlowDirections_South;
                    startData.flowDown = false;
                }
            }
            flowList.append(startData);
        }
    }
    if (flowList.length() == 0)
    {
        m_data.addData(m_StartPoint, 0, GameEnums::FlowDirections_None);
    }
}

GameEnums::FlowDirections TerrainFindingSystem::getDirection(QPoint cur, QPoint next) const
{
    if (cur.x() - next.x() > 0)
    {
        return GameEnums::FlowDirections_West;
    }
    else if (cur.x() - next.x() < 0)
    {
        return GameEnums::FlowDirections_East;
    }
    else if (cur.y() - next.y() > 0)
    {
        return GameEnums::FlowDirections_North;
    }
    else if (cur.y() - next.y() < 0)
    {
        return GameEnums::FlowDirections_South;
    }
    return GameEnums::FlowDirections_None;
}

GameEnums::FlowDirections TerrainFindingSystem::getDirection(QPoint cur, QPoint next, GameEnums::FlowDirections previous, bool flowDown) const
{
    if (cur.x() != next.x() &&
        (previous == GameEnums::FlowDirections_East ||
         previous == GameEnums::FlowDirections_West))
    {
        return previous;
    }
    else if (cur.y() != next.y() &&
             (previous == GameEnums::FlowDirections_South ||
              previous == GameEnums::FlowDirections_North))
    {
        return previous;
    }
    else
    {
        if (flowDown)
        {
            return getDirection(cur, next);
        }
        else
        {
            return getDirection(next, cur);
        }
    }
    return GameEnums::FlowDirections_None;
}
