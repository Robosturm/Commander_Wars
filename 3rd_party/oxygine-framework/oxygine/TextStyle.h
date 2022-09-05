#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

#include <QColor>
#include <QFont>

namespace oxygine
{
    struct Font
    {
#ifdef GRAPHICSUPPORT
        QFont font;
        qint32 borderWidth{2};
        QColor borderColor{Qt::black};
        Qt::PenCapStyle borderCapStyle{Qt::RoundCap};
        Qt::PenJoinStyle borderJoin{Qt::RoundJoin};
        qint32 offsetX{0};
        qint32 offsetY{0};
#endif
    };

    struct TextStyle
    {
        enum HorizontalAlign
        {
            HALIGN_DEFAULT,
            HALIGN_LEFT,
            HALIGN_MIDDLE,
            HALIGN_RIGHT
        };

        explicit TextStyle(const Font & rs)
            : font(rs)
        {
        }
        Font font;
        HorizontalAlign hAlign{HALIGN_DEFAULT};
        bool multiline{false};
        QColor color{Qt::white};
        Qt::TextElideMode elideText{Qt::TextElideMode::ElideRight};
    };
}
