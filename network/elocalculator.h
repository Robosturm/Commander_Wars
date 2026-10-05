#pragma once

#include <QtGlobal>

#include <optional>

class EloCalculator final
{
public:
    static double expectedScore(qint32 playerRating, qint32 opponentRating);
    static std::optional<qint32> updatedRating(qint32 playerRating, qint32 opponentRating,
                                               qint32 kFactor, double score);
};
