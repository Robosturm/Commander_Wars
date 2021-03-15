#include "3rd_party/oxygine-framework/oxygine/tween/TweenGlow.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VertexDeclarationGL.h"

namespace oxygine
{



    class TweenGlowImpl : public TweenPostProcess
    {
    public:
        QColor _color;
        int _downsample;

        TweenGlowImpl(const QColor& c, const PostProcessOptions& opt) : TweenPostProcess(opt), _color(c), _downsample(1) {}

        void render(Actor* actor, const RenderState& rs) override
        {
            STDRenderer* renderer = STDRenderer::getCurrent();
            RenderState r = rs;
            actor->setRenderDelegate(_prevMaterial);
            actor->render(r);
            actor->setRenderDelegate(this);


            RectF src(0, 0,
                      _pp._screen.getWidth() / (float)_pp._rt->getWidth() / _downsample,
                      _pp._screen.getHeight() / (float)_pp._rt->getHeight() / _downsample);
            rsCache().setBlendMode(blend_premultiplied_alpha);
            AffineTransform tr = _pp._transform * _actor->computeGlobalTransform();
            renderer->setTransform(tr);
            //renderer->applySimpleMode(true);
//            Color color = Color(Color::White).withAlpha(255).premultiplied();
//            color = QColor::White;
            rsCache().setBlendMode(blend_add);
            //renderer->draw(_pp._rt, color.rgba(), src, dest);
            renderer->flush();
        }

        void _renderPP() override
        {
            PostProcess::initShaders();

            int w = _pp._screen.size.x;
            int h = _pp._screen.size.y;


            spIVideoDriver driver = IVideoDriver::instance;
            _downsample = 1;
            spNativeTexture rt = _pp._rt;
            spNativeTexture rt2 = getRTManager().get(0, w, h, _pp._format);

            Rect rc(0, 0, w, h);

            driver->setShaderProgram(PostProcess::shaderBlurH.get());
            driver->setUniform("step", 1.0f / rt->getWidth());
            pass(rt, rc, rt2, rc);
            QColor c = _color;
            c.setAlpha(64);
            driver->setShaderProgram(PostProcess::shaderBlurV.get());
            driver->setUniform("step", 1.0f / rt2->getHeight());

            pass(rt2, rc, rt, rc, qRgba(premultiply(c)));
        }
    };


    TweenGlow::TweenGlow(const QColor& color, const PostProcessOptions& opt) : TweenProxy(new TweenGlowImpl(color, opt))
    {
    }
}
