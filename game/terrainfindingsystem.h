#ifndef TERRAINFINDINGSYSTEM_H
#define TERRAINFINDINGSYSTEM_H

#include "coreengine/pathfindingsystem.h"

class TerrainFindingSystem : public PathFindingSystem
{
    Q_OBJECT
public:
    TerrainFindingSystem(QString terrainID, qint32 startX, qint32 startY);

    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y, qint32 currentCost);
    /**
     * @brief finished checks if this would be the target node to reach
     * @param x
     * @param y
     * @return
     */
    virtual bool finished(qint32 x, qint32 y);
    /**
     * @brief getCosts
     * @param x
     * @param y
     * @return the exact costs needed to get onto the given field. -1 = unreachable
     */
    virtual qint32 getCosts(qint32 x, qint32 y);
public slots:
    /**
     * @brief killTerrainFindingSystem
     */
    void killTerrainFindingSystem();
private:
    QString m_terrainID;
};

#endif // TERRAINFINDINGSYSTEM_H
