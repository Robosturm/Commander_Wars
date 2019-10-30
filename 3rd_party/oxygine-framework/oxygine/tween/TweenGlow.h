#pragma once
#include "oxygine-include.h"
#include "../PostProcess.h"

namespace oxygine
{
    class TweenGlow : public TweenProxy
    {
    public:
        TweenGlow(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
    };
}
