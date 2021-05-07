#pragma once
#include "3rd_party/oxygine-framework/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGL.h"
#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

#include <qopengl.h>

namespace oxygine
{
    class VideoDriverGLES20;
    DECLARE_SMART(VideoDriverGLES20, spVideoDriverGLES20);

    class VideoDriverGLES20: public VideoDriverGL
    {
    public:
        VideoDriverGLES20();
        ~VideoDriverGLES20();

        void reset() override;
        void restore() override;

        spNativeTexture createTexture() override;

        bool isReady() const  override;

        void begin(const Rect& viewport, const QColor* clearColor) override;
        void clear(const QColor& color) override;

        ShaderProgram*  getShaderProgram() const override { return m_p; }

        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const void* verticesData, quint32 verticesDataSize) override;
        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const void* verticesData, quint32 verticesDataSize, const unsigned short* indicesData, quint32 numIndices) override;
        void setDefaultSettings() override;

        void setViewport(const Rect& viewport) override;
        void setShaderProgram(ShaderProgram*) override;
        void setTexture(qint32 sampler, spNativeTexture) override;
        void setUniformInt(const char* id, qint32 v) override;
        void setUniform(const char* id, const Vector4* v, qint32 num) override;
        void setUniform(const char* id, const Vector3* v, qint32 num) override;
        void setUniform(const char* id, const Vector2* v, qint32 num) override;
        void setUniform(const char* id, const Matrix* mat, qint32 num) override;
        void setUniform(const char* id, float val) override;

    protected:
        qint32 m_programID;
        ShaderProgram* m_p;
    };
}
