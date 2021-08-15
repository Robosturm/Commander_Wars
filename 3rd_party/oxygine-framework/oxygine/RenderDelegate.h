#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

namespace oxygine
{
    class RenderDelegate;
    using  spRenderDelegate = oxygine::intrusive_ptr<RenderDelegate>;
    class RenderDelegate : public oxygine::ref_counter
    {
    public:
        static spRenderDelegate instance;

        explicit RenderDelegate() = default;
        virtual ~RenderDelegate() = default;
        virtual void render(Actor*, const RenderState&);
        void render(ClipRectActor*, const RenderState&);
        void render(MaskedSprite*, const RenderState&);
        void doRender(Sprite*, const RenderState&);
        void doRender(TextField*, const RenderState&);
        void doRender(ColorRectSprite*, const RenderState&);
    };
}
