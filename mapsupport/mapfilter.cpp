#include "mapsupport/mapfilter.h"

bool MapFilter::matches(GameMap::MapHeaderInfo & info) const
{
    bool matches = true;
    if (!m_mapName.isEmpty())
    {
        matches = info.m_mapName.contains(m_mapName);
    }
    if (matches && !m_mapAuthor.isEmpty())
    {
        matches = info.m_mapAuthor.contains(m_mapAuthor);
    }
    matches = matches && m_players.matches(info.m_playerCount);
    matches = matches && m_width.matches(info.m_width);
    matches = matches && m_height.matches(info.m_heigth);
    matches = matches && matchesMapFlags(info.m_mapFlags);
    return matches;
}

void MapFilter::addToFilter(GameEnums::MapFilterFlags flag, bool isOptional)
{
    bool found = false;
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            filter.isOptional = isOptional;
            found = true;
            break;
        }
    }
    if (!found)
    {
        FlagFilter filter;
        filter.flag = flag;
        filter.isOptional = isOptional;
        m_filter.append(filter);
    }
}

void MapFilter::removeFromFilter(GameEnums::MapFilterFlags flag)
{
    for (qint32 i = 0; i < m_filter.size(); ++i)
    {
        if (m_filter[i].flag == flag)
        {
            m_filter.removeAt(i);
            break;
        }
    }
}

bool MapFilter::MinMaxFilter::matches(qint32 value) const
{
    return (value > minValue || minValue == 0) &&
           (value < maxValue || maxValue == 0);
}

bool MapFilter::FlagFilter::matches(GameEnums::MapFilterFlags flags) const
{
    return (flags & flag) > 0;
}

bool MapFilter::matchesMapFlags(GameEnums::MapFilterFlags flags) const
{
    bool matches = true;
    bool hasOptional = false;
    qint32 count = 0;
    for (auto & filter : m_filter)
    {
        bool flagMatches = filter.matches(flags);
        if (filter.isOptional)
        {
            hasOptional = true;
        }
        if (flagMatches)
        {
            ++count;
        }
        else if (!filter.isOptional)
        {
            matches = false;
            break;
        }
    }
    return matches && (!hasOptional || count > 0);
}

const QString &MapFilter::getMapName() const
{
    return m_mapName;
}

void MapFilter::setMapName(const QString &newMapName)
{
    m_mapName = newMapName;
}

const QString &MapFilter::getMapAuthor() const
{
    return m_mapAuthor;
}

void MapFilter::setMapAuthor(const QString &newMapAuthor)
{
    m_mapAuthor = newMapAuthor;
}

void MapFilter::setMinHeight(qint32 value)
{
    m_height.minValue = value;
}

qint32 MapFilter::getMinHeight() const
{
    return m_height.minValue;
}

void MapFilter::setMaxHeight(qint32 value)
{
    m_height.maxValue = value;
}

qint32 MapFilter::getMaxHeight() const
{
    return m_height.maxValue;
}

void MapFilter::setMinWidth(qint32 value)
{
    m_width.minValue = value;
}

qint32 MapFilter::getMinWidth() const
{
    return m_width.minValue;
}

void MapFilter::setMaxWidth(qint32 value)
{
    m_width.maxValue = value;
}

qint32 MapFilter::getMaxWidth() const
{
    return m_width.maxValue;
}

void MapFilter::setMinPlayer(qint32 value)
{
    m_players.minValue = value;
}

qint32 MapFilter::getMinPlayer() const
{
    return m_players.minValue;
}

void MapFilter::setMaxPlayer(qint32 value)
{
    m_players.maxValue = value;
}

qint32 MapFilter::getMaxPlayer() const
{
    return m_players.maxValue;
}
