#include "game/jsData/terrainflowdata.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/console.h"
#include "coreengine/qmlvector.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"

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

bool TerrainFlowData::addData(QPoint newPosition, qint32 newCosts, GameEnums::FlowDirections newFlowDirection)
{
    qint32 index = m_positions.indexOf(newPosition);
    if (index >= 0)
    {
        m_flowDirections[index] = static_cast<GameEnums::FlowDirections>(m_flowDirections[index] | newFlowDirection);
    }
    else
    {
        m_positions.append(newPosition);
        m_costs.append(newCosts);
        m_flowDirections.append(newFlowDirection);
    }
    return index >= 0;
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
        ret = getFlowDirectionString(m_flowDirections.at(index));
    }
    return ret;
}

QString TerrainFlowData::getFlowDirectionString(GameEnums::FlowDirections flowDirection) const
{
    QString ret;
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
    return ret;
}

QStringList TerrainFlowData::getAlternateFlowString(GameEnums::FlowDirections flowDirection) const
{
    QStringList ret;
    qint32 count = 0;
    if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_North) == GameEnums::FlowDirections::FlowDirections_North)
    {
        ++count;
    }
    if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_South) == GameEnums::FlowDirections::FlowDirections_South)
    {
        ++count;
    }
    if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_East) == GameEnums::FlowDirections::FlowDirections_East)
    {
        ++count;
    }
    if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_West) == GameEnums::FlowDirections::FlowDirections_West)
    {
        ++count;
    }
    if (count > 1)
    {
        if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_North) == GameEnums::FlowDirections::FlowDirections_North)
        {
            auto direction = static_cast<GameEnums::FlowDirections>(flowDirection & ~GameEnums::FlowDirections_North);
            ret.append(getFlowDirectionString(direction));
            auto list = getAlternateFlowString(direction);
            if (list.size() > 0)
            {
                ret.append(list);
            }
        }
        if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_South) == GameEnums::FlowDirections::FlowDirections_South)
        {
            auto direction = static_cast<GameEnums::FlowDirections>(flowDirection & ~GameEnums::FlowDirections_South);
            ret.append(getFlowDirectionString(direction));
            auto list = getAlternateFlowString(direction);
            if (list.size() > 0)
            {
                ret.append(list);
            }
        }
        if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_West) == GameEnums::FlowDirections::FlowDirections_West)
        {
            auto direction = static_cast<GameEnums::FlowDirections>(flowDirection & ~GameEnums::FlowDirections_West);
            ret.append(getFlowDirectionString(direction));
            auto list = getAlternateFlowString(direction);
            if (list.size() > 0)
            {
                ret.append(list);
            }
        }
        if (static_cast<GameEnums::FlowDirections>(flowDirection & GameEnums::FlowDirections_East) == GameEnums::FlowDirections::FlowDirections_East)
        {
            auto direction = static_cast<GameEnums::FlowDirections>(flowDirection & ~GameEnums::FlowDirections_East);
            ret.append(getFlowDirectionString(direction));
            auto list = getAlternateFlowString(direction);
            if (list.size() > 0)
            {
                ret.append(list);
            }
        }
    }
    ret.removeDuplicates();
    return ret;
}

void TerrainFlowData::print()
{
    CONSOLE_PRINT("River length = " + QString::number(m_positions.size()), Console::eDEBUG);

    for (qint32 i = 0; i < m_flowDirections.size(); ++i)
    {
        CONSOLE_PRINT("X=" + QString::number(m_positions[i].x()) +
                      " Y=" + QString::number(m_positions[i].y()) +
                      " Direction=" + getFlowString(i), Console::eDEBUG);
    }
}

QVector<QPoint> TerrainFlowData::getOverlayTiles(QStringList terrains)
{
    QVector<QPoint> ret;
    spQmlVectorPoint circle(GlobalUtils::getCircle(1, 1));
    // circle->append(QPoint(1, 1));
    // circle->append(QPoint(-1, 1));
    // circle->append(QPoint(-1, -1));
    // circle->append(QPoint(1, -1));
    spGameMap pMap = GameMap::getInstance();
    const qint32 size = circle->size();
    for (qint32 i2 = 0; i2 < m_positions.size(); ++i2)
    {
        for (qint32 i = 0; i < size; ++i)
        {
            QPoint newPos = m_positions[i2] + circle->at(i);
            if (pMap->onMap(newPos.x(), newPos.y()) &&
                !ret.contains(newPos) &&
                terrains.contains(pMap->getTerrain(newPos.x(), newPos.y())->getID()))
            {
                ret.append(newPos);
                m_overlayTileMapping.append(i2);
            }
        }
    }
    return ret;
}

QVector<qint32> TerrainFlowData::getOverlayTileMapping() const
{
    return m_overlayTileMapping;
}
