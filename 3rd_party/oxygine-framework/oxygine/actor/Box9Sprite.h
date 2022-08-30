#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

#include <vector>

namespace oxygine
{
    class Box9Sprite;
    using spBox9Sprite = intrusive_ptr<Box9Sprite>;
    class Box9Sprite: public Sprite
    {
    public:
        enum StretchMode
        {
            TILING,
            TILING_FULL,
            STRETCHING
        };

        explicit Box9Sprite();
        virtual ~Box9Sprite() = default;
        StretchMode getVerticalMode() const
        {
            return m_vertMode;
        }
        StretchMode getHorizontalMode() const
        {
            return m_horzMode;
        }
        float getGuideX1() const
        {
            return m_guideX[0];
        }
        float getGuideX2() const
        {
            return m_guideX[1];
        }
        float getGuideY1() const
        {
            return m_guideY[0];
        }
        float getGuideY2() const
        {
            return m_guideY[1];
        }
        RectF getInnerArea();
        void setVerticalMode(StretchMode m);
        void setHorizontalMode(StretchMode m);
        /**
        Set distance from left, right, top and bottom edges
        */
        void setGuides(float x1, float x2, float y1, float y2);
        void setVerticalGuides(float x1, float x2);
        void setHorizontalGuides(float y1, float y2);
        virtual bool isOn(const Vector2& localPosition, float localScale) override;
        virtual RectF getDestRect() const override;
    protected:
        virtual void sizeChanged(const Vector2& size)  override;
        virtual void animFrameChanged(const AnimationFrame& f) override;
        virtual void changeAnimFrame(const AnimationFrame& f) override;
        virtual void doRender(const RenderState&) override;
        void prepare();

        inline bool isNormalSprite() const
        {
            return m_guideX[0] == 0 && m_guideX[1] == 0 &&
                   m_guideY[0] == 0 && m_guideY[1] == 0;
        }
    protected:
        bool m_prepared;
        StretchMode m_vertMode;
        StretchMode m_horzMode;
        float m_guideX[2];
        float m_guideY[2];
        std::vector<float> m_guidesX;
        std::vector<float> m_guidesY;
        std::vector<float> m_pointsX;
        std::vector<float> m_pointsY;
    };
}
