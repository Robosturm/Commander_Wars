#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"

#include <QColor>
#include <QFont>

namespace oxygine
{
    class TextStyle
    {
    public:
        enum HorizontalAlign
        {
            HALIGN_DEFAULT,
            HALIGN_LEFT,
            HALIGN_MIDDLE,
            HALIGN_RIGHT
        };

        explicit TextStyle(QFont rs)
            : font(rs)
        {
        }
        QFont font;
        HorizontalAlign hAlign{HALIGN_DEFAULT};
        bool multiline{false};
        QColor color{Qt::white};
        qint32 borderWidth{4};
        Qt::TextElideMode elideText{Qt::TextElideMode::ElideRight};
    };
}
