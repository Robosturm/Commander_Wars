#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/MaskedRenderer.h"

#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/UberShaderProgram.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include "3rd_party/oxygine-framework/oxygine/text_utils/Node.h"

#include <QPainter>

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
            driver->setScissorRect(scissorEnabled ? &scissorRect : nullptr);
        }
    }

    void RenderDelegate::doRender(Sprite* sprite, const RenderState& rs)
    {
        if (!sprite->getAnimFrame().getTexture())
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

        STDRenderer* renderer = STDRenderer::getCurrent();
        renderer->flush();
        VideoDriver* driver = renderer->getDriver();
        Rect scissorRect(0, 0, 0, 0);
        bool scissorEnabled = driver->getScissorRect(scissorRect);
        driver->setScissorRect(nullptr);
        Material::null->apply();

        //QPainter painter(oxygine::GameWindow::getWindow());
        //painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        //if (painter.isActive())
        //{
        //    text::DrawContext dc;
        //    if (scissorEnabled)
        //    {
        //        painter.setClipRect(scissorRect.getX(), scissorRect.getY(), scissorRect.getWidth(), scissorRect.getHeight());
        //    }
        //    renderer->setTransform(rs.transform);
        //    dc.m_primary = premultiply(rs.getFinalColor(tf->getColor()));
        //    dc.m_color = tf->getStyle().color * dc.m_primary;
        //    if (!scissorEnabled)
        //    {
        //        root->draw(rs, tf->getStyle(), dc, painter, scissorRect, scissorEnabled);
        //    }
        //    painter.end();
        //}
        driver->setScissorRect(scissorEnabled ? &scissorRect : nullptr);
    }

    void RenderDelegate::doRender(ColorRectSprite* sprite, const RenderState& rs)
    {
        QColor color = rs.getFinalColor(sprite->getColor());
        sprite->getMaterial()->apply();
        sprite->getMaterial()->render(rs.transform, color, sprite->getAnimFrame().getSrcRect(), sprite->getDestRect());
    }
}
