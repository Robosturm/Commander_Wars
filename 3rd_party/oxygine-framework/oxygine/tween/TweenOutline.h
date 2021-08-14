#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"

namespace oxygine
{
    class TweenOutlineImpl;
    using spTweenOutlineImpl = intrusive_ptr<TweenOutlineImpl>;
    class TweenOutlineImpl : public TweenPostProcess
    {
    public:
        explicit TweenOutlineImpl(const QColor& c, const PostProcessOptions& opt);
        virtual ~TweenOutlineImpl() = default;
        virtual void render(Actor* actor, const RenderState& rs) override;
        virtual void _renderPP() override;
    private:
        QColor m_color;
        qint32 m_downsample;
        spMaterial m_matx;
    };

    class TweenOutline : public TweenProxy<TweenOutlineImpl>
    {
    public:
        explicit TweenOutline(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
        virtual ~TweenOutline() = default;
    };
}
