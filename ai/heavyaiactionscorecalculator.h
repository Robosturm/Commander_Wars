#ifndef HEAVYAIACTIONSCORECALCULATOR_H
#define HEAVYAIACTIONSCORECALCULATOR_H

#include <QObject>

#include "ai/influencefrontmap.h"
#include "ai/islandmap.h"

/**
 * @brief The HeavyAiActionScoreCalculator class
 * calculates the score for a specific action
 */
class HeavyAiActionScoreCalculator : public QObject
{
    Q_OBJECT
public:
    explicit HeavyAiActionScoreCalculator(const QVector<spIslandMap> & islands,
                                          const InfluenceFrontMap & influenceFrontMap);

signals:

private:
    const QVector<spIslandMap> & m_islands;
    const InfluenceFrontMap & m_InfluenceFrontMap;
};

#endif // HEAVYAIACTIONSCORECALCULATOR_H
