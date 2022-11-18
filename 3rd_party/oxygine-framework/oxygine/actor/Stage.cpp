#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"

#include <QPainter>
#include <QPainterPath>

namespace oxygine
{
    spStage Stage::instance;

    Stage::Stage()
        : m_statUpdate(0)
    {
        spClock clock = spClock::create();
        setClock(clock);
        m_stage = this;
    }

    void Stage::onDeactivate(Event*)
    {
        core::reset();
    }


    void Stage::init(const Point& displaySize, const Point& gameSize)
    {
        float scaleFactor = displaySize.x / static_cast<float>(gameSize.x);
        setScale(scaleFactor);
        setSize(gameSize);
    }

    bool Stage::isOn(const Vector2&, float)
    {
        return true;
    }


    RectF Stage::getDestRect() const
    {
        Vector2 s = getSize() + getPosition();
        return RectF(-getPosition(), s);
    }

    void Stage::renderStage(const QColor* clearColor, const Rect& viewport, const QMatrix4x4 & viewProjection)
    {
        spVideoDriver driver = VideoDriver::instance;
        driver->setViewport(viewport);

        if (clearColor)
        {
            driver->clear(*clearColor);
        }
        STDRenderer::instance->setViewProj(viewProjection);
        RenderState rs;
        RectF clip(0.0f, 0.0f, viewport.getWidth(), viewport.getHeight());
        rs.clip = &clip;

        Actor::render(rs);
        STDRenderer::getCurrent()->flush();
        Material::null->apply();
    }

    void Stage::renderStage(const QColor& clearColor, const Rect& viewport)
    {
        auto viewProjection = getViewProjectionMatrix(viewport);
        renderStage(&clearColor, viewport, viewProjection);
    }

    QMatrix4x4 Stage::getViewProjectionMatrix(const Rect& viewport)
    {
        //initialize projection and view matrix
        static constexpr float zNear = 0.2f;
        static constexpr float zFar = 10000.0f;
        static constexpr float m33 = 1 / (zFar - zNear);
        static constexpr float m34 = zNear / (zNear - zFar);
        const float width = viewport.getWidth();
        const float height = viewport.getHeight();
        QMatrix4x4 viewProjection(2.0f / width, 0, 0, -1,
                                  0, -2.0f / height, 0, 1,
                                  0, 0, m33, m34,
                                  0, 0, 0, 1);
        return viewProjection;
    }

    void Stage::cleanup()
    {
        removeChildren();
    }

    void Stage::updateStage()
    {
        timeMS t = Clock::getTimeMS();
        UpdateState us;
        Actor::update(us);

        m_statUpdate = Clock::getTimeMS() - t;
    }
}
