#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
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

        void setDriver(IVideoDriver* d);

        const spNativeTexture& getTexture(qint32 sampler) { return _textures[sampler]; }

        void setTexture(qint32 sampler, const spNativeTexture& t);
        void setBlendMode(blend_mode blend);
        bool setShader(ShaderProgram* prog);

        void reset();
        void resetTextures();

        void changeDriver(IVideoDriver* d);//used for DebugActor

    protected:
        enum { MAX_TEXTURES = 8 };

        spNativeTexture _textures[MAX_TEXTURES];
        ShaderProgram*  _program;
        IVideoDriver*   _driver;
        blend_mode      _blend;
    };

    RenderStateCache& rsCache();

    class ShaderProgramChangedHook
    {
    public:
        ShaderProgramChangedHook(): prev(0), next(0) {}

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
        static std::vector<unsigned short> indices16;
        static size_t maxVertices;



        STDRenderer(IVideoDriver* driver = 0);
        virtual ~STDRenderer();

        const Matrix&               getViewProjection() const;
        IVideoDriver*               getDriver();
        const AffineTransform&      getTransform() const { return m_transform; }
        const VertexDeclaration*    getVertexDeclaration() const { return m_vdecl; }
        quint32                     getBaseShaderFlags() const { return m_baseShaderFlags; }

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

        virtual void addVertices(const void* data, quint32 size);

        void swapVerticesData(std::vector<unsigned char>& data);
        void swapVerticesData(STDRenderer& r);

        void pushShaderSetHook(ShaderProgramChangedHook* hook);
        void popShaderSetHook();

        bool isEmpty() const { return m_verticesData.empty(); }

    protected:
        virtual void shaderProgramChanged() {}
        virtual void xbegin();
        void setShader(ShaderProgram* prog);
        void xdrawBatch();
        void xaddVertices(const void* data, quint32 size);
        void checkDrawBatch();

    protected:
        Transform m_transform;
        std::vector<unsigned char> m_verticesData;
        const VertexDeclaration* m_vdecl;
        IVideoDriver* m_driver;
        Matrix m_vp;
        ShaderProgramChangedHook* m_sphookFirst;
        ShaderProgramChangedHook* m_sphookLast;

        UberShaderProgram* m_uberShader;
        quint32 m_baseShaderFlags;
        spNativeTexture m_prevRT;
    };


    typedef void(*render_texture_hook)(const spNativeTexture& nt);
    void set_render_texture_hook(render_texture_hook);
    render_texture_hook get_render_texture_hook();
}
