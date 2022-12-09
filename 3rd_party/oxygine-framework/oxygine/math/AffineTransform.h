#pragma once
#include "3rd_party/oxygine-framework/oxygine/math/Vector2.h"

namespace oxygine
{
    template<class T>
    class AffineTransformT final
    {
    public:

        using vector2 = VectorT2<T>;
        using affineTransform = AffineTransformT<T>;

        explicit AffineTransformT() = default;
        explicit AffineTransformT(T a_, T b_, T c_, T d_, T x_, T y_)
            : a(a_),
              b(b_),
              c(c_),
              d(d_),
              x(x_),
              y(y_)
        {
        }

        void translate(const vector2& v)
        {
            x += a * v.x + c * v.y;
            y += b * v.x + d * v.y;
        }

        void invert()
        {
            affineTransform t = *this;

            T det = T(1) / (t.a * t.d - t.b * t.c);

            a = det * t.d;
            b = -det * t.b;
            c = -det * t.c;
            d = det * t.a;
            x = det * (t.c * t.y - t.d * t.x);
            y = det * (t.b * t.x - t.a * t.y);
        }

        affineTransform operator * (const affineTransform& t2) const
        {
            affineTransform r;
            r.a = a * t2.a + b * t2.c;
            r.b = a * t2.b + b * t2.d;
            r.c = c * t2.a + d * t2.c;
            r.d = c * t2.b + d * t2.d;
            r.x = x * t2.a + y * t2.c + t2.x;
            r.y = x * t2.b + y * t2.d + t2.y;
            return r;
        }

        vector2 transform(const vector2& v) const
        {
            return vector2(
                       a * v.x + c * v.y + x,
                       b * v.x + d * v.y + y);
        }
        T a{1}; // m11
        T b{0}; // m12
        T c{0}; // m21
        T d{1}; // m22
        T x{0}; // m31
        T y{0}; // m32
    };

    using AffineTransform = AffineTransformT<float>;
}
