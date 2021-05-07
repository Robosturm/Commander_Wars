#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"

namespace oxygine
{

    ColorRectSprite::ColorRectSprite()
    {
        STDMaterial mat;
        mat.m_base = STDRenderer::white;

        m_mat = dynamic_pointer_cast<STDMaterial>(MaterialCache::mc().cache(mat));
    }

    ColorRectSprite::~ColorRectSprite()
    {

    }

    void ColorRectSprite::doRender(const RenderState& rs)
    {
        m_rdelegate->doRender(this, rs);
    }

    void ColorRectSprite::sizeChanged(const Vector2& size)
    {
        Actor::sizeChanged(size);
    }
}
