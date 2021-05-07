#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"

namespace oxygine
{

    class TweenAlphaFadeImpl;
    using spTweenAlphaFadeImpl = intrusive_ptr<TweenAlphaFadeImpl>;

    class TweenAlphaFadeImpl : public TweenPostProcess
    {
    public:
        TweenAlphaFadeImpl(bool fadeIn, const PostProcessOptions& opt);
        void render(Actor*, const RenderState&);
    private:
        bool m_fadeIn;
    };

    class TweenAlphaFade: public TweenProxy<TweenAlphaFadeImpl>
    {
    public:
        TweenAlphaFade(bool fadeIn, const PostProcessOptions& opt = PostProcessOptions());
    };
}
