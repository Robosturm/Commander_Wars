#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"

namespace oxygine
{
    class RenderDelegate : public oxygine::ref_counter
    {
    public:
        virtual ~RenderDelegate();

        virtual void render(Actor*, const RenderState& rs);
        virtual void render(ClipRectActor*, const RenderState& rs) {}
        virtual void render(MaskedSprite*, const RenderState& rs) {}
        virtual void doRender(Sprite*, const RenderState& rs) {}
        virtual void doRender(TextField*, const RenderState& rs) {}
        virtual void doRender(ColorRectSprite*, const RenderState& rs) {}
        virtual void doRender(ProgressBar*, const RenderState& rs) {}

        static void setCurrent(void*) {}
    };
}
