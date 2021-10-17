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

void MapFilter::addToFilter(GameEnums::MapFilterFlags flag, bool active, bool isOptional)
{
    bool found = false;
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        FlagFilter filter;
        filter.flag = flag;
        filter.isActive = active;
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
            m_filter[i].isActive = false;
            break;
        }
    }
}

void MapFilter::setFlagActive(GameEnums::MapFilterFlags flag, bool isActive)
{
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            filter.isActive = isActive;
            break;
        }
    }
}

bool MapFilter::getFlagActive(GameEnums::MapFilterFlags flag)
{
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            return filter.isActive;
        }
    }
    FlagFilter filter;
    filter.flag = flag;
    filter.isActive = false;
    filter.isOptional = false;
    m_filter.append(filter);
    return false;
}

bool MapFilter::getFlagOptional(GameEnums::MapFilterFlags flag)
{
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            return filter.isOptional;
        }
    }
    FlagFilter filter;
    filter.flag = flag;
    filter.isActive = false;
    filter.isOptional = false;
    m_filter.append(filter);
    return false;
}

void MapFilter::setFlagOptional(GameEnums::MapFilterFlags flag, bool isOptional)
{
    for (auto & filter : m_filter)
    {
        if (filter.flag == flag)
        {
            filter.isOptional = isOptional;
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
        if (filter.isActive)
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
    }
    return matches && (!hasOptional || count > 0);
}

QString MapFilter::getMapName() const
{
    return m_mapName;
}

void MapFilter::setMapName(const QString &newMapName)
{
    m_mapName = newMapName;
}

QString MapFilter::getMapAuthor() const
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
