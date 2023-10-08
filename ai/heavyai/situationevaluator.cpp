#include "ai/heavyai/situationevaluator.h"

#include "coreengine/globalutils.h"
#include "game/gamemap.h"

SituationEvaluator::SituationEvaluator()
    : m_inputVector(1, UNIT_COUNT * UNIT_COUNT * static_cast<qint32>(Features::Max)),
      m_searchRange(GlobalUtils::getSpCircle(0, SEARCH_RANGE))
{
}

void SituationEvaluator::updateInputVector(GameMap* pMap, const QPoint & searchPoint)
{
    std::vector<Unit*> units;
    getUnitsInRange(units, pMap, searchPoint);

}

void SituationEvaluator::getUnitsInRange(std::vector<Unit*> & units, GameMap* pMap, const QPoint & searchPoint)
{
    for (const auto & point : m_searchRange->getVector())
    {
        QPoint mapPoint = point + searchPoint;
        Terrain* pTerrain = pMap->getTerrain(mapPoint);
        if (pTerrain != nullptr)
        {
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr)
            {
                units.push_back(pUnit);
            }
        }
    }
}
