#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/ShaderProgramGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resource.h"

#include <qfile.h>
#include <qtextstream.h>


namespace oxygine
{
    spShaderProgram PostProcess::shaderBlurV;
    spShaderProgram PostProcess::shaderBlurH;
    spShaderProgram PostProcess::shaderBlit;
    QVector<PPTask*> PostProcess::m_postProcessItems;
    RenderTargetsManager PostProcess::m_rtm;

    bool _ppBuilt = false;

    void PostProcess::initShaders()
    {
        if (_ppBuilt)
        {
            return;
        }
        _ppBuilt = true;

        spVideoDriver driver = VideoDriver::instance;

        const VertexDeclaration* decl = VideoDriver::instance->getVertexDeclaration();
        // read shader data
        QString vs_h;
        QString vs_v;
        QString fs_blur;
        QString vs_blit;
        QString fs_blit;
        QString filepath = "system/pp_hblur_vs.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            vs_h = stream.readAll();
        }
        filepath = "system/pp_vblur_vs.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            vs_v = stream.readAll();
        }
        filepath = "system/pp_rast_fs.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fs_blur = stream.readAll();
        }
        filepath = "system/pp_blit_vs.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            vs_blit = stream.readAll();
        }
        filepath = "system/pp_blit_fs.glsl";
        if (!QFile::exists(filepath))
        {
            filepath = oxygine::Resource::RCC_PREFIX_PATH + filepath;
        }
        if (QFile::exists(filepath))
        {
            QFile file(filepath);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fs_blit = stream.readAll();
        }
        // create shaders
        shaderBlurV = spShaderProgramGL::create(vs_v, fs_blur, decl);
        driver->setShaderProgram(shaderBlurV.get());
        driver->setUniformInt("s_texture", 0);
        shaderBlurH = spShaderProgramGL::create(vs_h, fs_blur, decl);
        driver->setShaderProgram(shaderBlurH.get());
        driver->setUniformInt("s_texture", 0);
        shaderBlit = spShaderProgramGL::create(vs_blit, fs_blit, decl);
        driver->setShaderProgram(shaderBlit.get());
        driver->setUniformInt("s_texture", 0);
    }

    void PostProcess::freeShaders()
    {
        shaderBlit = nullptr;
        shaderBlurH = nullptr;
        shaderBlurV = nullptr;
    }

    const qint32 ALIGN_SIZE = 256;
    const timeMS TEXTURE_LIVE = timeMS(3000);
    const qint32 MAX_FREE_TEXTURES = 3;
    qint32 alignTextureSize(qint32 v)
    {
        qint32 n = (v - 1) / ALIGN_SIZE;
        return (n + 1) * ALIGN_SIZE;
    }

    class NTP
    {
    public:
        qint32 _w;
        qint32 _h;
        ImageData::TextureFormat _tf;
        NTP(qint32 w, qint32 h, ImageData::TextureFormat tf)
            :
              _w(w),
              _h(h),
              _tf(tf)
        {
        }

        bool operator()(const spTexture& t1, const spTexture&) const
        {
            if (t1->getFormat() < _tf)
            {
                return true;
            }
            if (t1->getWidth() < _w)
            {
                return true;
            }
            return t1->getHeight() < _h;
        }

        static bool cmp(const spTexture& t2, const spTexture& t1)
        {
            if (t1->getFormat() > t2->getFormat())
            {
                return true;
            }
            if (t1->getWidth() > t2->getWidth())
            {
                return true;
            }
            return t1->getHeight() > t2->getHeight();
        }
    };

    bool RenderTargetsManager::isGood(const spTexture& t, qint32 w, qint32 h, ImageData::TextureFormat tf) const
    {
        if (!t)
        {
            return false;
        }
        if (!t->getHandle())
        {
            return false;
        }
        if (t->getFormat() == tf &&
            t->getWidth() >= w && t->getHeight() >= h &&
            t->getWidth() <= (w + ALIGN_SIZE) && t->getHeight() <= (h + ALIGN_SIZE))
        {
            return true;
        }
        return false;
    }

    spTexture RenderTargetsManager::get(spTexture current, qint32 w, qint32 h, ImageData::TextureFormat tf)
    {
        w = alignTextureSize(w);
        h = alignTextureSize(h);
        if (isGood(current, w, h, tf))
        {
            current->setCreationTime(Clock::getTimeMS());
            return current;
        }


        spTexture result;

        auto it = std::lower_bound(m_free.cbegin(), m_free.cend(), result, NTP(w, h, tf));
        if (it != m_free.end())
        {
            const spTexture& t = *it;
            if (isGood(t, w, h, tf))
            {
                result = t;
                m_free.erase(it);
            }
        }

        if (!result)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "unable to find a render target");
        }
        result->setCreationTime(Clock::getTimeMS());
        m_rts.push_back(result);
        return result;
    }



    void RenderTargetsManager::update()
    {
        timeMS tm = Clock::getTimeMS();
        for (size_t i = 0, sz = m_rts.size(); i < sz; ++i)
        {
            spTexture& texture = m_rts[i];
            if (texture->getRefCounter() == 1)
            {
                auto it = std::lower_bound(m_free.cbegin(), m_free.cend(), texture, NTP::cmp);
                m_free.insert(it, texture);
                m_rts.erase(m_rts.cbegin() + i);
                --i;
                --sz;
                continue;
            }
        }

        for (size_t i = 0, sz = m_free.size(); i < sz; ++i)
        {
            spTexture& t = m_free[i];
            timeMS createTime = t->getCreationTime();
            if (createTime + TEXTURE_LIVE > tm)
            {
                continue;
            }
            m_free.erase(m_free.cbegin() + i);
            --i;
            --sz;
        }

        if (m_free.size() > MAX_FREE_TEXTURES)
        {
            m_free.erase(m_free.cbegin(), m_free.cbegin() + m_free.size() - MAX_FREE_TEXTURES);
        }
    }

    void RenderTargetsManager::reset()
    {
        for (qint32 i = 0; i < m_rts.size(); ++i)
        {
            m_rts[i]->release();
        }

        m_free.clear();
        m_rts.clear();
    }

    RenderTargetsManager& PostProcess::getRTManager()
    {
        return m_rtm;
    }

    void PostProcess::addPostProcessItem(PPTask* task)
    {
        if (std::find(m_postProcessItems.begin(), m_postProcessItems.end(), task) == m_postProcessItems.end())
        {
            task->addRefPP();
            m_postProcessItems.push_back(task);
        }
    }

    void PostProcess::removePostProcessItem(PPTask* t)
    {
        auto i = std::find(m_postProcessItems.cbegin(), m_postProcessItems.cend(), t);
        if (i == m_postProcessItems.cend())
        {
            return;
        }
        t->releaseRefPP();
        m_postProcessItems.erase(i);
    }

    void PostProcess::updatePortProcessItems()
    {
        if (!m_postProcessItems.empty())
        {
            spVideoDriver driver = VideoDriver::instance;
            spTexture prevRT = driver->getRenderTarget();
            ShaderProgram* sp = driver->getShaderProgram();

            for (size_t i = 0; i < m_postProcessItems.size(); ++i)
            {
                PPTask* p = m_postProcessItems[i];
                p->renderPP();
                p->releaseRefPP();
            }

            m_postProcessItems.clear();
            driver->setRenderTarget(prevRT);
            if (sp)
            {
                driver->setShaderProgram(sp);
            }
        }
        m_rtm.update();
    }

    void PostProcess::clearPostProcessItems()
    {
        m_postProcessItems.clear();
        m_rtm.reset();
    }

    void PostProcess::pass(spTexture srcTexture, const Rect& srcRect, spTexture destTexture, const Rect& destRect, const QColor& color)
    {
        spVideoDriver driver = VideoDriver::instance;

        const VertexDeclaration* decl = driver->getVertexDeclaration();
        driver->setRenderTarget(destTexture);
        driver->clear(QColor(0, 0, 0, 0));
        driver->setViewport(destRect);
        driver->setTexture(0, srcTexture);

        std::vector<VertexPCT2> quad =
        {
            VertexPCT2(),
            VertexPCT2(),
            VertexPCT2(),
            VertexPCT2()
        };
        RectF dst = srcRect.cast<RectF>() / Vector2((float)srcTexture->getWidth(), (float)srcTexture->getHeight());
        STDRenderer::fillQuad(quad,
                              dst,
                              RectF(-1, -1, 2, 2),
                              AffineTransform::getIdentity(), qRgba(color));

        driver->draw(VideoDriver::PT_TRIANGLE_STRIP, decl, &quad.front(), quad.size());
        driver->setTexture(0, spTexture());
    }

    PostProcess::PostProcess(const PostProcessOptions& opt)
        : m_extend(2, 2),
          m_format(ImageData::TF_R8G8B8A8),
          m_options(opt)

    {
    }

    Rect PostProcess::getScreenRect(const Actor& actor) const
    {
        if (m_options.m_flags & PostProcessOptions::flag_screen)
        {
            return m_screen;
        }
        GameWindow* window = oxygine::GameWindow::getWindow();
        QSize size = window->size();

        Rect display(Point(0, 0), Point(size.width(), size.height()));

        if (m_options.m_flags & PostProcessOptions::flag_fullscreen)
        {
            return display;
        }
        RectF bounds = RectF::invalidated();
        AffineTransform transform = actor.computeGlobalTransform();
        if (m_options.m_flags & PostProcessOptions::flag_fixedBounds)
        {
            const RectF& fb = m_options.m_fixedBounds;
            bounds.unite(transform.transform(fb.getLeftTop()));
            bounds.unite(transform.transform(fb.getRightTop()));
            bounds.unite(transform.transform(fb.getRightBottom()));
            bounds.unite(transform.transform(fb.getLeftBottom()));
        }
        else
        {
            bounds = actor.computeBounds(transform);
        }
        Rect screen;
        screen = bounds.cast<Rect>();

        screen.size += Point(1, 1);
        screen.expand(m_extend, m_extend);

        if (!(m_options.m_flags & PostProcessOptions::flag_singleR2T))
        {
            screen.clip(display);
        }
        return screen.cast<Rect>();
    }

    void PostProcess::update(Actor* actor)
    {
        m_screen = getScreenRect(*actor);
        if (m_screen.isEmpty())
        {
            return;
        }
        m_rt = PostProcess::getRTManager().get(m_rt, m_screen.getWidth(), m_screen.getHeight(), m_format);


        m_transform = actor->computeGlobalTransform().inverted();


        Material::null->apply();


        spVideoDriver driver = VideoDriver::instance;

        driver->setRenderTarget(m_rt);

        Rect vp = m_screen;
        vp.pos = Point(0, 0);
        driver->setViewport(vp);

        driver->clear(m_options.m_clearColor);


        RenderState rs;
        spSTDRenderer renderer = STDRenderer::instance;


        RectF clip = vp.cast<RectF>();
        rs.clip = &clip;

        renderer->initCoordinateSystem(vp.getWidth(), vp.getHeight(), true);

        rs.transform = actor->getParent()->computeGlobalTransform();


        if (!(m_options.m_flags & PostProcessOptions::flag_fullscreen))
        {
            AffineTransform offset;
            offset.identity();
            offset.translate(-m_screen.pos);
            rs.transform = rs.transform * offset;
        }
        RenderDelegate* rd = actor->getRenderDelegate();
        actor->setRenderDelegate(RenderDelegate::instance.get());
        RenderDelegate::instance->RenderDelegate::render(actor, rs);
        STDRenderer::current->flush();
        actor->setRenderDelegate(rd);

        Material::null->apply();
    }

    TweenPostProcess::TweenPostProcess(const PostProcessOptions& opt)
        : m_actor(0),
          m_pp(opt),
          m_prevMaterial(0)
    {
    }

    TweenPostProcess::~TweenPostProcess()
    {
        PostProcess::removePostProcessItem(this);
        if (m_actor && m_actor->getRenderDelegate())
        {
            m_actor->setRenderDelegate(m_prevMaterial);
        }
    }


    void TweenPostProcess::renderPP()
    {
        if (m_pp.m_options.m_flags & PostProcessOptions::flag_singleR2T && m_pp.m_rt)
        {
            return;
        }
        m_pp.update(m_actor);
        _renderPP();
    }

    void TweenPostProcess::addRefPP()
    {
        m_pActorRef = m_actor;
    }

    void TweenPostProcess::releaseRefPP()
    {
        m_pActorRef = nullptr;
    }

    void TweenPostProcess::init(Actor& actor)
    {
        m_actor = &actor;
        m_prevMaterial = m_actor->getRenderDelegate();
        m_actor->setRenderDelegate(this);
    }

    void TweenPostProcess::update(Actor&, float p, const UpdateState&)
    {
        m_progress = p;
        PostProcess::addPostProcessItem(this);
    }

    void TweenPostProcess::done(Actor&)
    {
        if (m_actor->getRenderDelegate())
        {
            m_actor->setRenderDelegate(m_prevMaterial);
        }
    }

}
