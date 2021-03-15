#ifndef INFLUENCEFRONTMAP_H
#define INFLUENCEFRONTMAP_H

#include <QObject>
#include <QMap>
#include <QVector>
#include "ai/islandmap.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;
class Unit;
class UnitPathFindingSystem;
class GameMap;
class QmlVectorPoint;

class InfluenceFrontMap : public QObject
{
    Q_OBJECT
public:
    struct InfluenceInfo
    {
        InfluenceInfo(GameMap* pMap);
        void reset();
        void updateOwner();
        QVector<qint32> playerValues;
        qint32 highestInfluence{0};
        QVector<qint32> owners;
        QStringList frontMovetype;
        QVector<qint32> frontOwners;
        bool frontLineCreated{false};
    };
    explicit InfluenceFrontMap(const QVector<spIslandMap> & islands);
    virtual ~InfluenceFrontMap() = default;
    void addBuildingInfluence();
    void addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints);
    void updateOwners();
    void findFrontLines();
    /**
     * @brief show for debugging purpose
     */
    void show();
    /**
     * @brief showFrontline
     */
    void showFrontlines();
    /**
     * @brief show for debugging purpose
     */
    void hide();

    void reset();

    const InfluenceInfo & getInfluenceInfo(qint32 x, qint32 y) const
    {
        return m_InfluenceMap[x][y];
    }
private:
    /**
     * @brief getIslandFromUnitId
     * @param unitId
     * @param unitIdToIsland
     */
    qint32 getIslandFromUnitId(QString unitId, QMap<QString, qint32> & unitIdToIsland);
    /**
     * @brief findFrontLineTiles
     */
    void findFrontLineTiles();
    /**
     * @brief addFrontLineMoveTypes
     */
    void addFrontLineMoveTypes(InfluenceInfo & info, qint32 x1, qint32 y1, qint32 x2, qint32 y2);
    /**
     * @brief createFrontLine
     */
    void createFrontLine();
    /**
     * @brief searchFrontLine
     * @param neighbours
     * @param info
     * @param x
     * @param y
     */
    void searchFrontLine(QmlVectorPoint* neighbours, InfluenceInfo & info, qint32 x, qint32 y, QVector<QPoint> & frontline);
private:

    QVector<QVector<InfluenceInfo>> m_InfluenceMap;
    const QVector<spIslandMap> & m_islands;
    QVector<QVector<QPoint>> m_frontLines;
    Player* m_pOwner;
    QVector<oxygine::spActor> m_info;

};

#endif // INFLUENCEFRONTMAP_H
