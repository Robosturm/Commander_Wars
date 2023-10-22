#include "ai/heavyai/simulationmap.h"

SimulationMap::SimulationMap()
{
#ifdef GRAPHICSUPPORT
    setObjectName("SimulationMap");
#endif
}

void SimulationMap::loadStartState(GameMap* pMap)
{
    QBuffer data;
    data.open(QIODevice::ReadWrite);
    QDataStream stream(&data);
    pMap->serializeObject(stream);
    data.seek(0);
    clearMap();
    deserializer(stream, true);
}

void SimulationMap::restoreInitialState()
{
    using resetCb = void (SimulationMap::*)(SimulationStep & step);
    constexpr resetCb resetCbcallbacks[] =
        {
            &SimulationMap::resetMoveUnit,
            &SimulationMap::resetDealDamage,
            &SimulationMap::resetIncreaseCapturePoints,
        };
    for (qint32 i = m_steps.size() - 1; i >= 0; --i)
    {
        auto & step = m_steps[i];
        (this->*resetCbcallbacks[static_cast<qint32>(step.type)])(step);
    }
    m_steps.clear();
}


void SimulationMap::moveUnit(QPoint currentPos, QPoint newPos)
{
    SimulationStep step;
    step.type = SimulationType::Move;
    spUnit pUnit = getTerrain(currentPos.x(), currentPos.y())->getSpUnit();
    getTerrain(newPos.x(), newPos.y())->setUnit(pUnit);
    step.stepInfo.push_back(currentPos.x());
    step.stepInfo.push_back(currentPos.y());
    step.stepInfo.push_back(newPos.x());
    step.stepInfo.push_back(newPos.y());
    m_steps.push_back(step);
}

void SimulationMap::resetMoveUnit(SimulationStep & step)
{
    spUnit pUnit = getTerrain(step.stepInfo[2], step.stepInfo[3])->getSpUnit();
    getTerrain(step.stepInfo[0], step.stepInfo[1])->setUnit(pUnit);
}

void SimulationMap::dealDamage(QPoint position, float damage)
{
    SimulationStep step;
    step.type = SimulationType::DealDamage;
    spUnit pUnit = getTerrain(position.x(), position.y())->getSpUnit();
    if (pUnit.get() != nullptr)
    {
        pUnit->setHp(pUnit->getHp() - damage, false);
    }
    step.stepInfo.push_back(position.x());
    step.stepInfo.push_back(position.y());
    step.stepInfo.push_back(damage);
    m_steps.push_back(step);
}

void SimulationMap::resetDealDamage(SimulationStep & step)
{
    spUnit pUnit = getTerrain(step.stepInfo[0], step.stepInfo[1])->getSpUnit();
    if (pUnit.get() != nullptr)
    {
        pUnit->setHp(pUnit->getHp() + step.stepInfo[2], false);
    }
}

void SimulationMap::increaseCapturePoints(QPoint position, qint32 points)
{
    SimulationStep step;
    step.type = SimulationType::DealDamage;
    spUnit pUnit = getTerrain(position.x(), position.y())->getSpUnit();
    if (pUnit.get() != nullptr)
    {
        pUnit->setCapturePoints(pUnit->getCapturePoints() + points, false);
    }
    step.stepInfo.push_back(position.x());
    step.stepInfo.push_back(position.y());
    step.stepInfo.push_back(points);
    m_steps.push_back(step);
}

void SimulationMap::resetIncreaseCapturePoints(SimulationStep & step)
{
    spUnit pUnit = getTerrain(step.stepInfo[0], step.stepInfo[1])->getSpUnit();
    if (pUnit.get() != nullptr)
    {
        pUnit->setHp(pUnit->getHp() + step.stepInfo[2], false);
    }

}
