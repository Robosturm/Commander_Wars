#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    DECLARE_SMART(Box9Sprite, spBox9Sprite);

    class Box9Sprite: public Sprite
    {
    public:
        enum StretchMode
        {
            TILING,
            TILING_FULL,
            STRETCHING
        };

        Box9Sprite();

        StretchMode getVerticalMode() const {return _vertMode;}
        StretchMode getHorizontalMode() const {return _horzMode;}
        float       getGuideX1() const { return _guideX[0]; }
        float       getGuideX2() const { return _guideX[1]; }
        float       getGuideY1() const { return _guideY[0]; }
        float       getGuideY2() const { return _guideY[1]; }


        RectF       getInnerArea() const;

        void setVerticalMode(StretchMode m);
        void setHorizontalMode(StretchMode m);

        /**
        Set distance from left, right, top and bottom edges
        */
        void setGuides(float x1, float x2, float y1, float y2);
        void setVerticalGuides(float x1, float x2);
        void setHorizontalGuides(float y1, float y2);

        bool isOn(const Vector2& localPosition, float localScale) override;

        RectF getDestRect() const override;

    protected:
        void sizeChanged(const Vector2& size)  override;
        void animFrameChanged(const AnimationFrame& f) override;
        void changeAnimFrame(const AnimationFrame& f) override;

        mutable bool _prepared;

        StretchMode _vertMode;
        StretchMode _horzMode;

        mutable float _guideX[2];
        mutable float _guideY[2];

        mutable QVector<float> _guidesX;
        mutable QVector<float> _guidesY;
        mutable QVector<float> _pointsX;
        mutable QVector<float> _pointsY;

        void prepare() const;

        void doRender(const RenderState&) override;
    };
}
