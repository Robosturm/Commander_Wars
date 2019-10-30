#pragma once
#include "oxygine-include.h"
#include "STDRenderDelegate.h"
#include "core/Renderer.h"

namespace oxygine
{
    class RState
    {
    public:
        RState(): alpha(255), clip(0)
        {
            transform.identity();
        }

        QColor getFinalColor(const QColor& clr) const
        {
            QColor color = clr;
            color.setAlpha((color.alpha() * static_cast<qint32>(alpha)) / 255);
            return color;
        }

        Transform transform;
        unsigned char alpha;
        const RectF* clip;
    };

    class RenderState: public RState
    {
    public:
        RenderState() {}
    };
}
