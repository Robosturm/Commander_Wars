#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"

namespace oxygine
{
    class ColorRectSprite;
    using spColorRectSprite = intrusive_ptr<ColorRectSprite>;
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
