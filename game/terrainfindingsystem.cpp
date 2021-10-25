#include "game/terrainfindingsystem.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"

#include "coreengine/globalutils.h"

TerrainFindingSystem::TerrainFindingSystem(QString terrainID, qint32 startX, qint32 startY)
    : PathFindingSystem(startX, startY,
                        GameMap::getInstance()->getMapWidth(),
                        GameMap::getInstance()->getMapHeight()),
      m_terrainID(terrainID)
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
            if (pMap->getTerrain(x, y)->getID() == m_terrainID)
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
    spQmlVectorPoint circle(GlobalUtils::getCircle(1, 1));
    const qint32 size = circle->size();
    QVector<PositionFlowData> flowList;
    // determine start search pattern
    PositionFlowData startData;
    startData.current = m_StartPoint;
    for (qint32 i = size - 1; i >= 0; --i)
    {
        qint32 index = qAbs(i - 1) % size;
        QPoint pos = m_StartPoint + circle->at(index);
        if (getTargetCosts(pos.x(), pos.y()) > 0)
        {
            startData.next = pos;
            if (flowList.size() == 0)
            {
                m_data.addData(m_StartPoint, 0, getDirection(startData.current, startData.next));
            }
            flowList.append(startData);
        }
    }
    while (flowList.size() > 0)
    {
        auto & current = flowList.front();



        flowList.pop_front();
    }
    return &m_data;
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
