#pragma once
#include "oxygine-include.h"
#include "actor/Actor.h"
#include "actor/Button.h"
#include "actor/Sprite.h"
#include "actor/TextField.h"

namespace oxygine
{

    namespace args
    {
        template<typename value, typename valueRef, typename C, void (C::*SetF)(valueRef)>
        class argT
        {
            bool _init;
            value _v;

        public:
            argT(): _init(false) {}

            argT& operator = (valueRef v)
            {
                _init = true;
                _v = v;
                return *this;
            }

            argT& operator = (intrusive_ptr<C> v)
            {
                _init = true;
                _v = v.get();
                return *this;
            }

            void apply(C* c) const
            {
                if (!_init)
                    return;
                return (c->*SetF)(_v);
            }
        };

        typedef argT<Vector2, const Vector2&, Actor, &Actor::setAnchor> anchor;
        typedef argT<Vector2, const Vector2&, Actor, &Actor::setPosition> position;
        typedef argT<float, float, Actor, &Actor::setX> x;
        typedef argT<float, float, Actor, &Actor::setY> y;
        typedef argT<float, float, Actor, &Actor::setRotation> rotation;
        typedef argT<bool, bool, Actor, &Actor::setVisible> visible;
        typedef argT<float, float, Actor, &Actor::setWidth> width;
        typedef argT<float, float, Actor, &Actor::setHeight> height;
        typedef argT<Vector2, const Vector2&, Actor, &Actor::setSize> size;
        typedef argT<float, float, Actor, &Actor::setScale> scale;
        typedef argT<Vector2, const Vector2&, Actor, &Actor::setScale> scale2;
        typedef argT<float, float, Actor, &Actor::setScaleX> scaleX;
        typedef argT<float, float, Actor, &Actor::setScaleY> scaleY;
        typedef argT<unsigned char, unsigned char, Actor, &Actor::setAlpha> alpha;
        typedef argT<bool, bool, Actor, &Actor::setTouchEnabled> input;
        typedef argT<Actor*, Actor*, Actor, &Actor::attachTo> attachTo;
        typedef argT<QColor, const QColor&, VStyleActor, &VStyleActor::setColor> color;
        typedef argT<blend_mode, blend_mode, VStyleActor, &VStyleActor::setBlendMode> blend;
        typedef argT<qint32, qint32, Actor, &Actor::setPriority> priority;
        typedef argT<char, char, Actor, &Actor::setExtendedClickArea> extendedClickArea;

        typedef argT<TextStyle, const TextStyle&, TextField, &TextField::setStyle> style;
        typedef argT<QString, QString, TextField, &TextField::setText> text;
        typedef argT<QString, QString, TextField, &TextField::setHtmlText> htmlText;
        typedef argT<TextStyle::HorizontalAlign, TextStyle::HorizontalAlign, TextField, &TextField::setHAlign> hAlign;
        typedef argT<TextStyle::VerticalAlign, TextStyle::VerticalAlign, TextField, &TextField::setVAlign> vAlign;
        typedef argT<int, int, TextField, &TextField::setFontSize> fontSize;
        typedef argT<const ResFont*, const ResFont*, TextField, &TextField::setFont> font;
        typedef argT<bool, bool, TextField, &TextField::setMultiline> multiline;


        class animFrame
        {
            qint32 _col;
            qint32 _row;
            const ResAnim* _resAnim;
            bool _useResAnim;
            bool _managed;
            AnimationFrame _frame;

        public:
            animFrame(bool managed, qint32 col, qint32 row): _col(col), _row(row), _resAnim(0), _useResAnim(true), _managed(managed) {}

            animFrame& operator = (const ResAnim* r)
            {
                _useResAnim = true;
                _resAnim = r;
                return *this;
            }

            animFrame& operator = (const AnimationFrame& frame)
            {
                _useResAnim = false;
                _frame = frame;
                return *this;
            }

            void apply(Sprite* s) const
            {
                if (_managed)
                    s->setManageResAnim(_managed);
                if (_useResAnim)
                    return s->setAnimFrame(_resAnim, _col, _row);
                return s->setAnimFrame(_frame);
            }

            void apply(Button* b) const
            {
                Q_ASSERT(_useResAnim);
                b->setResAnim(_resAnim);
            }
        };
    }

    template<class T>
    T initActor(T actor)
    {
        return actor;
    }

    template<class T, class A1>
    T initActor(T actor, const A1& arg1)
    {
        initActor(actor);
        arg1.apply(actor);

        return actor;
    }

    template<class T, class A1, class A2>
    T initActor(T actor, const A1& arg1, const A2& arg2)
    {
        initActor(actor, arg1);
        arg2.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3)
    {
        initActor(actor, arg1, arg2);
        arg3.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4)
    {
        initActor(actor, arg1, arg2, arg3);
        arg4.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4, class A5>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5)
    {
        initActor(actor, arg1, arg2, arg3, arg4);
        arg5.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6)
    {
        initActor(actor, arg1, arg2, arg3, arg4, arg5);
        arg6.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6, const A7& arg7)
    {
        initActor(actor, arg1, arg2, arg3, arg4, arg5, arg6);
        arg7.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6, const A7& arg7, const A8& arg8)
    {
        initActor(actor, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        arg8.apply(actor);
        return actor;
    }

    template<class T, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    T initActor(T actor, const A1& arg1, const A2& arg2, const A3& arg3, const A4& arg4, const A5& arg5, const A6& arg6, const A7& arg7, const A8& arg8, const A9& arg9)
    {
        initActor(actor, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        arg9.apply(actor);
        return actor;
    }
}
