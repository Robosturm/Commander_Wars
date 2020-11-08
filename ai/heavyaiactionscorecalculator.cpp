#include "heavyaiactionscorecalculator.h"

HeavyAiActionScoreCalculator::HeavyAiActionScoreCalculator(const QVector<spIslandMap> & islands,
                                                           const InfluenceFrontMap & influenceFrontMap)
    : m_islands(islands),
      m_InfluenceFrontMap(influenceFrontMap)
{

}
