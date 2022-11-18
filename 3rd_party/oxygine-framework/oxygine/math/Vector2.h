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
        void setZero()
        {
            x = 0;
            y = 0;
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

        T length() const
        {
            return (T)qSqrt(x * x + y * y);
        }
        T sqlength() const
        {
            return dot(*this);
        }

        void normalize()
        {
            normalize(*this, *this);
        }
        void normalizeTo(T len)
        {
            normalize();
            *this *= len;
        }
        VectorT2 normalized() const
        {
            VectorT2 t = *this;
            t.normalize();
            return t;
        }
        float distance(const VectorT2& v) const
        {
            return VectorT2(x - v.x, y - v.y).length();
        }
        T dot(const VectorT2& vr) const
        {
            return dot(*this, vr);
        }

        static T dot(const VectorT2& v1, const VectorT2& v2);
        static VectorT2& normalize(VectorT2& out, const VectorT2& v);
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

    template <class T>
    inline T VectorT2<T>::dot(const VectorT2& v1, const VectorT2& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <class T>
    inline VectorT2<T>& VectorT2<T>::normalize(VectorT2<T>& out, const VectorT2<T>& v)
    {
        T norm = T(1.0) / qSqrt(v.x * v.x + v.y * v.y);
        out = v;
        out.x *= norm;
        out.y *= norm;
        return out;
    }
    using Vector2 = VectorT2<float>;
    using Point = VectorT2<int>;
}
