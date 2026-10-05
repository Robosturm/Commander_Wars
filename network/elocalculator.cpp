#include "network/elocalculator.h"

#include <QtMath>

#include <cmath>
#include <limits>

double EloCalculator::expectedScore(qint32 playerRating, qint32 opponentRating)
{
    const double ratingDifference = static_cast<double>(opponentRating) - playerRating;
    return 1.0 / (1.0 + qPow(10.0, ratingDifference / 400.0));
}

std::optional<qint32> EloCalculator::updatedRating(qint32 playerRating, qint32 opponentRating,
                                                   qint32 kFactor, double score)
{
    if (playerRating < 0 || opponentRating < 0 || kFactor < 0 ||
        !std::isfinite(score) || score < 0.0 || score > 1.0)
    {
        return std::nullopt;
    }
    const double updated = playerRating +
                          qRound(kFactor * (score - expectedScore(playerRating, opponentRating)));
    if (!std::isfinite(updated) || updated < 0.0 ||
        updated > std::numeric_limits<qint32>::max())
    {
        return std::nullopt;
    }
    return static_cast<qint32>(updated);
}
