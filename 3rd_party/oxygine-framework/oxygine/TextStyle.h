#pragma once

#include <QColor>
#include <QFont>

namespace oxygine
{
    struct Font
    {
#ifdef GRAPHICSUPPORT
        QFont font;
        QColor mainColor{Qt::black};
        Qt::PenCapStyle borderCapStyle{Qt::RoundCap};
        Qt::PenJoinStyle borderJoin{Qt::RoundJoin};
        qint32 offsetX{0};
        qint32 offsetY{0};
        bool antialiasing{false};
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
#ifdef GRAPHICSUPPORT
            , color(rs.mainColor)
#endif
        {
        }
        Font font;
        HorizontalAlign hAlign{HALIGN_DEFAULT};
        bool multiline{false};
        QColor color{Qt::white};
        Qt::TextElideMode elideText{Qt::TextElideMode::ElideRight};
    };
}
