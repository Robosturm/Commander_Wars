#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"

namespace oxygine
{
    class TweenOutline : public TweenProxy
    {
    public:
        TweenOutline(const QColor& color, const PostProcessOptions& opt = PostProcessOptions());
    };
}
