#pragma once

#include <QString>
#include <map>

class AwbwDataTypes
{
public:
    static const qint32 TERRAIN_OWNER = -2;
    static const qint32 NEUTRAL_OWNER = -1;
    struct TileInfo
    {
        QString id;
        qint32 owner;
    };

    static std::map<QString, qint32> FACTION_MAP;
    static std::map<qint32, QString> UNITID_MAP;
    static std::map<qint32, TileInfo> TERRAIN_BUILDING_DATA;
    static std::map<QString, QString> UNIT_ID_ID_MAP;
};
