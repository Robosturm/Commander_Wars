#include "3rd_party/oxygine-framework/oxygine/tween/TweenGlow.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

namespace oxygine
{
    TweenGlowImpl::TweenGlowImpl(const QColor& c, const PostProcessOptions& opt)
        : TweenPostProcess(opt),
          m_color(c),
          m_downsample(1)
    {
    }

    void TweenGlowImpl::render(Actor* actor, const RenderState& rs)
    {
        STDRenderer* renderer = STDRenderer::getCurrent();
        RenderState r = rs;
        actor->setRenderDelegate(m_prevMaterial);
        actor->render(r);
        actor->setRenderDelegate(this);
        RectF src(0, 0,
                  m_pp.m_screen.getWidth() / (float)m_pp.m_rt->getWidth() / m_downsample,
                  m_pp.m_screen.getHeight() / (float)m_pp.m_rt->getHeight() / m_downsample);
        rsCache().setBlendMode(blend_premultiplied_alpha);
        AffineTransform tr = m_pp.m_transform * m_actor->computeGlobalTransform();
        renderer->setTransform(tr);
        rsCache().setBlendMode(blend_add);
        renderer->flush();
    }

    void TweenGlowImpl::_renderPP()
    {
        PostProcess::initShaders();

        qint32 w = m_pp.m_screen.size.x;
        qint32 h = m_pp.m_screen.size.y;


        spVideoDriver driver = VideoDriver::instance;
        m_downsample = 1;
        spTexture rt = m_pp.m_rt;
        spTexture rt2 = PostProcess::getRTManager().get(spTexture(), w, h, m_pp.m_format);

        Rect rc(0, 0, w, h);

        driver->setShaderProgram(PostProcess::shaderBlurH.get());
        driver->setUniform("step", 1.0f / rt->getWidth());
        PostProcess::pass(rt, rc, rt2, rc);
        QColor c = m_color;
        c.setAlpha(64);
        driver->setShaderProgram(PostProcess::shaderBlurV.get());
        driver->setUniform("step", 1.0f / rt2->getHeight());

        PostProcess::pass(rt2, rc, rt, rc, qRgba(premultiply(c)));
    }

    TweenGlow::TweenGlow(const QColor& color, const PostProcessOptions& opt)
        : TweenProxy<TweenGlowImpl>(spTweenGlowImpl::create(color, opt))
    {
    }
}
