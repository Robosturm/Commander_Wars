#pragma once

#include <QTransform>
#include <QRect>

#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/VideoDriver.h"
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
        const QTransform& getTransform() const
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
        void setTransform(const QTransform& world);
        void addQuad(const QColor& clr, const QRectF& srcRect, const QRect& destRect);

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

        static inline void fillQuad(std::vector<VertexPCT2> & quad, const QRectF& srcRect, const QRect& destRect, const QTransform& transform, quint32 rgba)
        {
            float u = srcRect.x();
            float v = srcRect.y();
            float du = srcRect.width();
            float dv = srcRect.height();
            VertexPCT2 vt;
            vt.color = rgba;
            QPoint p1(destRect.topLeft());
            QPoint p2(destRect.bottomLeft());
            QPoint p3(destRect.topRight());
            QPoint p4(destRect.bottomRight());

            p1 = transform.map(p1);
            p2 = transform.map(p2);
            p3 = transform.map(p3);
            p4 = transform.map(p4);

            vt.z = 0;
            vt.x = p1.x();
            vt.y = p1.y();
            vt.u = u;
            vt.v = v;
            quad[0] = vt;

            vt.x = p2.x();
            vt.y = p2.y();
            vt.u = u;
            vt.v = v + dv;
            quad[1] = vt;

            vt.x = p3.x();
            vt.y = p3.y();
            vt.u = u + du;
            vt.v = v;
            quad[2] = vt;

            vt.x = p4.x();
            vt.y = p4.y();
            vt.u = u + du;
            vt.v = v + dv;
            quad[3] = vt;
        }
    protected:
        virtual void shaderProgramChanged() {}
        void setShader(ShaderProgram* prog);
        void checkDrawBatch();

    protected:
        QTransform m_transform;
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
