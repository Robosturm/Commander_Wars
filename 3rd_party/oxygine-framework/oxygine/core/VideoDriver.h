#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "ShaderProgram.h"
#include "texture.h"

#include <QMatrix4x4>

namespace oxygine
{
    class VideoDriver;
    using spVideoDriver = std::shared_ptr<VideoDriver>;

    class VideoDriver final
    {
    public:
        static constexpr const char* const getTypeName()
        {
            return "VideoDriver";
        }
        enum class PRIMITIVE_TYPE
        {
            POINTS,
            LINES,
            LINE_LOOP,
            LINE_STRIP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            COUNT,
        };

        enum class STATE
        {
            BLEND,
            NUM
        };

        enum class BLEND_MODE
        {
            NONE,
            ALPHA,
            COUNT,
        };

        struct Uniform3f
        {
            explicit Uniform3f()
                : data{0, 0, 0}
            {
            }
            explicit Uniform3f(float x, float y, float z)
                : data{x, y, z}
            {
            }
            GLfloat data[3];
        };
        struct Uniform4f
        {
            explicit Uniform4f()
                : data{0, 0, 0, 0}
            {
            }
            explicit Uniform4f(float x, float y, float z , float w)
                : data{x, y, z, w}
            {
            }
            GLfloat data[4];
        };

        static spVideoDriver instance;
        explicit VideoDriver();
        ~VideoDriver();
        void reset();
        void restore();
        bool isReady() const;
        spTexture createTexture();
        void clear(const QColor& color);
        void begin(const QRect& viewport, const QColor* color);
        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, qint32 primitives);
        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, const quint16* indicesData, quint32 numIndices);
        void getViewport(QRect& r) const;
        QRect getScissorRect(bool & active) const;
        spTexture getRenderTarget() const;
        ShaderProgram*  getShaderProgram() const
        {
            return m_pShaderProgram;
        }
        const VertexDeclaration* getVertexDeclaration() const;
        void setScissorRect(const QRect* rect);
        void setDefaultSettings();
        void setViewport(const QRectF& viewport);
        void setRenderTarget(spTexture &);
        void setShaderProgram(ShaderProgram*);
        void setTexture(qint32 sampler, spTexture &);
        void setState(STATE state, bool value);
        void setBlendFunc(BLEND_MODE func);
        void setUniform(const char* id, const Uniform4f* v, qint32 num);
        void setUniform(const char* id, const Uniform3f* v, qint32 num);
        void setUniform(const char* id, float v);
        void setUniformInt(const char* id, qint32 v);
        void setUniform(const char* id, const Uniform4f& v);
        void setUniform(const char* id, const Uniform3f& v);
        void setUniform(const char* id, const QMatrix4x4&  v);
    protected:
        quint32 getPT(VideoDriver::PRIMITIVE_TYPE pt);
        void _begin(const QRect& viewport, const QColor* clearColor);
    protected:
        spTexture m_rt;
        VertexDeclaration m_VertexDeclaration;
        qint32 m_programID{0};
        ShaderProgram* m_pShaderProgram{nullptr};
    };
}
