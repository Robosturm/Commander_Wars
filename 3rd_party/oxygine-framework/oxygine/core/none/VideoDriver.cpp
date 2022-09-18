#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spVideoDriver VideoDriver::instance;
    VideoDriver::Stats VideoDriver::m_stats;
    VertexDeclaration VideoDriver::m_VertexDeclaration;

    void VideoDriver::setUniform(const char* id, const QMatrix4x4& mat)
    {
    }

    void VideoDriver::setUniform(const char* id, const Vector2& v)
    {
        setUniform(id, &v, 1);
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f& v)
    {
        setUniform(id, &v, 1);
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f& v)
    {
        setUniform(id, &v, 1);
    }

    VideoDriver::VideoDriver()
    {
        m_rt = spTexture::create();
    }

    VideoDriver::~VideoDriver()
    {
    }

    quint32 VideoDriver::getPT(VideoDriver::PRIMITIVE_TYPE pt)
    {
        return 0;
    }

    quint32 VideoDriver::getBT(VideoDriver::BLEND_TYPE pt)
    {
        return 0;
    }

    spTexture VideoDriver::getRenderTarget() const
    {
        return m_rt;
    }

    const VertexDeclaration* VideoDriver::getVertexDeclaration() const
    {
        return &m_VertexDeclaration;
    }

    void VideoDriver::getViewport(Rect& r) const
    {
    }

    void VideoDriver::setScissorRect(const Rect* rect)
    {
    }

    bool VideoDriver::getScissorRect(Rect& r) const
    {
        return false;
    }

    void VideoDriver::setRenderTarget(spTexture & rt)
    {
        m_rt = rt;
    }

    void VideoDriver::_begin(const Rect& viewport, const QColor* clearColor)
    {
    }

    void VideoDriver::setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest)
    {
    }

    void VideoDriver::setState(STATE state, quint32 value)
    {
    }

    void VideoDriver::restore()
    {
        setDefaultSettings();
    }

    bool VideoDriver::isReady() const
    {
        return true;
    }

    spTexture VideoDriver::createTexture()
    {
        return spTexture::create();
    }

    void VideoDriver::reset()
    {
    }

    void VideoDriver::begin(const Rect& viewport, const QColor* clearColor)
    {
        _begin(viewport, clearColor);
    }

    void VideoDriver::clear(const QColor& color)
    {
    }

    void VideoDriver::setViewport(const Rect& viewport)
    {
    }

    void VideoDriver::setShaderProgram(ShaderProgram* prog_)
    {
        prog_->bind();
        m_programID = prog_->getID();
        m_pShaderProgram = prog_;
    }

    void VideoDriver::setTexture(qint32 sampler, spTexture & t)
    {
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, qint32 primitives)
    {
    }

    void VideoDriver::draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, const quint16* indicesData, quint32 numIndices)
    {
    }

    void VideoDriver::setDefaultSettings()
    {
    }

    void VideoDriver::setUniformInt(const char* id, qint32 v)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform4f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, const Vector2* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, const Uniform3f* v, qint32 num)
    {
    }

    void VideoDriver::setUniform(const char* id, float val)
    {
    }
}
