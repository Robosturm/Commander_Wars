#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/MaskedRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/VisualStyle.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/MaskedSprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/core/NativeTexture.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFont.h"
#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

#include <qmath.h>

namespace oxygine
{
    spSTDRenderDelegate STDRenderDelegate::instance = nullptr;

    void RenderDelegate::render(Actor* parent, const RenderState& parentRS)
    {
        RenderState rs;
        if (!parent->internalRender(rs, parentRS))
        {
            return;
        }
        spActor actor = parent->getFirstChild();
        while (actor.get() != nullptr)
        {
            if (actor->getParent())
            {
                actor->render(rs);
                actor = actor->getNextSibling().get();
            }
            else
            {
                break;
            }
        }
    }

    void STDRenderDelegate::render(ClipRectActor* actor, const RenderState& parentRS)
    {
        STDRenderer* renderer = STDRenderer::getCurrent();
        IVideoDriver* driver = renderer->getDriver();

        RenderState rs = parentRS;

        const RectF* parentClippedRect = parentRS.clip;
        RectF clippedRect = *parentClippedRect;
        rs.clip = &clippedRect;

        Rect scissorRect(0, 0, 0, 0);
        bool scissorEnabled = driver->getScissorRect(scissorRect);
        bool vis = true;
        if (actor->getClipping())
        {
            renderer->flush();
            RectF ss_rect = getActorTransformedDestRect(actor, actor->getTransform() * parentRS.transform);

            clippedRect.clip(ss_rect);
            if (!clippedRect.isEmpty())
            {
                Rect gl_rect = Rect(
                                   int(clippedRect.pos.x + 0.01f),
                                   int(clippedRect.pos.y + 0.01f),
                                   int(clippedRect.size.x + 0.01f),
                                   int(clippedRect.size.y + 0.01f));

                if (!driver->getRenderTarget()->getHandle())
                {
                    GameWindow* window = oxygine::GameWindow::getWindow();
                    QSize size = window->size();
                    gl_rect.pos.y = size.height() - gl_rect.getBottom();
                }

                driver->setScissorRect(&gl_rect);
            }
            else
            {
                vis = false;
            }
        }

        if (vis)
        {
            actor->Actor::render(rs);
        }


        if (actor->getClipping())
        {
            renderer->flush();
            driver->setScissorRect(scissorEnabled ? &scissorRect : 0);
        }
    }

    void STDRenderDelegate::render(MaskedSprite* sprite, const RenderState& parentRS)
    {
        spSprite maskSprite = sprite->getMask();
        if (!maskSprite)
        {
            sprite->Sprite::render(parentRS);
            return;
        }

        const Diffuse& df = maskSprite->getAnimFrame().getDiffuse();
        if (!df.base)
        {
            sprite->Sprite::render(parentRS);
            return;
        }


        Material::null->apply();

        Transform world = maskSprite->computeGlobalTransform();

        RectF maskDest = maskSprite->getDestRect();
        RectF maskSrc = maskSprite->getSrcRect();


        bool useR           = sprite->getUseRChannel();
        bool rchannel               = useR ? true    : (df.alpha ? true     : false);
        spNativeTexture maskTexture = useR ? df.base : (df.alpha ? df.alpha : df.base);

        STDRenderer* renderer = STDRenderer::getCurrent();

        ClipUV clipUV = ClipUV(
                            world.transform(maskDest.getLeftTop()),
                            world.transform(maskDest.getRightTop()),
                            world.transform(maskDest.getLeftBottom()),
                            maskSrc.getLeftTop(),
                            maskSrc.getRightTop(),
                            maskSrc.getLeftBottom());

        Vector2 v(1.0f / maskTexture->getWidth(), 1.0f / maskTexture->getHeight());
        maskSrc.expand(v, v);



        qint32 sflags = renderer->getBaseShaderFlags();
        qint32 baseShaderFlags = sflags;

        baseShaderFlags |= UberShaderProgram::MASK;
        if (rchannel)
        {
            baseShaderFlags |= UberShaderProgram::MASK_R_CHANNEL;
        }
        Vector3 msk[4];

        clipUV.get(msk);
        Vector4 clipMask = Vector4(maskSrc.getLeft(), maskSrc.getTop(), maskSrc.getRight(), maskSrc.getBottom());

        rsCache().setTexture(UberShaderProgram::SAMPLER_MASK, maskTexture);



        ShaderProgramChangedHook hook;
        hook.hook = [&]()
        {

            IVideoDriver::instance->setUniform("clip_mask", clipMask);
            IVideoDriver::instance->setUniform("msk", msk, 4);

        };


        renderer->pushShaderSetHook(&hook);
        renderer->setBaseShaderFlags(baseShaderFlags);

        sprite->Sprite::render(parentRS);

        Material::null->apply();

        renderer->popShaderSetHook();
        renderer->setBaseShaderFlags(sflags);
    }

    void STDRenderDelegate::doRender(Sprite* sprite, const RenderState& rs)
    {
        if (!sprite->getAnimFrame().getDiffuse().base)
        {
            return;
        }

        QColor color = rs.getFinalColor(sprite->getColor());

        sprite->m_mat->apply();
        sprite->m_mat->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
    }

    void STDRenderDelegate::doRender(TextField* tf, const RenderState& rs)
    {
        text::Node* root = tf->getRootNode();
        if (!root)
        {
            return;
        }
        text::DrawContext dc;
        STDRenderer* renderer = STDRenderer::getCurrent();
        dc.m_primary = premultiply(rs.getFinalColor(tf->getColor()));
        dc.m_color = tf->getStyle().color * dc.m_primary;
        renderer->setTransform(rs.transform);
        root->draw(dc);
    }

    void STDRenderDelegate::doRender(ColorRectSprite* sprite, const RenderState& rs)
    {
        sprite->m_mat->apply();
        QColor color = rs.getFinalColor(sprite->getColor());
        sprite->m_mat->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
    }

    void STDRenderDelegate::doRender(ProgressBar*, const RenderState&)
    {

    }
}
