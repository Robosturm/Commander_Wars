#pragma once
#include "../oxygine-include.h"
#include <math.h>
#include <stdlib.h>

namespace oxygine
{
    template<class T>
    inline T lerp(T a, T b, float v)
    {
        return T(a + (b - a) * v);
    }

    namespace scalar
    {
        inline float sign(float s)
        {return s < 0 ? -1.0f : 1.0f;}

        inline double sign(double s)
        {return s < 0 ? -1.0 : 1.0;}

        inline bool equal(double a, double b, const double eps = 10e-8)
        {
            return ((a < b + eps) && (a > b - eps)) ? true : false;
        }

        inline bool equal(float a, float b, const float eps = (float)(10e-6))
        {
            return ((a < b + eps) && (a > b - eps)) ? true : false;
        }

        template <class T>
        inline T clamp(T v, T min_, T max_)
        {
            if (v < min_)
                v = min_;
            if (v > max_)
                v = max_;
            return v;
        }
    }
}
