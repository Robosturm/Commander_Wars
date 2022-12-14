#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

#include <QFile>
#include <QTextStream>

class STDRenderer;
using spSTDRenderer = oxygine::intrusive_ptr<STDRenderer>;

namespace oxygine
{
    bool STDRenderer::m_restored = false;
    spSTDRenderer STDRenderer::instance{nullptr};
    spSTDRenderer STDRenderer::current{nullptr};
    spTexture STDRenderer::white{nullptr};
    std::vector<unsigned short> STDRenderer::indices16;
    size_t STDRenderer::maxVertices = 0;
    QScopedPointer<UberShaderProgram> STDRenderer::m_uberShader;

    RenderStateCache& rsCache()
    {
        static QScopedPointer<RenderStateCache> r;
        if (r.isNull())
        {
            r.reset(new RenderStateCache());
        }
        return *r.get();
    }

    RenderStateCache::RenderStateCache()
    {
        reset();
    }

    void RenderStateCache::setDriver(VideoDriver* d)
    {
        m_driver = d;
    }

    void RenderStateCache::reset()
    {
        resetTextures();
        m_blend = VideoDriver::blend_disabled;
        if (m_driver)
        {
            m_driver->setState(VideoDriver::STATE_BLEND, 0);
        }
        m_program = nullptr;
    }

    void RenderStateCache::resetTextures()
    {
        for (qint32 i = 0; i < MAX_TEXTURES; ++i)
        {
            m_textures[i] = nullptr;
        }
    }

    void RenderStateCache::setTexture(qint32 sampler, spTexture& t)
    {
        if (sampler >= MAX_TEXTURES)
        {
            oxygine::handleErrorPolicy(oxygine::ep_show_error, "RenderStateCache::setTexture texture error");
        }

        if (m_textures[sampler] == t)
        {
            return;
        }
        m_textures[sampler] = t;
        m_driver->setTexture(sampler, t);
    }

    void RenderStateCache::setBlendMode(VideoDriver::blend_mode blend)
    {
        if (m_blend == blend)
        {
            return;
        }
        m_blend = blend;
        setBlendModeInternal();
    }

    void RenderStateCache::setBlendModeInternal()
    {
        if (m_blend == 0)
        {
            m_driver->setState(VideoDriver::STATE_BLEND, 0);
        }
        else
        {
            VideoDriver::BLEND_TYPE src = static_cast<VideoDriver::BLEND_TYPE>(m_blend >> 16);
            VideoDriver::BLEND_TYPE dest = static_cast<VideoDriver::BLEND_TYPE>(m_blend & 0xFFFF);
            m_driver->setBlendFunc(src, dest);
            m_driver->setState(VideoDriver::STATE_BLEND, 1);
        }
    }

    bool RenderStateCache::setShader(ShaderProgram* prog)
    {
        if (m_program == prog)
        {
            return false;
        }
        m_program = prog;
        m_driver->setShaderProgram(prog);
        return true;
    }

    void RenderStateCache::restoreAfterPainterUse()
    {
        m_driver->setShaderProgram(m_program);
        setBlendModeInternal();        
    }

    void STDRenderer::initialize()
    {
        // load buffer with triangle quad information
        constexpr size_t bufferSize = 3000;
        constexpr size_t triangleInfoSize = 6;
        indices16.reserve(bufferSize * triangleInfoSize);
        for (quint16 t = 0; t < bufferSize; ++t)
        {
            quint16 i = t * 4u;
            indices16.push_back(i + 0u);
            indices16.push_back(i + 1u);
            indices16.push_back(i + 2u);

            indices16.push_back(i + 2u);
            indices16.push_back(i + 1u);
            indices16.push_back(i + 3u);
        }
        maxVertices = indices16.size() * 2u / 3u;
        m_uberShader.reset(new UberShaderProgram());
        m_uberShader->init();

        restore();
    }

    void STDRenderer::release()
    {
        indices16.clear();
        if (!m_uberShader.isNull())
        {
            m_uberShader->release();
            m_uberShader.reset(nullptr);
        }
        if (white)
        {
            white->release();
        }
        white = nullptr;
        instance = nullptr;
        current = nullptr;
    }

    void STDRenderer::reset()
    {
        m_restored = false;
        if (white)
        {
            white->release();
        }
        white = nullptr;
        m_uberShader->release();
        m_uberShader.reset(nullptr);
    }

    bool STDRenderer::isReady()
    {
        return m_restored;
    }

    void STDRenderer::restore()
    {
        QImage imgWhite(32, 32, QImage::Format_RGBA8888);
        imgWhite.fill(Qt::white);
        white = VideoDriver::instance->createTexture();
        white->init(imgWhite);
        white->setLinearFilter(GL_NEAREST);
        white->setClamp2Edge(false);
        m_restored = true;
    }


    STDRenderer* STDRenderer::getCurrent()
    {
        return current.get();
    }

    STDRenderer::~STDRenderer()
    {
        flush();
    }

    const QMatrix4x4& STDRenderer::getViewProjection() const
    {
        return m_vp;
    }

    void STDRenderer::setShader(ShaderProgram* prog)
    {
        if (rsCache().setShader(prog))
        {
            shaderProgramChanged();
            ShaderProgramChangedHook* hook = m_sphookFirst;
            while (hook)
            {
                hook->hook();
                hook = hook->next;
            }
        }
    }

    VideoDriver* STDRenderer::getDriver()
    {
        return m_driver;
    }

    void STDRenderer::setViewProj(const QMatrix4x4& viewProj)
    {
        flush();
        m_vp = viewProj;
        if (!m_driver->getShaderProgram())
        {
            return;
        }
        m_driver->setUniform("mat", m_vp);
    }

    void STDRenderer::pushShaderSetHook(ShaderProgramChangedHook* hook)
    {
        m_sphookLast->next = hook;
        hook->prev = m_sphookLast;
        m_sphookLast = hook;
    }

    void STDRenderer::popShaderSetHook()
    {
        m_sphookLast = m_sphookLast->prev;
        m_sphookLast->next = 0;
    }

    void STDRenderer::end()
    {
        flush();
        if (m_prevRT)
        {
            m_driver->setRenderTarget(m_prevRT);
            m_prevRT = nullptr;
        }

    }

    void STDRenderer::setVertexDeclaration(const VertexDeclaration* decl)
    {
        if (m_vdecl != decl)
        {
            flush();
        }
        m_vdecl = decl;
    }

    void STDRenderer::addVertices(const std::vector<VertexPCT2> & data)
    {
        m_verticesData.insert(m_verticesData.end(), data.begin(), data.end());
        checkDrawBatch();
    }

    void STDRenderer::checkDrawBatch()
    {
        if (m_verticesData.size() >= maxVertices)
        {
            flush();
        }
    }

    STDRenderer::STDRenderer(VideoDriver* driver)
    {
        if (!driver)
        {
            driver = VideoDriver::instance.get();
        }
        m_driver = driver;
        m_vp.setToIdentity();
        m_vdecl = m_driver->getVertexDeclaration();
        m_sphookFirst = this;
        m_sphookLast  = this;

        hook = [this]()
        {
            m_driver->setUniform("mat", m_vp);
        };
    }

    void STDRenderer::setTransform(const AffineTransform& tr)
    {
        m_transform = tr;
    }

    void STDRenderer::addQuad(const QColor& clr, const RectF& srcRect, const RectF& destRect)
    {
        QColor color = clr;
        static std::vector<VertexPCT2> quad =
        {
            VertexPCT2(),
            VertexPCT2(),
            VertexPCT2(),
            VertexPCT2(),
        };
        fillQuad(quad, srcRect, destRect, m_transform, qRgba(premultiply(color)));
        addVertices(quad);
    }

    void STDRenderer::setFracShader(UberShaderProgram::ColorMode fracShader)
    {
        ShaderProgram* sp = m_uberShader->getShaderProgram(fracShader);
        setShader(sp);
    }

    void STDRenderer::flush()
    {
        size_t count = (m_verticesData.size() * 3) >> 1;
        if (count == 0)
        {
            return;
        }
        m_driver->draw(VideoDriver::PT_TRIANGLES, m_vdecl,
                      &m_verticesData.front(),
                      &STDRenderer::indices16.front(), count);
        m_verticesData.clear();
    }
}
