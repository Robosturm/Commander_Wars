#pragma once

#include "mapsupport/mapfilter.h"

class SqlMapFilterCreator
{
public:
    SqlMapFilterCreator() = default;
protected:
    void addFilterOption(QString & filterCommand, qint32 value, qint32 & filterCount, const char* const item, const char* const opCommand);
    void addFilterOption(QString & filterCommand, QString value, qint32 & filterCount, const char* const item);
    void addFlagFilterOption(QString & filterCommand, qint32 & filterCount, const QVector<MapFilter::FlagFilter> & filters);
};

