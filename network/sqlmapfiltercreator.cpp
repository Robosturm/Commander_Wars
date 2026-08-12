#include "network/sqlmapfiltercreator.h"
#include "network/mainserver.h"

void SqlMapFilterCreator::addFlagFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, const QVector<MapFilter::FlagFilter> & filters)
{
    bool requiresOptional = true;
    for (const auto & filter : filters)
    {
        if (filter.isActive && !filter.isOptional)
        {
            requiresOptional = false;
            if (filterCount > 0)
            {
                filterCommand += " AND ";
            }
            filterCommand += QString(MainServer::SQL_MAPFLAGS) + " & ? > 0";
            filterValues.append(filter.flag);
            ++filterCount;
        }
    }
    bool initialAnd = false;
    if (requiresOptional)
    {
        for (const auto & filter : filters)
        {
            if (filter.isActive && filter.isOptional)
            {
                if (filterCount > 0 && !initialAnd)
                {
                    filterCommand += " AND ( ";
                    initialAnd = true;
                }
                else if (!initialAnd)
                {
                    filterCommand += " ( ";
                    initialAnd = true;
                }
                else
                {
                    filterCommand += " OR ";
                }
                filterCommand += QString(MainServer::SQL_MAPFLAGS) + " & ? > 0";
                filterValues.append(filter.flag);
                ++filterCount;
            }
        }
    }
    if (initialAnd)
    {
        filterCommand += ")";
    }
}

void SqlMapFilterCreator::addFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, qint32 value, const char* const item, const char* const opCommand)
{
    if (value > 0)
    {
        if (filterCount > 0)
        {
            filterCommand += " AND ";
        }
        filterCommand += QString(item) + " " + opCommand + " ?";
        filterValues.append(value);
        ++filterCount;
    }
}

void SqlMapFilterCreator::addFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, const QString &value, const char* const item)
{
    if (!value.isEmpty())
    {
        if (filterCount > 0)
        {
            filterCommand += " AND ";
        }
        filterCommand += QString(item) + " LIKE ?";
        filterValues.append(value + "%");
        ++filterCount;
    }
}
