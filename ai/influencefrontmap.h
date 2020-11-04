#ifndef INFLUENCEFRONTMAP_H
#define INFLUENCEFRONTMAP_H

#include <QObject>
#include <QMap>

class Player;
class Unit;
class UnitPathFindingSystem;
class GameMap;

class InfluenceFrontMap : public QObject
{
    Q_OBJECT
public:
    explicit InfluenceFrontMap();
    virtual ~InfluenceFrontMap() = default;

    void addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs);
private:
    struct InfluenceInfo
    {
        InfluenceInfo(GameMap* pMap);
        QVector<qint32> playerValues;
        qint32 frontField{-1};
    };
    QVector<QVector<InfluenceInfo>> m_InfluenceMap;
    Player* m_pOwner;
    QMap<QString, qint32> m_costInfo;
};

#endif // INFLUENCEFRONTMAP_H
