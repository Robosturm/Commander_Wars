#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{
    spIVideoDriver IVideoDriver::instance;

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

    IVideoDriver::Stats IVideoDriver::m_stats;
}
