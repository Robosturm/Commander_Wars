#ifndef INFLUENCEFRONTMAP_H
#define INFLUENCEFRONTMAP_H

#include <QObject>
#include <QMap>

class Player;
class Unit;
class UnitPathFindingSystem;
class GameMap;

#include "oxygine-framework.h"

class InfluenceFrontMap : public QObject
{
    Q_OBJECT
public:
    explicit InfluenceFrontMap();
    virtual ~InfluenceFrontMap() = default;

    void addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints);

    /**
     * @brief show for debugging purpose
     */
    void show();
    /**
     * @brief show for debugging purpose
     */
    void hide();

    void reset();
private:
    struct InfluenceInfo
    {
        InfluenceInfo(GameMap* pMap);
        QVector<qint32> playerValues;
        qint32 frontField{-1};
        qint32 getOwner();
    };
    QVector<QVector<InfluenceInfo>> m_InfluenceMap;
    Player* m_pOwner;
    QMap<QString, qint32> m_costInfo;

    QVector<oxygine::spColorRectSprite> m_info;
};

#endif // INFLUENCEFRONTMAP_H
