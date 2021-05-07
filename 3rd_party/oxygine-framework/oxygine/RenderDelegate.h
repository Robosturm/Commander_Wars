#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"

namespace oxygine
{
    class RenderDelegate : public oxygine::ref_counter
    {
    public:
        virtual ~RenderDelegate();

        virtual void render(Actor*, const RenderState&);
        virtual void render(ClipRectActor*, const RenderState&) {}
        virtual void render(MaskedSprite*, const RenderState&) {}
        virtual void doRender(Sprite*, const RenderState&) {}
        virtual void doRender(TextField*, const RenderState&) {}
        virtual void doRender(ColorRectSprite*, const RenderState&) {}
        virtual void doRender(ProgressBar*, const RenderState&) {}
    };
}
