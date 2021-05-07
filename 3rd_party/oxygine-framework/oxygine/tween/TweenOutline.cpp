#include "3rd_party/oxygine-framework/oxygine/tween/TweenOutline.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"

namespace oxygine
{

    TweenOutlineImpl::TweenOutlineImpl(const QColor& c, const PostProcessOptions& opt)
        : TweenPostProcess(opt),
          m_color(c),
          m_downsample(1)
    {
        m_matx = spSTDMaterial::create();
        m_matx->m_blend = blend_premultiplied_alpha;
    }

    void TweenOutlineImpl::render(Actor* actor, const RenderState& rs)
    {
        if (!_pp._rt)
        {
            return;
        }
        spNativeTexture rt = _pp._rt;

        m_matx->m_base = rt;
        m_matx->apply();

        STDRenderer* renderer = STDRenderer::getCurrent();
        RectF src(0, 0,
                  _pp._screen.getWidth() / (float)rt->getWidth(),
                  _pp._screen.getHeight() / (float)rt->getHeight());
        RectF dest = _pp._screen.cast<RectF>();
        AffineTransform tr = _pp._transform * _actor->computeGlobalTransform();
        renderer->setTransform(tr);
        QColor color = Qt::white;
        color.setAlpha(255);
        renderer->addQuad(qRgba(premultiply(color)), src, dest);
        RenderState r = rs;
        actor->setRenderDelegate(_prevMaterial);
        actor->render(r);
        actor->setRenderDelegate(this);
    }

    void TweenOutlineImpl::_renderPP()
    {
        PostProcess::initShaders();
        qint32 w = _pp._screen.size.x;
        qint32 h = _pp._screen.size.y;
        if (w < 0 || h < 0)
        {
            return;
        }
        spIVideoDriver driver = IVideoDriver::instance;
        m_downsample = 1;
        spNativeTexture rt = _pp._rt;
        spNativeTexture rt2 = getRTManager().get(0, w, h, _pp._format);

        Rect rc(0, 0, w, h);
        driver->setShaderProgram(PostProcess::shaderBlurH.get());
        driver->setUniform("step", 1.0f / rt->getWidth());
        pass(rt, rc, rt2, rc);


        qint32 alpha = lerp(0, 255, _progress);
        QColor c;
        if (_pp._options._flags & PostProcessOptions::flag_singleR2T)
        {
            c = m_color;
        }
        else
        {
            c = m_color;
            c.setAlpha(alpha);
            c = premultiply(c);
        }

        driver->setShaderProgram(PostProcess::shaderBlurV.get());
        driver->setUniform("step", 1.0f / rt2->getHeight());

        pass(rt2, rc, rt, rc, c);
    }


    TweenOutline::TweenOutline(const QColor& color, const PostProcessOptions& opt)
        : TweenProxy<TweenOutlineImpl>(spTweenOutlineImpl::create(color, opt))
    {
    }
}
