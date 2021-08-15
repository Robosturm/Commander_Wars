#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/ref_counter.h"
#include "3rd_party/oxygine-framework/oxygine/core/vertex.h"
#include "3rd_party/oxygine-framework/oxygine/math/Matrix.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/math/Vector4.h"
#include "3rd_party/oxygine-framework/oxygine/core/VertexDeclaration.h"
#include "3rd_party/oxygine-framework/oxygine/core/Restorable.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
#include <qopengl.h>

namespace oxygine
{
    class VideoDriver;
    using spVideoDriver = oxygine::intrusive_ptr<VideoDriver>;

    class VideoDriver : public oxygine::ref_counter
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

        class Stats
        {
        public:
            explicit Stats() : batches(0), start(0), duration(0) { memset(elements, 0, sizeof(elements)); }
            virtual  ~Stats() = default;
            qint32 batches;
            qint32 elements[PT_COUNT];
            timeMS start;
            timeMS duration;
        };
        static Stats m_stats;

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
            STATE_CULL_FACE,
            STATE_NUM
        };

        static spVideoDriver instance;
        explicit VideoDriver();
        virtual ~VideoDriver();
        void reset();
        void restore();
        bool isReady() const;
        spTexture createTexture();
        void clear(const QColor& color);
        void begin(const Rect& viewport, const QColor* color);
        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, GLsizei primitives);
        void draw(PRIMITIVE_TYPE pt, const VertexDeclaration* decl, const VertexPCT2* verticesData, const quint16* indicesData, quint32 numIndices);
        void getViewport(Rect& r) const;
        bool getScissorRect(Rect&) const;
        spTexture getRenderTarget() const;
        ShaderProgram*  getShaderProgram() const
        {
            return m_pShaderProgram;
        }
        const VertexDeclaration* getVertexDeclaration() const;
        void setScissorRect(const Rect*);
        void setDefaultSettings();
        void setViewport(const Rect& viewport);
        void setRenderTarget(spTexture);
        void setShaderProgram(ShaderProgram*);
        void setTexture(qint32 sampler, spTexture);
        void setState(STATE, quint32 value);
        void setBlendFunc(BLEND_TYPE src, BLEND_TYPE dest);
        void setUniform(const char* id, const Vector4* v, qint32 num);
        void setUniform(const char* id, const Vector3* v, qint32 num);
        void setUniform(const char* id, const Vector2* v, qint32 num);
        void setUniform(const char* id, const Matrix* v, qint32 num);
        void setUniform(const char* id, float v);
        void setUniformInt(const char* id, qint32 v);
        void setUniform(const char* id, const Vector4& v);
        void setUniform(const char* id, const Vector3& v);
        void setUniform(const char* id, const Vector2& v);
        void setUniform(const char* id, const Matrix&  v);
    protected:
        quint32 getPT(VideoDriver::PRIMITIVE_TYPE pt);
        quint32 getBT(VideoDriver::BLEND_TYPE pt);
        void _begin(const Rect& viewport, const QColor* clearColor);
    protected:
        spTexture m_rt;
        static VertexDeclaration m_VertexDeclaration;
        qint32 m_programID{0};
        ShaderProgram* m_pShaderProgram{nullptr};
    };
}
