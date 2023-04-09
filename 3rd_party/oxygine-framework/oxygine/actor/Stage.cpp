#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/Material.h"

namespace oxygine
{
    spStage Stage::instance{nullptr};

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


    void Stage::init(const QSize& gameSize, const qreal scaleFactor)
    {
        setScale(scaleFactor);
        setSize(gameSize.width(), gameSize.height());
    }

    bool Stage::isOn(const QPoint&)
    {
        return true;
    }


    QRect Stage::getDestRect() const
    {
        return QRect(-getX(), -getY(),
                     getX() + getWidth(), getY() + getHeight());
    }

    void Stage::renderStage(const QColor* clearColor, const QRect& viewport, const QMatrix4x4 & viewProjection)
    {
        spVideoDriver driver = VideoDriver::instance;
        driver->setViewport(viewport);

        if (clearColor != nullptr)
        {
            driver->clear(*clearColor);
        }
        STDRenderer::instance->setViewProj(viewProjection);
        RenderState rs;        
        rs.clip = QRect(0, 0, viewport.width(), viewport.height());

        Actor::render(rs);
        STDRenderer::getCurrent()->flush();
        Material::null->apply();
    }

    void Stage::renderStage(const QColor& clearColor, const QRect& viewport)
    {
        auto viewProjection = getViewProjectionMatrix(viewport);
        renderStage(&clearColor, viewport, viewProjection);
    }

    QMatrix4x4 Stage::getViewProjectionMatrix(const QRectF& viewport)
    {
        //initialize projection and view matrix
        const float width = viewport.width();
        const float height = viewport.height();
        QMatrix4x4 viewProjection(2.0f / width, 0, 0, -1,
                                  0, -2.0f / height, 0, 1,
                                  0, 0, 1, 0,
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
