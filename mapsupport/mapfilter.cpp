#include "mapsupport/mapfilter.h"
#include "network/JsonKeys.h"

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
    matches = matches && matchesMapFlags(m_filter, info.m_mapFlags);
    return matches;
}

QJsonObject MapFilter::toJson() const
{
    QJsonObject obj;
    obj.insert(JsonKeys::JSONKEY_MAPNAME, m_mapName);
    obj.insert(JsonKeys::JSONKEY_MAPAUTHOR, m_mapAuthor);
    obj.insert(JsonKeys::JSONKEY_MINPLAYERS, m_players.minValue);
    obj.insert(JsonKeys::JSONKEY_MAXPLAYERS, m_players.maxValue);
    obj.insert(JsonKeys::JSONKEY_MINWIDTH, m_width.minValue);
    obj.insert(JsonKeys::JSONKEY_MAXWIDTH, m_width.maxValue);
    obj.insert(JsonKeys::JSONKEY_MINHEIGHT, m_height.minValue);
    obj.insert(JsonKeys::JSONKEY_MAXHEIGHT, m_height.maxValue);
    QJsonArray filters;
    for (const auto & filter : m_filter)
    {
        QJsonObject filterObj;
        filterObj.insert(JsonKeys::JSONKEY_FLAGFILTERFLAGS, filter.flag);
        filterObj.insert(JsonKeys::JSONKEY_FLAGFILTERISACTIVE, filter.isActive);
        filterObj.insert(JsonKeys::JSONKEY_FLAGFILTERISOPTIONAL, filter.isOptional);
        filters.append(filterObj);
    }
    obj.insert(JsonKeys::JSONKEY_FLAGFILTER, filters);
    return obj;
}

void MapFilter::fromJson(const QJsonObject & object)
{
    m_mapName = object.value(JsonKeys::JSONKEY_MAPNAME).toString();
    m_mapAuthor = object.value(JsonKeys::JSONKEY_MAPAUTHOR).toString();
    m_players.minValue = object.value(JsonKeys::JSONKEY_MINPLAYERS).toInt();
    m_players.maxValue = object.value(JsonKeys::JSONKEY_MAXPLAYERS).toInt();
    m_width.minValue = object.value(JsonKeys::JSONKEY_MINWIDTH).toInt();
    m_width.maxValue = object.value(JsonKeys::JSONKEY_MAXWIDTH).toInt();
    m_height.minValue = object.value(JsonKeys::JSONKEY_MINHEIGHT).toInt();
    m_height.maxValue = object.value(JsonKeys::JSONKEY_MAXHEIGHT).toInt();
    QJsonArray filters = object.value(JsonKeys::JSONKEY_FLAGFILTER).toArray();
    m_filter.clear();
    for (const auto & filterObj : filters)
    {
        auto filterInfo = filterObj.toObject();
        FlagFilter filter;
        filter.flag = static_cast<GameEnums::MapFilterFlags>(filterInfo.value(JsonKeys::JSONKEY_FLAGFILTERFLAGS).toInteger());
        filter.isActive = filterInfo.value(JsonKeys::JSONKEY_FLAGFILTERISACTIVE).toBool();
        filter.isOptional = filterInfo.value(JsonKeys::JSONKEY_FLAGFILTERISOPTIONAL).toBool();
        m_filter.append(filter);
    }
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
    return (value >= minValue || minValue == 0) &&
           (value <= maxValue || maxValue == 0);
}

bool MapFilter::FlagFilter::matches(GameEnums::MapFilterFlags flags) const
{
    return (flags & flag) > 0;
}

bool MapFilter::matchesMapFlags(const QVector<FlagFilter> & filters, GameEnums::MapFilterFlags flags)
{
    bool matches = true;
    bool hasOptional = false;
    qint32 count = 0;
    for (auto & filter : filters)
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
