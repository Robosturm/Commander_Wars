#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{

    ColorRectSprite::ColorRectSprite()
    {
#ifdef GRAPHICSUPPORT
        Material mat;
        mat.m_base = STDRenderer::white;
        m_mat = MaterialCache::mc().cache(mat);
#endif
    }

    void ColorRectSprite::doRender(const RenderState& rs)
    {
        RenderDelegate::instance->doRender(this, rs);
    }

    void ColorRectSprite::sizeChanged(const Vector2& size)
    {
        Actor::sizeChanged(size);
    }

    bool ColorRectSprite::isOn(const Vector2& localPosition, float localScale)
    {
        return Actor::isOn(localPosition, localScale);
    }
}
