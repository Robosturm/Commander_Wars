#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    class STDRenderDelegate;
    typedef oxygine::intrusive_ptr<STDRenderDelegate> spSTDRenderDelegate;

    class STDRenderDelegate : public RenderDelegate
    {
    public:
        static spSTDRenderDelegate instance;

        explicit STDRenderDelegate() = default;
        virtual ~STDRenderDelegate() = default;

        void render(ClipRectActor*,     const RenderState& rs) override;
        void render(MaskedSprite*,      const RenderState& rs) override;
        void doRender(Sprite*,          const RenderState& rs) override;
        void doRender(TextField*,       const RenderState& rs) override;
        void doRender(ColorRectSprite*, const RenderState& rs) override;
        void doRender(ProgressBar*,     const RenderState& rs) override;
    };

}
