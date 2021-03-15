#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{
    spIVideoDriver IVideoDriver::instance = nullptr;

    IVideoDriver::~IVideoDriver()
    {

    }

    void IVideoDriver::setUniform(const char* id, const Matrix& v)
    {
        setUniform(id, &v, 1);
    }

    void IVideoDriver::setUniform(const char* id, const Vector2& v)
    {
        setUniform(id, &v, 1);
    }

    void IVideoDriver::setUniform(const char* id, const Vector3& v)
    {
        setUniform(id, &v, 1);
    }

    void IVideoDriver::setUniform(const char* id, const Vector4& v)
    {
        setUniform(id, &v, 1);
    }

    IVideoDriver::Stats IVideoDriver::_stats;

    spNativeTexture VideoDriverNull::createTexture()
    {
        //return new NativeTextureNull;
        return nullptr;
    }

    void VideoDriverNull::begin(const Rect&, const QColor*)
    {

    }

    void VideoDriverNull::getViewport(Rect&) const
    {

    }

    bool VideoDriverNull::getScissorRect(Rect&) const
    {
        return false;
    }

    spNativeTexture VideoDriverNull::getRenderTarget() const
    {
        return _rt;
    }

    const VertexDeclaration*    VideoDriverNull::getVertexDeclaration(bvertex_format bf) const
    {
        return IVideoDriver::instance->getVertexDeclaration(bf);
    }

    void VideoDriverNull::setScissorRect(const Rect*)
    {

    }

    void VideoDriverNull::setDefaultSettings()
    {

    }
    void VideoDriverNull::setRenderTarget(spNativeTexture rt)
    {
        _rt = rt;
    }
    void VideoDriverNull::setShaderProgram(ShaderProgram*)
    {

    }
    void VideoDriverNull::setTexture(int, spNativeTexture)
    {

    }
}
