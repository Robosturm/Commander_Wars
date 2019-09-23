#ifndef UNITPATHFINDINGSYSTEM_H
#define UNITPATHFINDINGSYSTEM_H

#include "coreengine/pathfindingsystem.h"

#include "game/unit.h"

class UnitPathFindingSystem;
typedef oxygine::intrusive_ptr<UnitPathFindingSystem> spUnitPathFindingSystem;

class UnitPathFindingSystem : public PathFindingSystem
{
    Q_OBJECT
public:
    explicit UnitPathFindingSystem(Unit* pUnit, Player* pPlayer = nullptr);

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
     * @param costs
     * @return
     */
    virtual bool finished(qint32 x, qint32 y, qint32)  override;
    /**
     * @brief getCosts
     * @param x
     * @param y
     * @return the exact costs needed to get onto the given field. -1 = unreachable
     */
    virtual qint32 getCosts(qint32 x, qint32 y)  override;
    /**
     * @brief getCosts returns the costs need to go the given path
     * @param path the path the unit should go
     * @return the costs needed when using the given path
     */
    qint32 getCosts(QVector<QPoint> path);
    /**
     * @brief getClosestReachableMovePath
     * @param target
     * @return
     */
    QVector<QPoint> getClosestReachableMovePath(QPoint target, qint32 movepoints = -2, bool direct = false);
    /**
     * @brief setMoveCosts
     * @param movepoints how far this pfs explodes. -2 for infinite
     */
    void setMovepoints(const qint32 &movepoints);

protected:
    Unit* m_pUnit;
    Player* m_pPlayer{nullptr};
    qint32 m_Movepoints{-2};
};

#endif // UNITPATHFINDINGSYSTEM_H
