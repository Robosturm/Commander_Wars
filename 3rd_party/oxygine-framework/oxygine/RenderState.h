#pragma once
#include <QTransform>
#include <QRect>
#include <QColor>

namespace oxygine
{

    class RenderState final
    {
    public:
        explicit RenderState() = default;
        ~RenderState() = default;

        QColor getFinalColor(const QColor& clr) const
        {
            QColor color = clr;
            color.setAlpha((color.alpha() * static_cast<qint32>(alpha)) / 255);
            return color;
        }

        QTransform transform;
        unsigned char alpha{255};
        QRect * clip{nullptr};
    };
}
