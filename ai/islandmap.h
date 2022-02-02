#pragma once
#include <QObject>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class Player;
class GameMap;
class IslandMap;
using spIslandMap = oxygine::intrusive_ptr<IslandMap>;

class IslandMap : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
    static constexpr qint32 UNKNOWN = -1;
public:
    explicit IslandMap(GameMap* pMap, const QString & unitID, Player* pOwner);
    virtual ~IslandMap() = default;


public slots:
    /**
     * @brief sameIsland
     * @param x1
     * @param y1
     * @param x2
     * @param y2
     * @return
     */
    inline bool sameIsland(qint32 x1, qint32 y1,
                           qint32 x2, qint32 y2) const
    {
        return (m_Islands[x1][y1] > UNKNOWN &&
                m_Islands[x1][y1] == m_Islands[x2][y2]);
    }
    /**
     * @brief getIsland
     * @param x
     * @param y
     * @return the island for this point -1 means not reachable
     */
    inline qint32 getIsland(qint32 x, qint32 y) const
    {
        return m_Islands[x][y];
    }
    /**
     * @brief getIslandSize
     * @param island
     * @return
     */
    qint32 getIslandSize(qint32 island) const;
    /**
     * @brief getMovementType
     * @return
     */
    QString getMovementType() const;
    /**
     * @brief getValueOnIsland
     * @param island
     * @param ownValue
     * @param enemyValue
     */
    void getValueOnIsland(qint32 island, qint32 &ownValue, qint32 & enemyValue);
private:
    QVector<QVector<qint32>> m_Islands;
    QString m_MovementType;
    Player* m_pOwner;
    GameMap* m_pMap{nullptr};
};
