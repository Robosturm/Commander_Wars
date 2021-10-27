#pragma once

#include <QObject>
#include <QPoint>

#include "game/GameEnums.h"

class TerrainFlowData : public QObject
{
    Q_OBJECT
public:
    explicit TerrainFlowData();
    virtual ~TerrainFlowData() = default;
public slots:
    bool addData(QPoint newPosition, qint32 newCosts, GameEnums::FlowDirections newFlowDirection);
    void addFlowDirection(qint32 index, GameEnums::FlowDirections newFlowDirection);
    QPoint getPosition(qint32 index) const;
    qint32 getCosts(qint32 index) const;
    const GameEnums::FlowDirections getFlowDirection(qint32 index) const;
    QString getFlowString(qint32 index) const;
    void clear();
    qint32 size() const
    {
        return m_positions.size();
    }
    void print();
private:
    QVector<QPoint> m_positions;
    QVector<qint32> m_costs;
    QVector<GameEnums::FlowDirections> m_flowDirections;
};

