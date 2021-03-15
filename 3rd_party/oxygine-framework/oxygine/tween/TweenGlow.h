#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"

namespace oxygine
{
    class TweenGlow : public TweenProxy
    {
    public:
        TweenGlow(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
    };
}
