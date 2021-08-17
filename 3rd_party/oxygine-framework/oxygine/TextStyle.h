#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "qcolor.h"

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

        enum VerticalAlign
        {
            VALIGN_DEFAULT,
            VALIGN_BASELINE,
            VALIGN_TOP,
            VALIGN_MIDDLE,
            VALIGN_BOTTOM
        };

        explicit TextStyle(const ResFont* rs = nullptr)
            : font(rs)
        {
        }

        const ResFont* font;
        HorizontalAlign hAlign{HALIGN_DEFAULT};
        VerticalAlign vAlign{VALIGN_DEFAULT};
        qint32 linesOffset{0};      /**<vertical distance offset between lines */
        qint32 kerning{0};          /**<horizontal distance */
        bool multiline{false};
        qint32 fontSize{0};
        bool breakLongWords{false}; /**<works with multiline flag. breakLongWords = false doesn't allow to break too long words  */
        QColor color{Qt::white};
        float baselineScale{1.0f};  /**<baseline distance multiplier */
    };
}
