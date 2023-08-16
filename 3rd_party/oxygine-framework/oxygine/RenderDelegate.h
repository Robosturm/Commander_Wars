#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

namespace oxygine
{
    class RenderDelegate;
    using spRenderDelegate = std::shared_ptr<RenderDelegate>;
    class RenderDelegate final
    {
    public:
        static spRenderDelegate instance;
        explicit RenderDelegate() = default;
       virtual ~RenderDelegate() = default;
        void render(Actor*, const RenderState&);
        void render(ClipRectActor*, const RenderState&);
        void doRender(Sprite*, const RenderState&);
        void doRender(TextField*, const RenderState&);
        void doRender(ColorRectSprite*, const RenderState&);
    };
}
