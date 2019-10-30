#pragma once
#include "oxygine-include.h"
#include "../PostProcess.h"

namespace oxygine
{
    class TweenOutline : public TweenProxy
    {
    public:
        TweenOutline(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
    };
}
