#pragma once

#include <QtGlobal>

#include <cmath>
#include <limits>

namespace PrecisionGrid
{
static constexpr qint32 BOUNDARY_ULPS = 32;

inline qreal normalizeBoundary(qreal value, qreal precision)
{
    const qreal scaledValue = value * precision;
    const qreal nearestInteger = std::round(scaledValue);
    const qreal adjacentValue = std::nextafter(
        nearestInteger,
        scaledValue < nearestInteger
            ? -std::numeric_limits<qreal>::infinity()
            : std::numeric_limits<qreal>::infinity());
    const qreal tolerance = std::abs(nearestInteger - adjacentValue) * BOUNDARY_ULPS;
    if (std::abs(scaledValue - nearestInteger) <= tolerance)
    {
        return nearestInteger;
    }
    return scaledValue;
}

inline qreal floor(qreal value, qreal precision)
{
    return std::floor(normalizeBoundary(value, precision)) / precision;
}

inline qint32 floorToInt(qreal value, qreal precision)
{
    return std::floor(normalizeBoundary(value, precision));
}
}
