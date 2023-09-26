#pragma once

#include "game/GameEnums.h"
#include "game/gamemap.h"

class MapFilter final
{
    struct FlagFilter
    {
        GameEnums::MapFilterFlags flag;
        bool isActive{false};
        bool isOptional{false};
        bool matches(GameEnums::MapFilterFlags flags) const;
    };
    struct MinMaxFilter
    {
        qint32 minValue{0};
        qint32 maxValue{0};
        bool matches(qint32 value) const;
    };

public:
    explicit MapFilter() = default;
    virtual ~MapFilter() = default;

    void setFlagActive(GameEnums::MapFilterFlags flag, bool isOptional);
    bool getFlagActive(GameEnums::MapFilterFlags flag);
    void setFlagOptional(GameEnums::MapFilterFlags flag, bool isOptional);
    bool getFlagOptional(GameEnums::MapFilterFlags flag);
    void addToFilter(GameEnums::MapFilterFlags flag, bool active, bool isOptional);
    void removeFromFilter(GameEnums::MapFilterFlags flag);

    bool matches(GameMap::MapHeaderInfo & info) const;

    void setMinHeight(qint32 value);
    qint32 getMinHeight() const;
    void setMaxHeight(qint32 value);
    qint32 getMaxHeight() const;

    void setMinWidth(qint32 value);
    qint32 getMinWidth() const;
    void setMaxWidth(qint32 value);
    qint32 getMaxWidth() const;

    void setMinPlayer(qint32 value);
    qint32 getMinPlayer() const;
    void setMaxPlayer(qint32 value);
    qint32 getMaxPlayer() const;

    QString getMapAuthor() const;
    void setMapAuthor(const QString &newMapAuthor);

    QString getMapName() const;
    void setMapName(const QString &newMapName);

private:
    bool matchesMapFlags(GameEnums::MapFilterFlags flags) const;
private:

    QVector<FlagFilter> m_filter;
    QString m_mapName;
    QString m_mapAuthor;
    MinMaxFilter m_players;
    MinMaxFilter m_width;
    MinMaxFilter m_height;
};

