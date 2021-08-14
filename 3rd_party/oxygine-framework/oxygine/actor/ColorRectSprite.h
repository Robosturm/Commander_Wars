#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    DECLARE_SMART(ColorRectSprite, spColorRectSprite);

    class ColorRectSprite: public Sprite
    {
    public:
        explicit ColorRectSprite();
        virtual ~ColorRectSprite() = default;

        void doRender(const RenderState&) override;
        RectF getDestRect() const override;

    protected:
        void sizeChanged(const Vector2& size) override;
    };
}
