#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/MaskedRenderer.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/MaskedSprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

namespace oxygine
{
    spRenderDelegate RenderDelegate::instance;

    void RenderDelegate::render(Actor* parent, const RenderState& parentRS)
    {
        RenderState rs;
        if (!parent->internalRender(rs, parentRS))
        {
            return;
        }
        auto & children = parent->getChildren();
        for (auto & child : children)
        {
            child->render(rs);
        }
    }

    void RenderDelegate::render(ClipRectActor* actor, const RenderState& parentRS)
    {
        STDRenderer* renderer = STDRenderer::getCurrent();
        VideoDriver* driver = renderer->getDriver();

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
            RectF ss_rect = Actor::getActorTransformedDestRect(actor, actor->getTransform() * parentRS.transform);

            clippedRect.clip(ss_rect);
            if (!clippedRect.isEmpty())
            {
                Rect gl_rect = Rect(static_cast<qint32>(clippedRect.pos.x + 0.01f),
                                    static_cast<qint32>(clippedRect.pos.y + 0.01f),
                                    static_cast<qint32>(clippedRect.size.x + 0.01f),
                                    static_cast<qint32>(clippedRect.size.y + 0.01f));

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

    void RenderDelegate::render(MaskedSprite* sprite, const RenderState& parentRS)
    {
        Sprite* maskSprite = sprite->getMask();
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


        bool useR             = sprite->getUseRChannel();
        bool rchannel         = useR ? true    : (df.alpha ? true     : false);
        spTexture maskTexture = useR ? df.base : (df.alpha ? df.alpha : df.base);

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
            VideoDriver::instance->setUniform("clip_mask", clipMask);
            VideoDriver::instance->setUniform("msk", msk, 4);
        };

        renderer->pushShaderSetHook(&hook);
        renderer->setBaseShaderFlags(baseShaderFlags);

        sprite->Sprite::render(parentRS);

        Material::null->apply();

        renderer->popShaderSetHook();
        renderer->setBaseShaderFlags(sflags);
    }

    void RenderDelegate::doRender(Sprite* sprite, const RenderState& rs)
    {
        if (!sprite->getAnimFrame().getDiffuse().base)
        {
            return;
        }

        QColor color = rs.getFinalColor(sprite->getColor());

        sprite->getMaterial()->apply();
        sprite->getMaterial()->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
    }

    void RenderDelegate::doRender(TextField* tf, const RenderState& rs)
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

    void RenderDelegate::doRender(ColorRectSprite* sprite, const RenderState& rs)
    {
        sprite->getMaterial()->apply();
        QColor color = rs.getFinalColor(sprite->getColor());
        sprite->getMaterial()->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
    }
}
