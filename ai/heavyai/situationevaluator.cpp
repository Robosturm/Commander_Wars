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
    std::array<Unit*, UNIT_COUNT> units({nullptr});
    getUnitsInRange(units, pMap, searchPoint);
    for (qint32 i = 0; i < UNIT_COUNT; ++i)
    {
        Unit* pUnit = units[i];
        if (pUnit == nullptr)
        {

        }
        else
        {

        }
    }
}

void SituationEvaluator::getUnitsInRange(std::array<Unit*, UNIT_COUNT> & units, GameMap* pMap, const QPoint & searchPoint)
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
            if (pUnit != nullptr)
            {
                if (m_pOwner->isEnemyUnit(pUnit))
                {
                    units[enemyPosition] = pUnit;
                    --enemyPosition;
                }
                else
                {
                    units[alliedPosition] = pUnit;
                    ++alliedPosition;
                }
                if (alliedPosition > enemyPosition)
                {
                    break;
                }
            }
        }
    }
}
