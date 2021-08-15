#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include <functional>
#include <vector>

namespace oxygine
{
    class RenderStateCache
    {
    public:
        RenderStateCache();

        void setDriver(VideoDriver* d);

        const spNativeTexture& getTexture(qint32 sampler)
        {
            return _textures[sampler];
        }

        void setTexture(qint32 sampler, const spNativeTexture& t);
        void setBlendMode(blend_mode blend);
        bool setShader(ShaderProgram* prog);

        void reset();
        void resetTextures();

    protected:
        enum { MAX_TEXTURES = 8 };

        spNativeTexture _textures[MAX_TEXTURES];
        ShaderProgram*  m_program;
        VideoDriver*   m_driver;
        blend_mode      m_blend;
    };

    RenderStateCache& rsCache();

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
        std::function< void() > hook;
    };    

    class STDRenderer;
    typedef oxygine::intrusive_ptr<STDRenderer> spSTDRenderer;
    class STDRenderer : public ShaderProgramChangedHook, public oxygine::ref_counter
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
        static spNativeTexture white;
        static spNativeTexture invisible;

        static UberShaderProgram uberShader;
        static QString fracShaderBody;
        static QString fracTableShaderBody;
        static QString vertexShaderBody;
        static std::vector<quint16> indices16;
        static size_t maxVertices;

        explicit STDRenderer(VideoDriver* driver = 0);
        virtual ~STDRenderer();

        const Matrix& getViewProjection() const;
        VideoDriver* getDriver();
        const AffineTransform& getTransform() const
        {
            return m_transform;
        }
        const VertexDeclaration* getVertexDeclaration() const
        {
            return m_vdecl;
        }
        quint32 getBaseShaderFlags() const
        {
            return m_baseShaderFlags;
        }

        void setShaderFlags(unsigned int);
        void setViewProj(const Matrix& viewProj);
        void setVertexDeclaration(const VertexDeclaration* decl);
        void setUberShaderProgram(UberShaderProgram* pr);
        void setBaseShaderFlags(quint32 fl);

        /**Sets World transformation.*/
        void setTransform(const Transform& world);
        void addQuad(const QColor&, const RectF& srcRect, const RectF& destRect);

        /**Begins rendering into RenderTexture or into primary framebuffer if rt is null*/
        void begin();
        void begin(spNativeTexture renderTarget, const Rect* viewport = 0);
        /**Completes started rendering and restores previous Frame Buffer.*/
        void end();
        /**initializes View + Projection matrices where TopLeft is (0,0) and RightBottom is (width, height). use flipU = true for render to texture*/
        void initCoordinateSystem(qint32 width, qint32 height, bool flipU = false);

        /**Draws existing batch immediately.*/
        void flush();
        void addVertices(std::vector<VertexPCT2> & data);

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
            vt.x = p1.x;
            vt.y = p1.y;
            vt.u = u;
            vt.v = v;
            quad[0] = vt;

            vt.x = p2.x;
            vt.y = p2.y;
            vt.u = u;
            vt.v = v + dv;
            quad[1] = vt;

            vt.x = p3.x;
            vt.y = p3.y;
            vt.u = u + du;
            vt.v = v;
            quad[2] = vt;

            vt.x = p4.x;
            vt.y = p4.y;
            vt.u = u + du;
            vt.v = v + dv;
            quad[3] = vt;
        }
        /**Returns View matrix where Left Top corner is (0,0), and right bottom is (w,h)*/
        static Matrix makeViewMatrix(qint32 w, qint32 h, bool flipU = false);
    protected:
        virtual void shaderProgramChanged() {}
        void setShader(ShaderProgram* prog);
        void checkDrawBatch();

    protected:
        Transform m_transform;
        std::vector<VertexPCT2> m_verticesData;
        const VertexDeclaration* m_vdecl;
        VideoDriver* m_driver;
        Matrix m_vp;
        ShaderProgramChangedHook* m_sphookFirst;
        ShaderProgramChangedHook* m_sphookLast;

        UberShaderProgram* m_uberShader;
        quint32 m_baseShaderFlags;
        spNativeTexture m_prevRT;
    };
}
