#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/math/AffineTransform.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
#include "coreengine/globalutils.h"
#include <functional>
#include <vector>

namespace oxygine
{
    class ShaderProgramChangedHook
    {
    public:
        ShaderProgramChangedHook()
            : prev(0),
              next(0)
        {
        }
        ShaderProgramChangedHook* prev;
        ShaderProgramChangedHook* next;
        std::function<void()> hook;
    };    

    class STDRenderer;
    using spSTDRenderer = oxygine::intrusive_ptr<STDRenderer>;
    class STDRenderer final : public ShaderProgramChangedHook, public oxygine::ref_counter
    {
    public:
        static spSTDRenderer current;
        static spSTDRenderer instance;
        /**Initializes internal classes. Called automatically from oxygine::init();*/
        static void initialize();
        /**Clears internal data*/
        static void release();
        /**for lost context*/
        static void reset();
        /**is Renderer was restored and ready to be used*/
        static bool isReady();
        /**restore after lost context*/
        static void restore();

        /**returns activated renderer with STDRenderer::begin*/
        static STDRenderer* getCurrent();

        /**White 4x4 Texture*/
        static spTexture white;
        static std::vector<quint16> indices16;
        static size_t maxVertices;

        explicit STDRenderer(VideoDriver* driver = nullptr);
        virtual ~STDRenderer();

        const QMatrix4x4& getViewProjection() const;
        VideoDriver* getDriver();
        const AffineTransform& getTransform() const
        {
            return m_transform;
        }
        const VertexDeclaration* getVertexDeclaration() const
        {
            return m_vdecl;
        }

        void setFracShader(UberShaderProgram::ColorMode fracShader);
        void setViewProj(const QMatrix4x4& viewProj);
        void setVertexDeclaration(const VertexDeclaration* decl);
        void setBaseShaderFlags(quint32 fl);
        /**Sets World transformation.*/
        void setTransform(const AffineTransform& world);
        void addQuad(const QColor&, const RectF& srcRect, const RectF& destRect);

        /**Completes started rendering and restores previous Frame Buffer.*/
        void end();

        /**Draws existing batch immediately.*/
        void flush();
        void addVertices(const std::vector<VertexPCT2> & data);

        void pushShaderSetHook(ShaderProgramChangedHook* hook);
        void popShaderSetHook();

        bool isEmpty() const
        {
            return m_verticesData.empty();
        }

        static inline void fillQuad(std::vector<VertexPCT2> & quad, const RectF& srcRect, const RectF& destRect, const AffineTransform& transform, quint32 rgba)
        {
            float u = srcRect.pos.x;
            float v = srcRect.pos.y;
            float du = srcRect.size.x;
            float dv = srcRect.size.y;
            VertexPCT2 vt;
            vt.color = rgba;
            const Vector2& pos = destRect.pos;
            const Vector2& size = destRect.size;

            Vector2 p1(pos.x, pos.y);
            Vector2 p2(pos.x, pos.y + size.y);
            Vector2 p3(pos.x + size.x, pos.y);
            Vector2 p4(pos.x + size.x, pos.y + size.y);

            p1 = transform.transform(p1);
            p2 = transform.transform(p2);
            p3 = transform.transform(p3);
            p4 = transform.transform(p4);

            vt.z = 0;
            vt.x = static_cast<qint32>(p1.x);
            vt.y = static_cast<qint32>(p1.y);
            vt.u = u;
            vt.v = v;
            quad[0] = vt;

            vt.x = static_cast<qint32>(p2.x);
            vt.y = GlobalUtils::roundUp(p2.y);
            vt.u = u;
            vt.v = v + dv;
            quad[1] = vt;

            vt.x = GlobalUtils::roundUp(p3.x);
            vt.y = static_cast<qint32>(p3.y);
            vt.u = u + du;
            vt.v = v;
            quad[2] = vt;

            vt.x = GlobalUtils::roundUp(p4.x);
            vt.y = GlobalUtils::roundUp(p4.y);
            vt.u = u + du;
            vt.v = v + dv;
            quad[3] = vt;
        }
    protected:
        virtual void shaderProgramChanged() {}
        void setShader(ShaderProgram* prog);
        void checkDrawBatch();

    protected:
        AffineTransform m_transform;
        std::vector<VertexPCT2> m_verticesData;
        const VertexDeclaration* m_vdecl{nullptr};
        VideoDriver* m_driver;
        QMatrix4x4 m_vp;
        ShaderProgramChangedHook* m_sphookFirst;
        ShaderProgramChangedHook* m_sphookLast;

        spTexture m_prevRT;
    private:
        static bool m_restored;
        static QScopedPointer<UberShaderProgram> m_uberShader;
    };

    class RenderStateCache
    {
    public:
        RenderStateCache();

        void setDriver(VideoDriver* d);

        const spTexture& getTexture(qint32 sampler)
        {
            return m_textures[sampler];
        }

        void setTexture(qint32 sampler, spTexture& t);
        void setBlendMode(VideoDriver::blend_mode blend);
        bool setShader(ShaderProgram* prog);

        void reset();
        void resetTextures();
        void restoreAfterPainterUse();
    protected:
        void setBlendModeInternal();

    protected:
        static constexpr quint32 MAX_TEXTURES = 8;
        spTexture m_textures[MAX_TEXTURES];
        ShaderProgram* m_program{nullptr};
        VideoDriver* m_driver{nullptr};
        VideoDriver::blend_mode m_blend{VideoDriver::blend_disabled};
    };

    RenderStateCache& rsCache();

}
