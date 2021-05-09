#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-include.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/Renderer.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/tween/Tween.h"
#include "3rd_party/oxygine-framework/oxygine/core/ShaderProgram.h"

namespace oxygine
{
    class PostProcessOptions
    {
    public:
        enum
        {
            flag_singleR2T = 1,
            flag_fullscreen = 1 << 1,
            flag_screen = 1 << 2,
            flag_fixedBounds = 1 << 3,
        };

        PostProcessOptions(qint32 flags = 0) : _flags(flags), _downscale(1), _clearColor(0, 0, 0, 0) {}
        PostProcessOptions& fullscreen(bool enable = true) { _flags = enable ? (_flags | flag_fullscreen) : (_flags  & (~flag_fullscreen)); return *this; }
        PostProcessOptions& singleRender(bool enable = true) { _flags = enable ? (_flags | flag_singleR2T) : (_flags  & (~flag_singleR2T)); return *this; }
        //loops -(2, 3, 4, ...),  final size: 2^loops
        PostProcessOptions& downscale(qint32 loops = 2) { _downscale = loops; return *this; }
        PostProcessOptions& clear(const QColor& c) { _clearColor = c; return *this; }
        PostProcessOptions& fixedBounds(const RectF& b) { _fixedBounds = b; _flags |= flag_fixedBounds; return *this; }

        qint32 _flags;
        qint32 _downscale;
        RectF _fixedBounds;
        QColor _clearColor;
    };


    class PostProcess
    {
    public:

        static spShaderProgram shaderBlurV;
        static spShaderProgram shaderBlurH;
        static spShaderProgram shaderBlit;

        static void initShaders();
        static void freeShaders();

        PostProcess(const PostProcessOptions& opt);
        ~PostProcess();

        void free();

        void update(Actor* actor);
        Rect getScreenRect(const Actor& actor) const;

        Point _extend;
        spNativeTexture _rt;
        ImageData::TextureFormat _format;
        Transform _transform;
        Rect _screen;

        PostProcessOptions _options;
    };

    class PPTask
    {
    public:
        virtual ~PPTask() {}
        virtual void addRefPP() = 0;
        virtual void releaseRefPP() = 0;
        virtual void renderPP() = 0;
    };

    DECLARE_SMART(TweenPostProcess, spTweenPostProcess);

    class TweenPostProcess : public TweenBase, public RenderDelegate, public PPTask
    {
    public:
        TweenPostProcess(const PostProcessOptions& opt);
        ~TweenPostProcess();

        void renderPP() override;
        void addRefPP() override;
        void releaseRefPP() override;

        void init(Actor& actor) override;
        void update(Actor& actor, float p, const UpdateState& us) override;
        void done(Actor& actor) override;
    protected:
        virtual void _renderPP() {}


        Actor* _actor;
        spActor m_pActorRef;
        PostProcess _pp;
        float _progress;

        RenderDelegate* _prevMaterial;
    };


    class RenderTargetsManager
    {
    public:
        RenderTargetsManager();

        spNativeTexture get(spNativeTexture current, qint32 w, qint32 h, ImageData::TextureFormat tf);
        void update();
        void reset();

    protected:
        bool isGood(const spNativeTexture& t, qint32 w, qint32 h, ImageData::TextureFormat tf) const;

        typedef QVector<spNativeTexture> rts;
        rts _rts;

        typedef QVector<spNativeTexture> free;
        free _free;
    };

    void pass(spNativeTexture srcTexture, const Rect& srcRect, spNativeTexture destTexture, const Rect& destRect, const QColor& color = Qt::white);

    RenderTargetsManager& getRTManager();

    void updatePortProcessItems();
    bool isRenderingPostProcessItems();
    void addPostProcessItem(PPTask*);
    void removePostProcessItem(PPTask*);
    void clearPostProcessItems();
}
