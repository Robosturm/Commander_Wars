#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"

namespace oxygine
{

    class TweenAlphaFadeImpl;
    using spTweenAlphaFadeImpl = intrusive_ptr<TweenAlphaFadeImpl>;

    class TweenAlphaFadeImpl : public TweenPostProcess
    {
    public:
        explicit TweenAlphaFadeImpl(bool fadeIn, const PostProcessOptions& opt);
        virtual ~TweenAlphaFadeImpl() = default;
        void render(Actor*, const RenderState&);
    private:
        bool m_fadeIn;
    };

    class TweenAlphaFade: public TweenProxy<TweenAlphaFadeImpl>
    {
    public:
        explicit TweenAlphaFade(bool fadeIn, const PostProcessOptions& opt = PostProcessOptions());
        virtual ~TweenAlphaFade() = default;
    };
}
