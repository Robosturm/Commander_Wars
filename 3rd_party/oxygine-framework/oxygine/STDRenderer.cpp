#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/AnimationFrame.h"
#include "3rd_party/oxygine-framework/oxygine/Image.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/core/ImageDataOperations.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"

#include "qfile.h"
#include "qtextstream.h"

class STDRenderer;
typedef oxygine::intrusive_ptr<STDRenderer> spSTDRenderer;

namespace oxygine
{
    static bool _restored = false;
    spSTDRenderer STDRenderer::instance = nullptr;
    spSTDRenderer STDRenderer::current = nullptr;


    spNativeTexture STDRenderer::white;
    spNativeTexture STDRenderer::invisible;

    std::vector<unsigned short> STDRenderer::indices16;
    size_t STDRenderer::maxVertices = 0;
    UberShaderProgram STDRenderer::uberShader;
    QString STDRenderer::fracShaderBody;
    QString STDRenderer::fracTableShaderBody;
    QString STDRenderer::vertexShaderBody;


    RenderStateCache& rsCache()
    {
        static RenderStateCache r;
        return r;
    }

    RenderStateCache::RenderStateCache()
        : _program(0),
          _blend(blend_disabled)
    {
        reset();
    }

    void RenderStateCache::setDriver(IVideoDriver* d)
    {
        _driver = d;
    }

    void RenderStateCache::reset()
    {
        resetTextures();

        _blend = blend_disabled;
        if (_driver)
        {
            _driver->setState(IVideoDriver::STATE_BLEND, 0);
        }
        _program = 0;
    }

    void RenderStateCache::resetTextures()
    {
        for (qint32 i = 0; i < MAX_TEXTURES; ++i)
        {
            _textures[i] = 0;
        }
    }

    void RenderStateCache::setTexture(qint32 sampler, const spNativeTexture& t)
    {
        Q_ASSERT(sampler < MAX_TEXTURES);

        if (_textures[sampler] == t)
        {
            return;
        }
        _textures[sampler] = t;
        _driver->setTexture(sampler, t);
    }


    void RenderStateCache::setBlendMode(blend_mode blend)
    {
        if (_blend == blend)
        {
            return;
        }
        if (blend == 0)
        {
            _driver->setState(IVideoDriver::STATE_BLEND, 0);
        }
        else
        {
            IVideoDriver::BLEND_TYPE src = static_cast<IVideoDriver::BLEND_TYPE>(blend >> 16);
            IVideoDriver::BLEND_TYPE dest = static_cast<IVideoDriver::BLEND_TYPE>(blend & 0xFFFF);
            _driver->setBlendFunc(src, dest);
            _driver->setState(IVideoDriver::STATE_BLEND, 1);
        }
        _blend = blend;

    }

    void RenderStateCache::changeDriver(IVideoDriver* d)
    {
        _driver = d;
    }


    bool RenderStateCache::setShader(ShaderProgram* prog)
    {
        if (_program == prog)
        {
            return false;
        }
        _program = prog;
        _driver->setShaderProgram(prog);
        return true;
    }

    void nullTextureHook(const spNativeTexture&)
    {
    }

    render_texture_hook _renderTextureHook = nullTextureHook;


    render_texture_hook get_render_texture_hook()
    {
        return _renderTextureHook;
    }

    void set_render_texture_hook(render_texture_hook h)
    {
        _renderTextureHook = h;
    }

    template<class V, class XY>
    void fillQuad(V* v, const RectF& uv, XY* positions, const QColor& color)
    {
        const XY& p1 = positions[0];
        const XY& p2 = positions[1];
        const XY& p3 = positions[2];
        const XY& p4 = positions[3];

        V vt;
        vt.color = qRgba(color);
        vt.z = 0;

        vt.x = p1.x;
        vt.y = p1.y;
        vt.u = uv.pos.x;
        vt.v = uv.pos.y;
        *v = vt;
        ++v;

        vt.x = p2.x;
        vt.y = p2.y;
        vt.u = uv.pos.x;
        vt.v = uv.getBottom();
        *v = vt;
        ++v;

        vt.x = p3.x;
        vt.y = p3.y;
        vt.u = uv.getRight();
        vt.v = uv.pos.y;
        *v = vt;
        ++v;

        vt.x = p4.x;
        vt.y = p4.y;
        vt.u = uv.getRight();
        vt.v = uv.getBottom();
        *v = vt;
        ++v;
    }

    void STDRenderer::initialize()
    {
        indices16.reserve(3000 * 6);
        for (qint32 t = 0; t < 3000; t += 1)
        {
            qint32 i = t * 4;
            indices16.push_back(i + 0);
            indices16.push_back(i + 1);
            indices16.push_back(i + 2);

            indices16.push_back(i + 2);
            indices16.push_back(i + 1);
            indices16.push_back(i + 3);
        }

        maxVertices = indices16.size() / 3 * 2;

        if (QFile::exists("system/frac_shader.glsl"))
        {
            QFile file("system/frac_shader.glsl");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fracShaderBody = stream.readAll();
        }
        if (QFile::exists("system/vertex_shader.glsl"))
        {
            QFile file("system/vertex_shader.glsl");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            vertexShaderBody = stream.readAll();
        }
        if (QFile::exists("system/frac_table_shader.glsl"))
        {
            QFile file("system/frac_table_shader.glsl");
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            fracTableShaderBody = stream.readAll();
        }
        uberShader.init(fracShaderBody, vertexShaderBody, fracTableShaderBody);

        restore();
    }

    void STDRenderer::release()
    {
        indices16.clear();
        uberShader.release();
        fracShaderBody.clear();
        vertexShaderBody.clear();
        fracTableShaderBody.clear();
        if (white)
        {
            white->release();
        }
        white = nullptr;

        if (invisible)
        {
            invisible->release();
        }
        invisible = nullptr;
        instance = nullptr;
    }

    void STDRenderer::reset()
    {
        _restored = false;
        if (white)
        {
            white->release();
        }
        white = 0;

        if (invisible)
        {
            invisible->release();
        }
        invisible = 0;

        uberShader.release();
    }

    bool STDRenderer::isReady()
    {
        return _restored;
    }

    void STDRenderer::restore()
    {
        Image memwhite;
        memwhite.init(4, 4, ImageData::TF_R8G8B8A8);

        oxygine::operations::op_fill fill;
        ImageData im = memwhite.lock();
        oxygine::operations::applyOperation(fill, im);

        white = IVideoDriver::instance->createTexture();
        white->init(im, false);
        white->setLinearFilter(GL_LINEAR);
        white->setClamp2Edge(false);


        memwhite.fillZero();
        invisible = IVideoDriver::instance->createTexture();
        invisible->init(im, false);
        invisible->setLinearFilter(GL_LINEAR);
        invisible->setClamp2Edge(false);

        _restored = true;
    }


    STDRenderer* STDRenderer::getCurrent()
    {
        return current.get();
    }

    STDRenderer::~STDRenderer()
    {
        flush();
    }

    const oxygine::Matrix& STDRenderer::getViewProjection() const
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


    void STDRenderer::xdrawBatch()
    {
        size_t count = m_verticesData.size() / m_vdecl->size;
        size_t indices = (count * 3) / 2;

        getDriver()->draw(IVideoDriver::PT_TRIANGLES, m_vdecl, &m_verticesData.front(), (unsigned int)count, &indices16.front(), (unsigned int)indices);

        m_verticesData.clear();
    }

    void STDRenderer::initCoordinateSystem(qint32 width, qint32 height, bool flipU)
    {
        Matrix view = makeViewMatrix(width, height, flipU);
        Matrix proj;
        //initialize projection matrix
        Matrix::orthoLH(proj, (float)width, (float)height, 0, 1);

        Matrix vp = view * proj;
        setViewProj(vp);
    }

    IVideoDriver* STDRenderer::getDriver()
    {
        return m_driver;
    }


    void STDRenderer::setViewProj(const Matrix& viewProj)
    {
        m_vp = viewProj;
        flush();

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

    void STDRenderer::begin()
    {
        Q_ASSERT(m_verticesData.empty() == true);
        m_verticesData.clear();
        m_transform.identity();

        Material::null->apply();

        xbegin();

        current = this;
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

    void STDRenderer::addVertices(const void* data, quint32 size)
    {
        xaddVertices(data, size);
        checkDrawBatch();
    }

    void STDRenderer::xaddVertices(const void* data, quint32 size)
    {
        m_verticesData.insert(m_verticesData.end(), (const unsigned char*)data, (const unsigned char*)data + size);
    }

    void STDRenderer::checkDrawBatch()
    {
        if (m_verticesData.size() / sizeof(m_vdecl->size) >= maxVertices)
        {
            flush();
        }
    }

    Matrix makeViewMatrix(qint32 w, qint32 h, bool flipU)
    {
        Matrix view, scale, tr;
        float offset = 0.5f;

        offset = 0;

        Matrix::translation(tr, Vector3(-(float)w / 2.0f - offset, (flipU ? -1.0f : 1.0f) * (float)h / 2.0f + offset, 0.0f));
        Matrix::scaling(scale, Vector3(1.0f, flipU ? 1.0f : -1.0f, 1.0f));

        view = scale * tr;

        return view;
    }




    bool checkT2P(const Rect& viewport, const Matrix& vp, const vertexPCT2* v1, const vertexPCT2* v2, qint32 w, qint32 h)
    {
        Vector3 p1(v1->x, v1->y, 0);
        Vector3 p2(v2->x, v2->y, 0);

        p1 = vp.transformVec3(p1);
        p2 = vp.transformVec3(p2);

        Vector2 half = viewport.getSize().cast<Vector2>() / 2;
        p1.x = p1.x * half.x + half.x;
        p1.y = p1.y * half.y + half.y;

        p2.x = p2.x * half.x + half.x;
        p2.y = p2.y * half.y + half.y;

        Vector2 tc1(v1->u, v1->v);
        Vector2 tc2(v2->u, v2->v);
        Vector3 dp_ = p1 - p2;
        Vector2 dp(dp_.x, dp_.y);
        dp.x = qAbs(dp.x);
        dp.y = qAbs(dp.y);

        Vector2 dtc = tc1 - tc2;
        dtc.x = qAbs(dtc.x) * w;
        dtc.y = qAbs(dtc.y) * h;

        const float EPS = 0.05f;

        Vector2 d = dp - dtc;
        if (qAbs(d.x) >= EPS || qAbs(d.y) >= EPS)
        {
            return false;
        }
        p1.x = qAbs(p1.x);
        p1.y = qAbs(p1.y);

        if (qAbs(p1.x - int(p1.x + EPS)) > EPS ||
            qAbs(p1.y - int(p1.y + EPS)) > EPS)
        {
            return false;
        }
        return true;
    }



    STDRenderer::STDRenderer(IVideoDriver* driver)
        : m_vdecl(0),
          m_driver(driver),
          m_uberShader(0)
    {
        if (!driver)
        {
            driver = IVideoDriver::instance.get();
        }
        m_driver = driver;
        m_vp.identity();

        m_vdecl = m_driver->getVertexDeclaration(vertexPCT2::FORMAT);

        m_uberShader = &uberShader;
        m_transform.identity();
        m_baseShaderFlags = 0;

        m_sphookFirst = this;
        m_sphookLast  = this;

        hook = [ = ]()
        {
            m_driver->setUniform("mat", m_vp);
        };
    }


    template <class T>
    void append(std::vector<unsigned char>& buff, const T& t)
    {
        const unsigned char* ptr = (const unsigned char*)&t;
        buff.insert(buff.end(), ptr, ptr + sizeof(t));
    }

    bool _showTexel2PixelErrors = false;

    void STDRenderer::swapVerticesData(STDRenderer& r)
    {
        std::swap(m_verticesData, r.m_verticesData);
    }

    void STDRenderer::swapVerticesData(std::vector<unsigned char>& data)
    {
        std::swap(data, m_verticesData);
    }


    void STDRenderer::setTransform(const Transform& tr)
    {
        m_transform = tr;
    }

    void STDRenderer::xbegin()
    {
    }

    void STDRenderer::begin(spNativeTexture nt, const Rect* viewport)
    {
        Q_ASSERT(m_prevRT == 0);
        m_prevRT = m_driver->getRenderTarget();
        m_driver->setRenderTarget(nt);

        Rect vp;
        if (!viewport)
        {
            vp = Rect(0, 0, nt->getWidth(), nt->getHeight());
            viewport = &vp;
        }
        m_driver->setViewport(*viewport);


        initCoordinateSystem(viewport->getWidth(), viewport->getHeight(), true);
        begin();
    }


    void STDRenderer::addQuad(const QColor& clr, const RectF& srcRect, const RectF& destRect)
    {
        QColor color = clr;
        vertexPCT2 v[4];
        fillQuadT(v, srcRect, destRect, m_transform, qRgba(premultiply(color)));

        addVertices(v, sizeof(v));
    }


    void STDRenderer::setShaderFlags(quint32 flags)
    {
        ShaderProgram* sp = m_uberShader->getShaderProgram(m_baseShaderFlags | flags);
        setShader(sp);
    }

    void STDRenderer::flush()
    {
        size_t indices = (m_verticesData.size() / sizeof(vertexPCT2) * 3) / 2;
        if (!indices)
        {
            return;
        }

        m_driver->draw(IVideoDriver::PT_TRIANGLES, m_vdecl,
                      &m_verticesData.front(), (unsigned int)m_verticesData.size(),
                      &STDRenderer::indices16.front(), (unsigned int)indices);

        m_verticesData.clear();
    }


    void STDRenderer::setUberShaderProgram(UberShaderProgram* pr)
    {
        if (m_uberShader == pr)
        {
            return;
        }
        m_uberShader = pr;
    }

    void STDRenderer::setBaseShaderFlags(quint32 fl)
    {
        m_baseShaderFlags = fl;
    }

}
