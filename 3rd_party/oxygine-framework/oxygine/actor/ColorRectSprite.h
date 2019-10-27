#pragma once
#include "../oxygine-include.h"
#include "Sprite.h"

namespace oxygine
{
    DECLARE_SMART(ColorRectSprite, spColorRectSprite);

    class ColorRectSprite: public Sprite
    {
    public:
        DECLARE_COPYCLONE_NEW(ColorRectSprite);
        ColorRectSprite();
        ~ColorRectSprite();

        void doRender(const RenderState&) override;

        RectF getDestRect() const override { return Actor::getDestRect(); }

    protected:
        void sizeChanged(const Vector2& size) override;
    };
}
