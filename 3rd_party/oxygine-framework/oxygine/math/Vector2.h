#pragma once
#include "3rd_party/oxygine-framework/oxygine/math/ScalarMath.h"

#include <QtMath>

namespace oxygine
{
    template <class T>
    class VectorT2 final
    {
        using vector2 = VectorT2<T>;
    public:
        using type = T;
        explicit VectorT2() = default;
        explicit VectorT2(T, T);

        VectorT2& operator+=(const VectorT2&);
        VectorT2& operator-=(const VectorT2&);

        VectorT2 operator + (const VectorT2&) const;
        VectorT2 operator - (const VectorT2&) const;
        VectorT2 operator - () const;

        void set(T x_, T y_)
        {
            x = x_;
            y = y_;
        }

        template <class R>
        VectorT2 operator * (R s) const
        {
            VectorT2 r(*this);
            r.x = type(r.x * s);
            r.y = type(r.y * s);
            return r;
        }
        template <class R>
        VectorT2 operator / (R s) const
        {
            VectorT2 r(*this);
            r.x /= s;
            r.y /= s;
            return r;
        }
        template <class R>
        VectorT2 operator *= (R s)
        {
            x *= s;
            y *= s;
            return (*this);
        }
        template <class R>
        VectorT2 operator /= (R s)
        {
            x /= s;
            y /= s;
            return (*this);
        }
        VectorT2 mult(const VectorT2& r) const
        {
            return VectorT2(x * r.x, y * r.y);
        }
        VectorT2 div(const VectorT2& r) const
        {
            return VectorT2(x / r.x, y / r.y);
        }

        operator VectorT2<float> () const
        {
            return cast< VectorT2<float> >();
        }

        template<typename R>
        R cast() const
        {
            using vec2 = R;
            using vec2type = typename R::type;
            return vec2(vec2type(x), vec2type(y));
        }


        bool operator == (const VectorT2& r) const;
        bool operator != (const VectorT2& r) const;

        T sqlength() const
        {
            return x * x + y * y;
        }

        T x{0};
        T y{0};
    };

    template<class T>
    bool VectorT2<T>::operator == (const VectorT2<T>& r) const
    {
        if (x == r.x && y == r.y)
        {
            return true;
        }
        return false;
    }

    template<class T>
    bool VectorT2<T>::operator != (const VectorT2<T>& r) const
    {
        if (x != r.x || y != r.y)
            return true;
        return false;
    }
    template <class T>
    VectorT2<T>::VectorT2(T X, T Y):
        x(X), y(Y)
    {
    }

    template <class T>
    VectorT2<T>& VectorT2<T>::operator+=(const VectorT2& v)
    {
        x += v.x; y += v.y; return (*this);
    }

    template <class T>
    VectorT2<T>& VectorT2<T>::operator-=(const VectorT2& v)
    {
        x -= v.x; y -= v.y; return (*this);
    }

    template <class T>
    VectorT2<T> VectorT2<T>::operator + (const VectorT2& v) const
    {
        return VectorT2(x + v.x, y + v.y);
    }

    template <class T>
    VectorT2<T> VectorT2<T>::operator - (const VectorT2& v) const
    {
        return VectorT2(x - v.x, y - v.y);
    }

    template <class T>
    VectorT2<T> VectorT2<T>::operator - () const
    {
        return VectorT2<T>(-x, -y);
    }

    using Vector2 = VectorT2<float>;
    using Point = VectorT2<int>;
}
