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

        static affineTransform& multiply(affineTransform& out, const affineTransform& t1, const affineTransform& t2)
        {
            out.a = t1.a * t2.a + t1.b * t2.c;
            out.b = t1.a * t2.b + t1.b * t2.d;
            out.c = t1.c * t2.a + t1.d * t2.c;
            out.d = t1.c * t2.b + t1.d * t2.d;
            out.x = t1.x * t2.a + t1.y * t2.c + t2.x;
            out.y = t1.x * t2.b + t1.y * t2.d + t2.y;

            return out;
        }

        affineTransform operator * (const affineTransform& t2) const
        {
            affineTransform r;
            multiply(r, *this, t2);
            return r;
        }

        vector2 transform(const vector2& v) const
        {
            return vector2(
                       a * v.x + c * v.y + x,
                       b * v.x + d * v.y + y);
        }
        T a{1};
        T b{0};
        T c{0};
        T d{1};
        T x{0};
        T y{0};
    };

    using AffineTransform = AffineTransformT<float>;
}
