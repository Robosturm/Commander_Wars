#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

#include <QPainter>

namespace oxygine
{
    spRenderDelegate RenderDelegate::instance{nullptr};

    void RenderDelegate::render(Actor* parent, const RenderState& parentRS)
    {
#ifdef GRAPHICSUPPORT
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
#endif
    }

    void RenderDelegate::render(ClipRectActor* actor, const RenderState& parentRS)
    {
#ifdef GRAPHICSUPPORT
        STDRenderer* renderer = STDRenderer::getCurrent();
        VideoDriver* driver = renderer->getDriver();

        RenderState rs = parentRS;
        auto clippedRect = *parentRS.clip;
        rs.clip = &clippedRect;

        QRect scissorRect(0, 0, 0, 0);
        bool scissorEnabled = driver->getScissorRect(scissorRect);
        bool vis = true;
        if (actor->getClipping())
        {
            renderer->flush();
            QRect ss_rect = Actor::getActorTransformedDestRect(actor, actor->getTransform() * parentRS.transform);

            clippedRect = clippedRect.intersected(ss_rect);
            if (!clippedRect.isEmpty())
            {
                QRect gl_rect = clippedRect;
                if (!driver->getRenderTarget()->getHandle())
                {
                    GameWindow* window = oxygine::GameWindow::getWindow();
                    QSize size = window->size();
                    gl_rect.moveTop(size.height() - gl_rect.bottom());
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
            driver->setScissorRect(scissorEnabled ? &scissorRect : nullptr);
        }
#endif
    }

    void RenderDelegate::doRender(Sprite* sprite, const RenderState& rs)
    {
#ifdef GRAPHICSUPPORT
        if (!sprite->getAnimFrame().getTexture())
        {
            return;
        }
        QColor color = rs.getFinalColor(sprite->getColor());
        sprite->getMaterial()->apply();
        sprite->getMaterial()->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
#endif
    }

    void RenderDelegate::doRender(TextField* tf, const RenderState& rs)
    {
#ifdef GRAPHICSUPPORT
        text::Node* root = tf->getRootNode();
        if (!root)
        {
            return;
        }

        STDRenderer* renderer = STDRenderer::getCurrent();
        VideoDriver* driver = renderer->getDriver();
        QRect scissorRect(0, 0, 0, 0);
        bool scissorEnabled = driver->getScissorRect(scissorRect);

        spMaterial cur = Material::current;
        Material::null->apply();
        {
            //---------------------------------------------------------
            // qt painter usage
            GameWindow* window = oxygine::GameWindow::getWindow();
            QPainter painter(window);
            if (scissorEnabled)
            {
                QSize size = window->size();
                QRect clipRect(scissorRect.x(), size.height() - scissorRect.y() - scissorRect.height(), scissorRect.width(), scissorRect.height());
                painter.setClipRect(clipRect);
            }
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, tf->getFont()->antialiasing);
            root->draw(rs, tf->getStyle(), tf->getStyle().color, painter);
            painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, false);
            painter.end();
            //---------------------------------------------------------
        }
        rsCache().restoreAfterPainterUse();
        cur->apply();

        if (scissorEnabled)
        {
            driver->setScissorRect(&scissorRect);
        }
#endif
    }

    void RenderDelegate::doRender(ColorRectSprite* sprite, const RenderState& rs)
    {
#ifdef GRAPHICSUPPORT
        QColor color = rs.getFinalColor(sprite->getColor());
        sprite->getMaterial()->apply();
        sprite->getMaterial()->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
#endif
    }
}
