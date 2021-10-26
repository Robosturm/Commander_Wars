#ifndef TERRAINFINDINGSYSTEM_H
#define TERRAINFINDINGSYSTEM_H

#include "coreengine/pathfindingsystem.h"

#include "game/jsData/terrainflowdata.h"

class TerrainFindingSystem;
using spTerrainFindingSystem = oxygine::intrusive_ptr<TerrainFindingSystem>;

class TerrainFindingSystem : public PathFindingSystem
{
    Q_OBJECT
    struct PositionFlowData
    {
        QPoint current;
        QPoint next;
        qint32 cost{0};
    };
public:
    explicit TerrainFindingSystem(QString terrainID, qint32 startX, qint32 startY);
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
    virtual qint32 getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY ) override;
public slots:
    /**
     * @brief killTerrainFindingSystem
     */
    void killTerrainFindingSystem();
    /**
     * @brief getFlowData
     * @return
     */
    TerrainFlowData* getFlowData();
    /**
     * @brief getDirection
     * @param cur
     * @param next
     * @return
     */
    GameEnums::FlowDirections getDirection(QPoint cur, QPoint next) const;

private:
    QString m_terrainID;
    TerrainFlowData m_data;
};

#endif // TERRAINFINDINGSYSTEM_H
