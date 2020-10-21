#include "STDRenderDelegate.h"
#include "MaskedRenderer.h"
#include "RenderState.h"
#include "STDRenderer.h"
#include "VisualStyle.h"
#include "actor/ClipRectActor.h"
#include "actor/ColorRectSprite.h"
#include "actor/MaskedSprite.h"
#include "actor/Sprite.h"
#include "actor/TextField.h"
#include "core/NativeTexture.h"
#include "core/UberShaderProgram.h"
#include "core/oxygine.h"
#include "res/ResFont.h"
#include "text_utils/Node.h"

#include <qmath.h>
#include "core/gamewindow.h"

namespace oxygine
{
    STDRenderDelegate* STDRenderDelegate::instance = 0;

    void RenderDelegate::render(Actor* parent, const RenderState& parentRS)
    {
        spActor pParent = parent;
        if (pParent.get() != nullptr)
        {
            RenderState rs;
            if (!pParent->internalRender(rs, parentRS))
            {
                return;
            }
            // render fix changed actor to spActor  to avoid illegal deletion -> little bit slower but safer
            spActor actor = pParent->getFirstChild();
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
    }

    void STDRenderDelegate::render(ClipRectActor* actor, const RenderState& parentRS)
    {
        spClipRectActor pActor = actor;
        if (pActor.get() != nullptr)
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
            if (pActor->getClipping())
            {
                renderer->flush();

                RectF ss_rect = getActorTransformedDestRect(pActor.get(), pActor->getTransform() * parentRS.transform);

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
                pActor->Actor::render(rs);
            }


            if (pActor->getClipping())
            {
                renderer->flush();
                driver->setScissorRect(scissorEnabled ? &scissorRect : 0);
            }
        }
    }

    void STDRenderDelegate::render(MaskedSprite* sprite, const RenderState& parentRS)
    {
        spMaskedSprite pSprite = sprite;
        if (pSprite.get() != nullptr)
        {
            spSprite maskSprite = pSprite->getMask();
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


            bool useR           = pSprite->getUseRChannel();
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



            int sflags = renderer->getBaseShaderFlags();
            int baseShaderFlags = sflags;

            baseShaderFlags |= UberShaderProgram::MASK;
            if (rchannel)
                baseShaderFlags |= UberShaderProgram::MASK_R_CHANNEL;

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

            pSprite->Sprite::render(parentRS);

            Material::null->apply();

            renderer->popShaderSetHook();
            renderer->setBaseShaderFlags(sflags);
        }
    }

    void STDRenderDelegate::doRender(Sprite* sprite, const RenderState& rs)
    {
        spSprite pSprite = sprite;
        if (pSprite.get() != nullptr)
        {
            if (!pSprite->getAnimFrame().getDiffuse().base)
            {
                return;
            }

            QColor color = rs.getFinalColor(sprite->getColor());

            pSprite->_mat->apply();
            pSprite->_mat->render(rs.transform, color, pSprite->getAnimFrame().getSrcRect(), pSprite->getDestRect());
        }
    }

    void STDRenderDelegate::doRender(TextField* tf, const RenderState& rs)
    {
        spTextField pTf = tf;
        if (pTf.get() != nullptr)
        {
            float scale = qSqrt(rs.transform.a * rs.transform.a + rs.transform.c * rs.transform.c);

            text::Node* root = pTf->getRootNode(scale);
            if (!root)
            {
                return;
            }

            text::DrawContext dc;

            STDRenderer* renderer = STDRenderer::getCurrent();

            dc.primary = premultiply(rs.getFinalColor(tf->getColor()));
            dc.color = pTf->getStyle().color * dc.primary;

            //renderer->setBlendMode(tf->getBlendMode());
            renderer->setTransform(rs.transform);
            root->draw(dc);
        }
    }

    void STDRenderDelegate::doRender(ColorRectSprite* sprite, const RenderState& rs)
    {
        spColorRectSprite pSprite = sprite;
        if (pSprite.get() != nullptr)
        {
            pSprite->_mat->apply();
            QColor color = rs.getFinalColor(pSprite->getColor());
            pSprite->_mat->render(rs.transform, color, pSprite->getAnimFrame().getSrcRect(), pSprite->getDestRect());
        }
    }

    void STDRenderDelegate::doRender(ProgressBar*, const RenderState&)
    {

    }
}
