#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"

namespace oxygine
{
    class TweenGlowImpl;
    using spTweenGlowImpl = intrusive_ptr<TweenGlowImpl>;

    class TweenGlowImpl : public TweenPostProcess
    {
    public:
        TweenGlowImpl(const QColor& c, const PostProcessOptions& opt);
        virtual void render(Actor* actor, const RenderState& rs) override;
        virtual void _renderPP() override;
    private:
        QColor m_color;
        qint32 m_downsample;
    };

    class TweenGlow : public TweenProxy<TweenGlowImpl>
    {
    public:
        TweenGlow(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
    };
}
