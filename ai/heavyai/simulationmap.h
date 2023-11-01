#pragma once

#include "game/gamemap.h"

class SimulationMap final : public GameMap
{
    Q_OBJECT
    enum class SimulationType
    {
        Move,
        DealDamage,
        CapturePoints
    };

    struct SimulationStep
    {
        SimulationType type;
        std::vector<float> stepInfo;
    };
public:

    SimulationMap();

    void loadStartState(GameMap* pMap);
    void restoreInitialState();

    Q_INVOKABLE void moveUnit(QPoint currentPos, QPoint newPos);
    Q_INVOKABLE void dealDamage(QPoint position, float damage);
    Q_INVOKABLE void increaseCapturePoints(QPoint position, qint32 points);
private:
    void resetMoveUnit(SimulationStep & step);
    void resetDealDamage(SimulationStep & step);
    void resetIncreaseCapturePoints(SimulationStep & step);
private:
    std::vector<SimulationStep> m_steps;
};

Q_DECLARE_INTERFACE(SimulationMap, "SimulationMap");
