#pragma once
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
        void updateOwner(Player* pOwner);
        qint32 getPlayerInfluence(qint32 playerId);
        void increaseInfluence(qint32 player, qint32 value);
        QVector<qint32> owners;
        QStringList frontMovetype;
        QVector<qint32> frontOwners;
        bool frontLineCreated{false};
        qint32 highestInfluence{0};
        qint32 ownInfluence{0};
        qint32 highestEnemyInfluence{0};        
    private:
        QVector<qint32> playerValues;
        GameMap* m_pMap{nullptr};
    };
    explicit InfluenceFrontMap(GameMap* pMap, const QVector<spIslandMap> & islands);
    virtual ~InfluenceFrontMap() = default;
    void addBuildingInfluence();
    void addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints);
    void updateOwners();
    void calculateGlobalData();
    /**
     * @brief show for debugging purpose
     */
    void show();
    /**
     * @brief showFrontline
     */
    void showFrontlines();
    /**
     * @brief showPfs
     * @param pPfs
     */
    void showPfs(UnitPathFindingSystem* pPfs);
    /**
     * @brief hide for debugging purpose
     */
    void hide();

    void reset();
    /**
     * @brief getInfluenceInfo
     * @param x
     * @param y
     * @return
     */
    const InfluenceInfo & getInfluenceInfo(qint32 x, qint32 y) const
    {
        return m_InfluenceMap[x][y];
    }
    /**
     * @brief getTotalHighestInfluence
     * @return
     */
    qint32 getTotalHighestInfluence() const;

    Player *getOwner() const;
    void setOwner(Player *newPOwner);

private:
    /**
     * @brief getIslandFromUnitId
     * @param unitId
     * @param unitIdToIsland
     */
    qint32 getIslandFromUnitId(const QString & unitId, QMap<QString, qint32> & unitIdToIsland);
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
    /**
     * @brief updateHighestInfluence
     */
    void updateHighestInfluence();
private:

    QVector<QVector<InfluenceInfo>> m_InfluenceMap;
    const QVector<spIslandMap> & m_islands;
    QVector<QVector<QPoint>> m_frontLines;
    Player* m_pOwner;
    QVector<oxygine::spActor> m_info;
    qint32 m_totalHighestInfluence{0};
    GameMap* m_pMap{nullptr};
};
