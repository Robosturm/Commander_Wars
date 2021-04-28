#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"

namespace oxygine
{
    const oxygine::Font* ResFont::getClosestFont(float, qint32 styleFontSize, float& resScale) const
    {
        resScale = 1.0f;
        return getFont(0, styleFontSize);
    }
}
