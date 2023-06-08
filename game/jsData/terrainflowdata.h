#pragma once

#include <QObject>
#include <QPoint>

#include "game/GameEnums.h"

class GameMap;

class TerrainFlowData final : public QObject
{
    Q_OBJECT
public:
    explicit TerrainFlowData(GameMap* pMap);
   virtual ~TerrainFlowData() = default;
    /**
     * @brief getOverlayTiles
     * @param terrains
     * @return
     */
    QVector<QPoint> getOverlayTiles(const QStringList & terrains);
    /**
     * @brief getOverlayTileMapping
     * @return
     */
    QVector<qint32> getOverlayTileMapping() const;
    Q_INVOKABLE GameMap *getMap() const;
    Q_INVOKABLE bool addData(QPoint newPosition, qint32 newCosts, GameEnums::FlowDirections newFlowDirection);
    Q_INVOKABLE void addFlowDirection(qint32 index, GameEnums::FlowDirections newFlowDirection);
    Q_INVOKABLE QPoint getPosition(qint32 index) const;
    Q_INVOKABLE qint32 getCosts(qint32 index) const;
    Q_INVOKABLE const GameEnums::FlowDirections getFlowDirection(qint32 index) const;
    Q_INVOKABLE QString getFlowString(qint32 index) const;
    Q_INVOKABLE QString getFlowDirectionString(GameEnums::FlowDirections flowDirection) const;
    Q_INVOKABLE QStringList getAlternateFlowString(GameEnums::FlowDirections flowDirection) const;
    Q_INVOKABLE void clear();
    Q_INVOKABLE qint32 size() const
    {
        return m_positions.size();
    }
    Q_INVOKABLE void print();
private:
    QVector<QPoint> m_positions;
    QVector<qint32> m_costs;
    QVector<GameEnums::FlowDirections> m_flowDirections;
    QVector<qint32> m_overlayTileMapping;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(TerrainFlowData, "TerrainFlowData");
