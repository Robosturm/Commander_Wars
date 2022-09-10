#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"

#include <QPainter>

namespace oxygine
{
    class RenderDelegate;
    using spRenderDelegate = oxygine::intrusive_ptr<RenderDelegate>;
    class RenderDelegate final : public oxygine::ref_counter
    {
    public:
        static spRenderDelegate instance;
        explicit RenderDelegate() = default;
        ~RenderDelegate() = default;
        void render(Actor*, const RenderState&);
        void render(ClipRectActor*, const RenderState&);
        void doRender(Sprite*, const RenderState&);
        void doRender(TextField*, const RenderState&);
        void doRender(ColorRectSprite*, const RenderState&);
    };
}
