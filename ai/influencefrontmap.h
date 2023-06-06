#pragma once
#include <QObject>
#include <QMap>
#include <QVector>
#include "ai/islandmap.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

class Player;
class Unit;
class UnitPathFindingSystem;
class GameMap;
class QmlVectorPoint;
class InfluenceFrontMap;

class InfluenceInfo final : public QObject
{
    Q_OBJECT
public:
    InfluenceInfo();
    explicit InfluenceInfo(GameMap* pMap);
    ~InfluenceInfo() = default;

    void reset(GameMap* pMap);
    void updateOwner(Player* pOwner);
    qint32 getPlayerInfluence(qint32 playerId);
    void increaseInfluence(qint32 player, qint32 value);

    const std::vector<qint32> &getFrontOwners() const;
    Q_INVOKABLE QStringList getFrontMovetype() const;
    Q_INVOKABLE qint32 getHighestInfluence() const;
    Q_INVOKABLE qint32 getOwnInfluence() const;
    Q_INVOKABLE qint32 getEnemyInfluence() const;

private:
    friend class InfluenceFrontMap;
    std::vector<qint32> owners;
    QStringList frontMovetype;
    std::vector<qint32> frontOwners;
    bool frontLineCreated{false};
    qint32 highestInfluence{0};
    qint32 ownInfluence{0};
    qint32 enemyInfluence{0};
    std::vector<qint32> playerValues;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(InfluenceInfo, "InfluenceInfo");

class InfluenceFrontMap final : public QObject
{
    Q_OBJECT
public:
    explicit InfluenceFrontMap(GameMap* pMap, const std::vector<spIslandMap> & islands);
    ~InfluenceFrontMap() = default;
    void addBuildingInfluence();
    void addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints);
    void updateOwners();
    void calculateGlobalData();
    void setOwner(Player *newPOwner);
    void reset();
    void clear();
    /**
     * @brief show for debugging purpose
     */
    Q_INVOKABLE void show();
    /**
     * @brief showFrontline
     */
    Q_INVOKABLE void showFrontlines();
    /**
     * @brief showPfs
     * @param pPfs
     */
    Q_INVOKABLE void showPfs(UnitPathFindingSystem* pPfs);
    /**
     * @brief hide for debugging purpose
     */
    Q_INVOKABLE void hide();
    /**
     * @brief getInfluenceInfo
     * @param x
     * @param y
     * @return
     */
    Q_INVOKABLE const InfluenceInfo * getInfluenceInfo(qint32 x, qint32 y) const
    {
        return &(m_InfluenceMap[x][y]);
    }
    /**
     * @brief getTotalHighestInfluence
     * @return
     */
    Q_INVOKABLE qint32 getTotalHighestInfluence() const;
    Q_INVOKABLE Player *getOwner() const;

private:
    /**
     * @brief getIslandFromUnitId
     * @param unitId
     * @param unitIdToIsland
     */
    qint32 getIslandFromUnitId(const QString & unitId, std::map<QString, qint32> & unitIdToIsland);
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
    void searchFrontLine(QmlVectorPoint* neighbours, InfluenceInfo & info, qint32 x, qint32 y, std::vector<QPoint> & frontline);
    /**
     * @brief updateHighestInfluence
     */
    void updateHighestInfluence();
private:

    std::vector<std::vector<InfluenceInfo>> m_InfluenceMap;
    const std::vector<spIslandMap> & m_islands;
    std::vector<std::vector<QPoint>> m_frontLines;
    Player* m_pOwner;
    std::vector<oxygine::spActor> m_info;
    qint32 m_totalHighestInfluence{0};
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(InfluenceFrontMap, "InfluenceFrontMap");
