#pragma once

#include <QObject>

#include "3rd_party/opennn/opennn/opennn.h"

class Player;
class Unit;
class GameMap;
class QmlVectorPoint;
using spQmlVectorPoint = std::shared_ptr<QmlVectorPoint>;

class SituationEvaluator : public QObject
{
    Q_OBJECT
public:
    static constexpr qint32 UNIT_COUNT = 40;
    static constexpr qint32 SEARCH_RANGE = 30;
    enum class Features
    {
        HP,
        HpDamage,
        FundsDamage,
        MovementPoints,
        Distance,
        HasMoved,
        Defense,
        RepairsOnPosition,
        Max,
    };


    explicit SituationEvaluator(Player* pOwner);

    void updateInputVector(GameMap* pMap, const QPoint & searchPoint);

private:
    void getUnitsInRange(std::array<Unit*, UNIT_COUNT> & units, GameMap* pMap, const QPoint & searchPoint);

private:
    opennn::Tensor<opennn::type, 2> m_inputVector;
    spQmlVectorPoint m_searchRange;
    Player* m_pOwner{nullptr};
};

