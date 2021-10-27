#include "game/jsData/terrainflowdata.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/console.h"

TerrainFlowData::TerrainFlowData()
{
    setObjectName("TerrainFlowData");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void TerrainFlowData::clear()
{
    m_positions.clear();
    m_costs.clear();
    m_flowDirections.clear();
}

void TerrainFlowData::addData(QPoint newPosition, qint32 newCosts, GameEnums::FlowDirections newFlowDirection)
{
    m_positions.append(newPosition);
    m_costs.append(newCosts);
    m_flowDirections.append(newFlowDirection);
}

void TerrainFlowData::addFlowDirection(qint32 index, GameEnums::FlowDirections newFlowDirection)
{
    if (index >= 0 && index < m_flowDirections.size())
    {
        m_flowDirections[index] = static_cast<GameEnums::FlowDirections>(m_flowDirections[index] | newFlowDirection);
    }
}

QPoint TerrainFlowData::getPosition(qint32 index) const
{
    if (index >= 0 && index < m_flowDirections.size())
    {
        return m_positions[index];
    }
    return QPoint(-1, -1);
}

qint32 TerrainFlowData::getCosts(qint32 index) const
{
    if (index >= 0 && index < m_flowDirections.size())
    {
        return m_costs[index];
    }
    return -1;
}

const GameEnums::FlowDirections TerrainFlowData::getFlowDirection(qint32 index) const
{
    if (index >= 0 && index < m_flowDirections.size())
    {
        return m_flowDirections[index];
    }
    return GameEnums::FlowDirections_None;
}

QString TerrainFlowData::getFlowString(qint32 index) const
{
    QString ret;
    if (index >= 0 && index < m_flowDirections.size())
    {
        auto flowDirection = m_flowDirections.at(index);
        if (flowDirection & GameEnums::FlowDirections_North)
        {
            ret += "+N";
        }
        if (flowDirection & GameEnums::FlowDirections_East)
        {
            ret += "+E";
        }
        if (flowDirection & GameEnums::FlowDirections_South)
        {
            ret += "+S";
        }
        if (flowDirection & GameEnums::FlowDirections_West)
        {
            ret += "+W";
        }
    }
    return ret;
}

void TerrainFlowData::mergeFlows()
{
    for (qint32 i = 0; i < m_flowDirections.size(); ++i)
    {
        QPoint & pos = m_positions[i];
        for (qint32 i2 = 0; i2 < m_flowDirections.size(); ++i2)
        {
            QPoint & next = m_positions[i2];
            if ((next.y() == pos.y() && qAbs(pos.x() - next.x()) == 1) ||
                (next.x() == pos.x() && qAbs(pos.y() - next.y()) == 1))
            {
                // addFlowDirection(i
            }
        }
    }
}

void TerrainFlowData::print()
{
    for (qint32 i = 0; i < m_flowDirections.size(); ++i)
    {
        CONSOLE_PRINT("X=" + QString::number(m_positions[i].x()) +
                      " Y=" + QString::number(m_positions[i].y()) +
                      " Direction=" + getFlowString(i), Console::eDEBUG);
    }
}
