#include "ai/heavyai/situationevaluator.h"
#include "ai/heavyai/unittargetedpathfindingsystem.h"

#include "coreengine/globalutils.h"
#include "game/gamemap.h"



SituationEvaluator::SituationEvaluator(Player* pOwner)
    : m_inputVector(1, HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::SituationFeatures::MaxFeatures),
    m_searchRange(GlobalUtils::getSpCircle(0, HeavyAiSharedData::SEARCH_RANGE)),
    m_pOwner(pOwner)
{
    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        m_unitsInfo[i] = MemoryManagement::create<HeavyAiSharedData::UnitInfo>();
        m_unitsInfo[i]->stealthed.resize(pOwner->getMap()->getPlayerCount());
    }
}

void SituationEvaluator::updateInputVector(GameMap* pMap, const QPoint & searchPoint, bool updateUnitData)
{
    if (updateUnitData)
    {
        getUnitsInRange(pMap, searchPoint);
        createPathFindingSystems(pMap);
    }

    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        Unit* pUnit = m_unitsInfo[i]->pUnit;
        if (pUnit == nullptr)
        {
            clearUnitInput(i);
        }
        else
        {
            fillUnitInput(i);
        }
    }
}

void SituationEvaluator::clearUnitInput(qint32 index)
{
    for (qint32 feature = 0; feature < HeavyAiSharedData::SituationFeatures::MaxFeatures; ++feature)
    {
        qint32 basePosition = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * feature + index * HeavyAiSharedData::UNIT_COUNT;
        for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::fillUnitInput(qint32 index)
{
    for (qint32 feature = 0; feature < HeavyAiSharedData::SituationFeatures::MaxFeatures; ++feature)
    {
        qint32 basePosition = HeavyAiSharedData::UNIT_COUNT * HeavyAiSharedData::UNIT_COUNT * feature + index * HeavyAiSharedData::UNIT_COUNT;
        using updateFeature = void (SituationEvaluator::*)(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo);
        constexpr std::array<updateFeature, HeavyAiSharedData::SituationFeatures::MaxFeatures> featureCb{
            &SituationEvaluator::updateHp,
            &SituationEvaluator::updateHpDamage,
            &SituationEvaluator::updateFundsDamage,
            &SituationEvaluator::updateMovementPoints,
            &SituationEvaluator::updateDistance,
            &SituationEvaluator::updateHasMoved,
            &SituationEvaluator::updateDefense,
            &SituationEvaluator::updateRepairsOnPosition,
            &SituationEvaluator::updateCapturePoints,
            &SituationEvaluator::updateBuildingImportance,
            &SituationEvaluator::updateStealthed,
        };
        (this->*featureCb[feature])(basePosition, m_unitsInfo[index]);
    }
}

void SituationEvaluator::updateHp(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getHpRounded();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateHpDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{

}

void SituationEvaluator::updateFundsDamage(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{

}

void SituationEvaluator::updateMovementPoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::UnitMovementPoints];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateDistance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (m_unitsInfo[enemyUnit]->pUnit != nullptr)
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->multiplier * GlobalUtils::getDistance(unitInfo->pUnit->getX(), unitInfo->pUnit->getY(),
                                                                                  m_unitsInfo[enemyUnit]->pUnit->getX(), m_unitsInfo[enemyUnit]->pUnit->getY());
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateHasMoved(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (unitInfo->reachable[enemyUnit])
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getHasMoved();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateDefense(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getAiCache()[HeavyAiSharedData::AiCache::TerrainDefense];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateRepairsOnPosition(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{

}

void SituationEvaluator::updateCapturePoints(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {
            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->pUnit->getCapturePoints();
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

void SituationEvaluator::updateBuildingImportance(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{

}

void SituationEvaluator::updateStealthed(qint32 basePosition, const HeavyAiSharedData::spUnitInfo & unitInfo)
{
    for (qint32 enemyUnit = 0; enemyUnit < HeavyAiSharedData::UNIT_COUNT; ++enemyUnit)
    {
        if (shouldFillInfo(unitInfo, enemyUnit))
        {

            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit]->stealthed[unitInfo->pUnit->getOwner()->getPlayerID()];
        }
        else
        {
            m_inputVector(0, basePosition + enemyUnit) = 0;
        }
    }
}

bool SituationEvaluator::shouldFillInfo(const HeavyAiSharedData::spUnitInfo & unitInfo, qint32 enemyUnit)
{
    return unitInfo->multiplier != m_unitsInfo[enemyUnit]->multiplier &&
           unitInfo->reachable[enemyUnit];
}

void SituationEvaluator::getUnitsInRange(GameMap* pMap, const QPoint & searchPoint)
{
    qint32 alliedPosition = 0;
    qint32 enemyPosition = HeavyAiSharedData::UNIT_COUNT - 1;
    for (const auto & point : m_searchRange->getVector())
    {
        QPoint mapPoint = point + searchPoint;
        Terrain* pTerrain = pMap->getTerrain(mapPoint);
        if (pTerrain != nullptr)
        {
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr && pUnit->getHpRounded() > 0)
            {
                if (m_pOwner->isEnemyUnit(pUnit))
                {
                    m_unitsInfo[enemyPosition]->pUnit = pUnit;
                    m_unitsInfo[enemyPosition]->multiplier = -1;
                    updateStealthInfo(pMap, enemyPosition);
                    --enemyPosition;
                }
                else
                {
                    m_unitsInfo[alliedPosition]->pUnit = pUnit;
                    m_unitsInfo[alliedPosition]->multiplier = 1;
                    updateStealthInfo(pMap, alliedPosition);
                    ++alliedPosition;
                }
                if (alliedPosition > enemyPosition)
                {
                    break;
                }
            }
        }
    }
    for (qint32 i = alliedPosition; i <= enemyPosition; ++i)
    {
        m_unitsInfo[enemyPosition]->pUnit = nullptr;
        m_unitsInfo[enemyPosition]->pUnitTargetedPathFindingSystem.reset();
        m_unitsInfo[enemyPosition]->multiplier = 1;
    }
}

void SituationEvaluator::updateStealthInfo(GameMap* pMap, qint32 unitPosition)
{
    for (qint32 i = 0; i < pMap->getPlayerCount(); ++i)
    {
        m_unitsInfo[unitPosition]->stealthed[i] = m_unitsInfo[unitPosition]->pUnit->isStealthed(pMap->getPlayer(i));
    }
}

void SituationEvaluator::createPathFindingSystems(GameMap* pMap)
{
    for (qint32 i = 0; i < HeavyAiSharedData::UNIT_COUNT; ++i)
    {
        if (m_unitsInfo[i]->pUnit != nullptr)
        {
            m_unitsInfo[i]->pUnitTargetedPathFindingSystem = MemoryManagement::create<UnitTargetedPathFindingSystem>(pMap, i, m_unitsInfo);
        }
    }
}
