#pragma once

#include "mapsupport/mapfilter.h"
#include <QList>
#include <QVariant>

class SqlMapFilterCreator
{
public:
    SqlMapFilterCreator() = default;
protected:
    void addFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, qint32 value, const char* const item, const char* const opCommand);
    void addFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, const QString &value, const char* const item);
    void addFlagFilterOption(QString & filterCommand, QList<QVariant> & filterValues, qint32 & filterCount, const QVector<MapFilter::FlagFilter> & filters);
};

