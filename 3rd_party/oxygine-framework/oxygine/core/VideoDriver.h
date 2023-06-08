#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/intrusive_ptr.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "ShaderProgram.h"
#include "texture.h"

#include <QMatrix4x4>

namespace oxygine
{
    class VideoDriver;
    using spVideoDriver = oxygine::intrusive_ptr<VideoDriver>;

    class VideoDriver final : public oxygine::ref_counter
    {
    public:

        enum PRIMITIVE_TYPE
        {
            PT_POINTS,
            PT_LINES,
            PT_LINE_LOOP,
            PT_LINE_STRIP,
            PT_TRIANGLES,
            PT_TRIANGLE_STRIP,
            PT_TRIANGLE_FAN,
            PT_COUNT,
        };

        enum BLEND_TYPE
        {
            BT_ZERO,
            BT_ONE,
            BT_SRC_COLOR,
            BT_ONE_MINUS_SRC_COLOR,
            BT_SRC_ALPHA,
            BT_ONE_MINUS_SRC_ALPHA,
            BT_DST_ALPHA,
            BT_DST_COLOR,
            BT_ONE_MINUS_DST_ALPHA,
            BT_ONE_MINUS_DST_COLOR,
        };

        enum CULL_FACE_TYPE
        {
            CULL_FACE_FRONT_AND_BACK,
            CULL_FACE_FRONT,
            CULL_FACE_BACK,
        };

        enum STATE
        {
            STATE_BLEND,
            STATE_NUM
        };

        enum blend_mode
        {
            blend_disabled              = 0,
            blend_premultiplied_alpha   = ((BT_ONE                 << 16) | BT_ONE_MINUS_SRC_ALPHA),
            blend_alpha                 = ((BT_SRC_ALPHA           << 16) | BT_ONE_MINUS_SRC_ALPHA),
            blend_add                   = ((BT_ONE                 << 16) | BT_ONE),
            blend_screen                = ((BT_ONE                 << 16) | BT_ONE_MINUS_SRC_COLOR),
            blend_multiply              = ((BT_DST_COLOR           << 16) | BT_ONE_MINUS_SRC_ALPHA),
            blend_inverse               = ((BT_ONE_MINUS_DST_COLOR << 16) | BT_ZERO),
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
       virtual ~VideoDriver();
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
        void setState(STATE, quint32 value);
        void setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest);
        void setUniform(const char* id, const Uniform4f* v, qint32 num);
        void setUniform(const char* id, const Uniform3f* v, qint32 num);
        void setUniform(const char* id, float v);
        void setUniformInt(const char* id, qint32 v);
        void setUniform(const char* id, const Uniform4f& v);
        void setUniform(const char* id, const Uniform3f& v);
        void setUniform(const char* id, const QMatrix4x4&  v);
    protected:
        quint32 getPT(VideoDriver::PRIMITIVE_TYPE pt);
        quint32 getBT(VideoDriver::BLEND_TYPE pt);
        void _begin(const QRect& viewport, const QColor* clearColor);
    protected:
        spTexture m_rt;
        VertexDeclaration m_VertexDeclaration;
        qint32 m_programID{0};
        ShaderProgram* m_pShaderProgram{nullptr};
    };
}
