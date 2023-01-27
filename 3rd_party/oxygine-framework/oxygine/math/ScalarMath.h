#pragma once
#include <QColor>
#include <QPoint>
#include <QRect>

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
    /***********************************************************************/
    // color operators
    /***********************************************************************/
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
    static QColor operator+(const QColor& c1, const QColor& c2)
    {
        QColor v(clamp(c1.red()      + c2.red()  , 0, 255),
                 clamp(c1.green()    + c2.green(), 0, 255),
                 clamp(c1.blue()     + c2.blue() , 0, 255),
                 clamp(c1.alpha()    + c2.alpha(), 0, 255));
        return v;
    }
    static QColor operator-(const QColor& c1, const QColor& c2)
    {
        QColor v(clamp(c1.red()      - c2.red()  , 0, 255),
                 clamp(c1.green()    - c2.green(), 0, 255),
                 clamp(c1.blue()     - c2.blue() , 0, 255),
                 clamp(c1.alpha()    - c2.alpha(), 0, 255));
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
    static quint32 qRgba(QColor value)
    {
        //return value;
        quint32 out = 0;
        unsigned char* pOut = (unsigned char*)(&out);
        pOut[0] = value.red();
        pOut[1] = value.green();
        pOut[2] = value.blue();
        pOut[3] = value.alpha();
        return out;
    }
}
