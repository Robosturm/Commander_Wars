#ifndef TERRAINFINDINGSYSTEM_H
#define TERRAINFINDINGSYSTEM_H

#include "coreengine/pathfindingsystem.h"

#include "game/jsData/terrainflowdata.h"

class GameMap;
class TerrainFindingSystem;
using spTerrainFindingSystem = oxygine::intrusive_ptr<TerrainFindingSystem>;

class TerrainFindingSystem final : public PathFindingSystem
{
    Q_OBJECT
    struct PositionFlowData
    {
        QPoint current;
        QPoint next;
        qint32 cost{0};
        qint32 flowListSource{-1};
        GameEnums::FlowDirections flowDirection;
        bool flowDown{true};
    };
public:
    explicit TerrainFindingSystem(GameMap* pMap, QString terrainID, qint32 startX, qint32 startY);
    explicit TerrainFindingSystem(GameMap* pMap, QStringList terrainIDs, qint32 startX, qint32 startY);
   virtual ~TerrainFindingSystem() = default;
    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y, qint32 currentCost) override;
    /**
     * @brief finished checks if this would be the target node to reach
     * @param x
     * @param y
     * @return
     */
    virtual bool finished(qint32 x, qint32 y, qint32 costs) override;
    /**
     * @brief getCosts
     * @param x
     * @param y
     * @return the exact costs needed to get onto the given field. -1 = unreachable
     */
    virtual qint32 getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY, qint32 currentCost) override;
    /**
     * @brief killTerrainFindingSystem
     */
    Q_INVOKABLE void killTerrainFindingSystem();
    /**
     * @brief getFlowData
     * @return
     */
    Q_INVOKABLE TerrainFlowData* getFlowData();
    /**
     * @brief getDirection
     * @param cur
     * @param next
     * @return
     */
    Q_INVOKABLE GameEnums::FlowDirections getDirection(QPoint cur, QPoint next) const;
    /**
     * @brief TerrainFindingSystem::getDirection
     * @param cur
     * @param next
     * @param previous
     * @return
     */
    Q_INVOKABLE GameEnums::FlowDirections getDirection(QPoint cur, QPoint next, GameEnums::FlowDirections previous, bool flowDown) const;
private:
    void addStartFlows(const spQmlVectorPoint & circle, const qint32 size,
                       QVector<PositionFlowData> & flowList);
private:
    QStringList m_terrainIDs;
    TerrainFlowData m_data;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(TerrainFindingSystem, "TerrainFindingSystem");

#endif // TERRAINFINDINGSYSTEM_H
