#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{

    ColorRectSprite::ColorRectSprite()
    {
        Material mat;
        mat.m_base = STDRenderer::white;
        m_mat = MaterialCache::mc().cache(mat);
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
