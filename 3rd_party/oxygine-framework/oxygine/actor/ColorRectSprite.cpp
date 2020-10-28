#include "ColorRectSprite.h"
#include "../MaterialCache.h"
#include "../RenderDelegate.h"
#include "../RenderState.h"

namespace oxygine
{

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
