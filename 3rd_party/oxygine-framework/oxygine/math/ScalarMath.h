#pragma once
#ifndef OXYGINE_SCALARMATH
#define OXYGINE_SCALARMATH
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include <qcolor.h>

namespace oxygine
{
    template<class T>
    inline T lerp(T a, T b, float v)
    {
        return T(a + (b - a) * v);
    }
    template<>
    inline QColor lerp<class QColor>(QColor a, QColor b, float v)
    {
        return QColor(lerp(a.red(), b.red(), v),
                      lerp(a.green(), b.green(), v),
                      lerp(a.blue(), b.blue(), v),
                      lerp(a.alpha(), b.alpha(), v));
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
            {
                return min_;
            }
            else if (v > max_)
            {
                return max_;
            }
            return v;
        }
    }

    /**
     * @brief operator * multiplies two colors
     * @param c1 first color
     * @param c2 second color
     * @return multiplied color
     */
    static QColor operator*(const QColor& c1, const QColor& c2)
    {
        return QColor((c1.red() * c2.red()) / 255,
                      (c1.green() * c2.green()) / 255,
                      (c1.blue() * c2.blue()) / 255,
                      (c1.alpha() * c2.alpha()) / 255);
    }
    static QColor operator + (const QColor& c1, const QColor& c2)
    {
        QColor v(scalar::clamp(c1.red()      + c2.red()  , 0, 255),
                 scalar::clamp(c1.green()    + c2.green(), 0, 255),
                 scalar::clamp(c1.blue()     + c2.blue() , 0, 255),
                 scalar::clamp(c1.alpha()    + c2.alpha(), 0, 255));
        return v;
    }

    static QColor operator - (const QColor& c1, const QColor& c2)
    {
        QColor v(scalar::clamp(c1.red()      - c2.red()  , 0, 255),
                 scalar::clamp(c1.green()    - c2.green(), 0, 255),
                 scalar::clamp(c1.blue()     - c2.blue() , 0, 255),
                 scalar::clamp(c1.alpha()    - c2.alpha(), 0, 255));
        return v;
    }
    /**
     * @brief qRgba
     * @param value
     * @return format RRGGBBAA
     */
    static QColor premultiply(QColor value)
    {
        QColor ret((value.red()     * value.alpha()) / 255,
                   (value.green()   * value.alpha()) / 255,
                   (value.blue()    * value.alpha()) / 255,
                   value.alpha());
        return ret;
    }
    /**
     * @brief qRgba
     * @param value
     * @return format RRGGBBAA
     */
    static unsigned int qRgba(QColor value)
    {
        //return value;
        unsigned int out = 0;
        unsigned char* pOut = (unsigned char*)(&out);
        pOut[0] = value.red();
        pOut[1] = value.green();
        pOut[2] = value.blue();
        pOut[3] = value.alpha();
        return out;
    }
}

#endif // OXYGINE_SCALARMATH
