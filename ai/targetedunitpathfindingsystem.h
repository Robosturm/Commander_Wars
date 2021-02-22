#ifndef TARGETEDUNITPATHFINDINGSYSTEM_H
#define TARGETEDUNITPATHFINDINGSYSTEM_H

#include "game/unitpathfindingsystem.h"

class TargetedUnitPathFindingSystem;
typedef oxygine::intrusive_ptr<TargetedUnitPathFindingSystem> spTargetedUnitPathFindingSystem;

class TargetedUnitPathFindingSystem : public UnitPathFindingSystem
{
    Q_OBJECT
public:
    explicit TargetedUnitPathFindingSystem(Unit* pUnit, QVector<QVector3D>& targets, QVector<QVector<std::tuple<qint32, bool>>>* pMoveCostMap);
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
    virtual bool finished(qint32 x, qint32 y, qint32 movementCosts)  override;
    /**
     * @brief setFinishNode
     */
    virtual void setFinishNode(qint32, qint32) override;
    /**
     * @brief getTargetPath gets the target path shortened by the movepoints of this unit
     * @param movepoints
     * @return
     */
    QPoint getReachableTargetField(qint32 movepoints);
    /**
     * @brief getCosts
     * @param x
     * @param y
     * @return the exact costs needed to get onto the given field. -1 = unreachable
     */
    virtual qint32 getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY)  override;
    /**
     * @brief getUseBasecosts
     * @return
     */
    bool getUseBasecosts() const;
    /**
     * @brief setUseBasecosts
     * @param useBasecosts
     */
    void setUseBasecosts(bool useBasecosts);
    /**
     * @brief getAbortOnCostExceed
     * @return
     */
    bool getAbortOnCostExceed() const;
    /**
     * @brief setAbortOnCostExceed
     * @param abortOnCostExceed
     */
    void setAbortOnCostExceed(bool abortOnCostExceed);

private:
    bool m_useBasecosts{false};
    bool m_abortOnCostExceed{false};
    QVector<QVector3D> m_Targets;
    QVector<std::tuple<qint32, qint32, qint32, float>> m_FinishNodes;
    QVector<QVector<std::tuple<qint32, bool>>>* m_pMoveCostMap;
};

#endif // TARGETEDUNITPATHFINDINGSYSTEM_H
