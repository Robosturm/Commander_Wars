#ifndef ISLANDMAP_H
#define ISLANDMAP_H

#include <QObject>

#include <QVector>

#include "oxygine-framework.h"

class Player;

class IslandMap;
typedef oxygine::intrusive_ptr<IslandMap> spIslandMap;

class IslandMap : public QObject, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit IslandMap(QString unitID, Player* pOwner);
    virtual ~IslandMap() = default;


public slots:
    /**
     * @brief getIsland
     * @param x
     * @param y
     * @return the island for this point -1 means not reachable
     */
    inline qint32 getIsland(qint32 x, qint32 y)
    {
        return m_Islands[x][y];
    }
    QString getMovementType() const;
private:
    QVector<QVector<qint32>> m_Islands;
    QString m_MovementType;
};

#endif // ISLANDMAP_H
