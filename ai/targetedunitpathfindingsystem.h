#ifndef TARGETEDUNITPATHFINDINGSYSTEM_H
#define TARGETEDUNITPATHFINDINGSYSTEM_H

#include "game/unitpathfindingsystem.h"

class TargetedUnitPathFindingSystem : public UnitPathFindingSystem
{
public:
    explicit TargetedUnitPathFindingSystem(Unit* pUnit, QVector<QPoint>& targets);
    /**
     * @brief getRemainingCost
     * @param x
     * @param y
     * @return the remaining costs for this node to reach the target
     */
    virtual qint32 getRemainingCost(qint32 x, qint32 y, qint32) override;
    /**
     * @brief finished checks if this would be the target node to reach
     * @param x
     * @param y
     * @return
     */
    virtual bool finished(qint32 x, qint32 y)  override;
    /**
     * @brief getTargetPath gets the target path shortened by the movepoints of this unit
     * @param movepoints
     * @return
     */
    QPoint getReachableTargetField(qint32 movepoints);
private:
    QVector<QPoint> m_Targets;
};

#endif // TARGETEDUNITPATHFINDINGSYSTEM_H
