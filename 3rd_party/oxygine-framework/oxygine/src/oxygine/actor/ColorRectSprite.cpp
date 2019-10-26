#include "ColorRectSprite.h"
#include "../MaterialCache.h"
#include "../RenderDelegate.h"
#include "../RenderState.h"
#include "../utils/stringUtils.h"

namespace oxygine
{
    void ColorRectSprite::copyFrom(const ColorRectSprite& src, cloneOptions opt)
    {
        inherited::copyFrom(src, opt);
    }

    ColorRectSprite::ColorRectSprite()
    {
        STDMaterial mat;
        mat._base = STDRenderer::white;

        _mat = mc().cache(mat);
    }

    ColorRectSprite::~ColorRectSprite()
    {

    }

    void ColorRectSprite::doRender(const RenderState& rs)
    {
        _rdelegate->doRender(this, rs);
    }

    void ColorRectSprite::sizeChanged(const Vector2& size)
    {
        Actor::sizeChanged(size);
    }
}
