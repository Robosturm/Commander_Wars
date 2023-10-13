#include "ai/heavyai/situationevaluator.h"

#include "coreengine/globalutils.h"
#include "game/gamemap.h"



SituationEvaluator::SituationEvaluator(Player* pOwner)
    : m_inputVector(1, UNIT_COUNT * UNIT_COUNT * static_cast<qint32>(Features::Max)),
    m_searchRange(GlobalUtils::getSpCircle(0, SEARCH_RANGE)),
    m_pOwner(pOwner)
{
}

void SituationEvaluator::updateInputVector(GameMap* pMap, const QPoint & searchPoint)
{
    getUnitsInRange(pMap, searchPoint);
    for (qint32 i = 0; i < UNIT_COUNT; ++i)
    {
        Unit* pUnit = m_unitsInfo[i].pUnit;
        if (pUnit == nullptr)
        {
            for (qint32 feature = 0; feature < HeavyAiSharedData::Features::Max; ++feature)
            {
                qint32 basePosition = UNIT_COUNT * UNIT_COUNT * feature + i * UNIT_COUNT;
                for (qint32 enemyUnit = 0; enemyUnit < UNIT_COUNT; ++enemyUnit)
                {
                    m_inputVector(0, basePosition + enemyUnit) = 0;
                }
            }
        }
        else
        {
            for (qint32 feature = 0; feature < HeavyAiSharedData::Features::Max; ++feature)
            {
                qint32 basePosition = UNIT_COUNT * UNIT_COUNT * feature + i * UNIT_COUNT;

            }
        }
    }
}

//void SituationEvaluator::updateHp(qint32 basePosition, const UnitInfo & unitInfo)
//{
//    for (qint32 enemyUnit = 0; enemyUnit < UNIT_COUNT; ++enemyUnit)
//    {
//        if (unitInfo.multiplier != m_unitsInfo[enemyUnit].multiplier)
//        {
//            m_inputVector(0, basePosition + enemyUnit) = m_unitsInfo[enemyUnit].pUnit->getHpRounded();
//        }
//        else
//        {
//            m_inputVector(0, basePosition + enemyUnit) = 0;
//        }
//    }
//}

void SituationEvaluator::getUnitsInRange(GameMap* pMap, const QPoint & searchPoint)
{
    qint32 alliedPosition = 0;
    qint32 enemyPosition = UNIT_COUNT - 1;
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
                    m_unitsInfo[enemyPosition].pUnit = pUnit;
                    m_unitsInfo[enemyPosition].multiplier = -1;
                    --enemyPosition;
                }
                else
                {
                    m_unitsInfo[alliedPosition].pUnit = pUnit;
                    m_unitsInfo[alliedPosition].multiplier = 1;
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
        m_unitsInfo[enemyPosition].pUnit = nullptr;
        m_unitsInfo[enemyPosition].multiplier = 1;
    }
}
