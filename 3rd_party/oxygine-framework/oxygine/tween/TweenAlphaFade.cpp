#include "3rd_party/oxygine-framework/oxygine/tween/TweenAlphaFade.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"

namespace oxygine
{
    TweenAlphaFadeImpl::TweenAlphaFadeImpl(bool fadeIn, const PostProcessOptions& opt)
        : TweenPostProcess(opt),
          m_fadeIn(fadeIn)
    {
    }

    void TweenAlphaFadeImpl::render(Actor*, const RenderState&)
    {
        if (!_pp._rt)
        {
            return;
        }
        spSTDMaterial mat = spSTDMaterial::create();
        mat->m_base = _pp._rt;
        mat->m_blend = blend_premultiplied_alpha;
        mat->apply();

        qint32 _a = lerp(m_fadeIn ? 0 : 255, m_fadeIn ? 255 : 0, _progress);
        STDRenderer* renderer = STDRenderer::getCurrent();

        spNativeTexture rt = _pp._rt;
        RectF src(0, 0,
                  _pp._screen.getWidth() / (float)rt->getWidth(),
                  _pp._screen.getHeight() / (float)rt->getHeight());
        RectF dest = _pp._screen.cast<RectF>();
        AffineTransform tr = _pp._transform * _actor->computeGlobalTransform();
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
