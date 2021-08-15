#pragma once
#include "3rd_party/oxygine-framework/oxygine/oxygine-forwards.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/core/Texture.h"
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
        explicit PostProcessOptions(qint32 flags = 0)
            : m_flags(flags),
              m_downscale(1),
              m_clearColor(0, 0, 0, 0)
        {
        }
        PostProcessOptions& fullscreen(bool enable = true)
        {
            m_flags = enable ? (m_flags | flag_fullscreen) : (m_flags  & (~flag_fullscreen));
            return *this;
        }
        PostProcessOptions& singleRender(bool enable = true)
        {
            m_flags = enable ? (m_flags | flag_singleR2T) : (m_flags  & (~flag_singleR2T));
            return *this;
        }
        //loops -(2, 3, 4, ...),  final size: 2^loops
        PostProcessOptions& downscale(qint32 loops = 2)
        {
            m_downscale = loops;
            return *this;
        }
        PostProcessOptions& clear(const QColor& c)
        {
            m_clearColor = c;
            return *this;
        }
        PostProcessOptions& fixedBounds(const RectF& b)
        {
            m_fixedBounds = b; m_flags |= flag_fixedBounds;
            return *this;
        }
        qint32 m_flags;
        qint32 m_downscale;
        RectF m_fixedBounds;
        QColor m_clearColor;
    };

    class PPTask
    {
    public:
        explicit PPTask() = default;
        virtual ~PPTask() = default;
        virtual void addRefPP() = 0;
        virtual void releaseRefPP() = 0;
        virtual void renderPP() = 0;
    };

    class RenderTargetsManager
    {
    public:
        explicit RenderTargetsManager() = default;
        virtual ~RenderTargetsManager() = default;
        spTexture get(spTexture current, qint32 w, qint32 h, ImageData::TextureFormat tf);
        void update();
        void reset();
    protected:
        bool isGood(const spTexture& t, qint32 w, qint32 h, ImageData::TextureFormat tf) const;

        using rts = QVector<spTexture>;
        rts m_rts;
        using free = QVector<spTexture>;
        free m_free;
    };

    class PostProcess
    {
    public:

        static spShaderProgram shaderBlurV;
        static spShaderProgram shaderBlurH;
        static spShaderProgram shaderBlit;

        static void initShaders();
        static void freeShaders();
        static void updatePortProcessItems();
        static void addPostProcessItem(PPTask*);
        static void removePostProcessItem(PPTask*);
        static void clearPostProcessItems();
        static RenderTargetsManager& getRTManager();
        static void pass(spTexture srcTexture, const Rect& srcRect, spTexture destTexture, const Rect& destRect, const QColor& color = Qt::white);

        explicit PostProcess(const PostProcessOptions& opt);
        virtual ~PostProcess() = default;

        void update(Actor* actor);
        Rect getScreenRect(const Actor& actor) const;

        Point m_extend;
        spTexture m_rt;
        ImageData::TextureFormat m_format;
        Transform m_transform;
        Rect m_screen;

        PostProcessOptions m_options;
    private:
        static QVector<PPTask*> m_postProcessItems;
        static RenderTargetsManager m_rtm;
    };

    class TweenPostProcess;
    using spTweenPostProcess = intrusive_ptr<TweenPostProcess>;
    class TweenPostProcess : public TweenBase, public RenderDelegate, public PPTask
    {
    public:
        explicit TweenPostProcess(const PostProcessOptions& opt);
        virtual ~TweenPostProcess();

        virtual void renderPP() override;
        virtual void addRefPP() override;
        virtual void releaseRefPP() override;
        virtual void init(Actor& actor) override;
        virtual void update(Actor& actor, float p, const UpdateState& us) override;
        virtual void done(Actor& actor) override;
    protected:
        virtual void _renderPP() {}
    protected:
        Actor* m_actor;
        spActor m_pActorRef;
        PostProcess m_pp;
        float m_progress;

        RenderDelegate* m_prevMaterial;
    };
}
