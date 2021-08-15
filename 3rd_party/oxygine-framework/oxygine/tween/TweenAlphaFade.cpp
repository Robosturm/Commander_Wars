#include "3rd_party/oxygine-framework/oxygine/tween/TweenAlphaFade.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    TweenAlphaFadeImpl::TweenAlphaFadeImpl(bool fadeIn, const PostProcessOptions& opt)
        : TweenPostProcess(opt),
          m_fadeIn(fadeIn)
    {
    }

    void TweenAlphaFadeImpl::render(Actor*, const RenderState&)
    {
        if (!m_pp.m_rt)
        {
            return;
        }
        spMaterial mat = spMaterial::create();
        mat->m_base = m_pp.m_rt;
        mat->m_blend = blend_premultiplied_alpha;
        mat->apply();

        qint32 _a = lerp(m_fadeIn ? 0 : 255, m_fadeIn ? 255 : 0, m_progress);
        STDRenderer* renderer = STDRenderer::getCurrent();

        spNativeTexture rt = m_pp.m_rt;
        RectF src(0, 0,
                  m_pp.m_screen.getWidth() / (float)rt->getWidth(),
                  m_pp.m_screen.getHeight() / (float)rt->getHeight());
        RectF dest = m_pp.m_screen.cast<RectF>();
        AffineTransform tr = m_pp.m_transform * m_actor->computeGlobalTransform();
        renderer->setTransform(tr);
        QColor color = QColor(Qt::white);
        color.setAlpha(_a);
        renderer->addQuad(qRgba(premultiply(color)), src, dest);
    }


    TweenAlphaFade::TweenAlphaFade(bool fadeIn, const PostProcessOptions& opt)
        : TweenProxy<TweenAlphaFadeImpl>(spTweenAlphaFadeImpl::create(fadeIn, opt))
    {
    }
}
