#ifndef UNITPATHFINDINGSYSTEM_H
#define UNITPATHFINDINGSYSTEM_H

#include "coreengine/pathfindingsystem.h"

#include "game/unit.h"

class UnitPathFindingSystem;
using spUnitPathFindingSystem = oxygine::intrusive_ptr<UnitPathFindingSystem>;

class UnitPathFindingSystem : public PathFindingSystem
{
    Q_OBJECT
public:
    enum class CollisionIgnore
    {
        Off,
        OnlyNotMovedEnemies,
        All,
    };

    explicit UnitPathFindingSystem(GameMap* pMap, Unit* pUnit, Player* pPlayer = nullptr);
    virtual ~UnitPathFindingSystem() = default;
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
    virtual qint32 getCosts(qint32 index, qint32 x, qint32 y, qint32 curX, qint32 curY, qint32 currentCost)  override;
    /**
     * @brief getCosts returns the costs need to go the given path
     * @param path the path the unit should go
     * @return the costs needed when using the given path
     */
    qint32 getCosts(const std::vector<QPoint> & path);
    /**
     * @brief getClosestReachableMovePath
     * @param target
     * @return
     */
    std::vector<QPoint> getClosestReachableMovePath(QPoint target, qint32 movepoints = -2, bool direct = false);
    /**
     * @brief getClosestReachableMovePath
     * @param path
     * @param movepoints
     * @param direct
     * @return
     */
    std::vector<QPoint> getClosestReachableMovePath(std::vector<QPoint>& path, qint32 movepoints);
    /**
     * @brief getClosestReachableMovePath
     * @param path
     * @param movepoints
     * @return
     */
    std::vector<QPoint> getClosestReachableMovePath(QVector<QPoint>& path, qint32 movepoints);
    /**
     * @brief setMoveCosts
     * @param movepoints how far this pfs explodes. -2 for infinite
     */
    void setMovepoints(const qint32 &movepoints);
    /**
     * @brief isCrossable
     * @param pNodeUnit
     * @param x
     * @param y
     * @param movepoints
     * @return
     */
    bool isCrossable(Unit* pNodeUnit, qint32 x, qint32 y, qint32 curX, qint32 curY, qint32 movementCosts, qint32 movepoints);
    /**
     * @brief blockedByEnemy
     * @param pNodeUnit
     * @return
     */
    bool blockedByEnemy(Unit* pNodeUnit);
    /**
     * @brief getFast
     * @return
     */
    bool getFast() const;
    /**
     * @brief setFast
     * @param fast
     */
    void setFast(bool fast);
    /**
     * @brief getCostInfo
     * @return
     */
    QMap<QString, qint32>& getCostInfo()
    {
        return m_costInfo;
    }
    /**
     * @brief getIgnoreEnemies
     * @return
     */
    CollisionIgnore getIgnoreEnemies() const;
    /**
     * @brief setIgnoreEnemies
     * @param ignoreEnemies
     */
    void setIgnoreEnemies(CollisionIgnore ignoreEnemies);
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
public slots:
    /**
     * @brief getCosts returns the costs need to go the given path
     * @param path the path the unit should go
     * @return the costs needed when using the given path
     */
    qint32 getCosts(const QVector<QPoint> path);
    /**
     * @brief getClosestReachableMovePath
     * @param path
     * @param movepoints
     * @return
     */
    QVector<QPoint> getJsClosestReachableMovePath(QVector<QPoint> path, qint32 movepoints);
protected:
    Unit* m_pUnit;
    Player* m_pPlayer{nullptr};
    qint32 m_Movepoints{-2};
    qint32 m_unitMovepoints{0};
    bool m_fast{false};
    bool m_supportsShortCuts{true};
    bool m_useBasecosts{false};
    CollisionIgnore m_ignoreEnemies{CollisionIgnore::Off};
    QMap<QString, qint32> m_costInfo;
    GameMap* m_pMap{nullptr};
};

#endif // UNITPATHFINDINGSYSTEM_H
