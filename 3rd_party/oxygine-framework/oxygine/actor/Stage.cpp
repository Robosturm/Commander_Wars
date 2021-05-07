#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/RenderState.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/math/Rect.h"
#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

namespace oxygine
{
    spStage Stage::instance;

    Stage::Stage() : m_statUpdate(0), m_clipOuter(false), m_viewport(0, 0, 0, 0) //, _active(true)
    {
        spClock clock = spClock::create();
        setClock(clock);
        m_stage = this;
    }

    Stage::~Stage()
    {

    }

    void Stage::onDeactivate(Event*)
    {
        core::reset();
    }

    Rect    Stage::calcCenteredViewport(const Point& displaySize, const Point& gameSize)
    {
        float width = (float)displaySize.x;
        float height = (float)displaySize.y;

        float scaleFactorX = width / gameSize.x;
        float scaleFactorY = height / gameSize.y;

        float scaleFactor = scaleFactorX < scaleFactorY ? scaleFactorX : scaleFactorY;
        Vector2 size = gameSize * scaleFactor;

        Vector2 free = displaySize.cast<Vector2>() - size;

        return Rect((free / 2).cast<Point>(), size.cast<Point>());
    }

    void Stage::init(const Point& displaySize, const Point& gameSize)
    {
        setSize(gameSize);

        m_viewport = calcCenteredViewport(displaySize, gameSize);

        float scaleFactor = m_viewport.size.x / (float)gameSize.x;

        setScale(scaleFactor);
        setPosition(m_viewport.pos);
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

    void Stage::renderStage(const QColor* clearColor, const Rect& viewport, const Matrix& view, const Matrix& proj)
    {

        spIVideoDriver driver = IVideoDriver::instance;
        driver->setViewport(viewport);

        if (clearColor)
        {
            driver->clear(*clearColor);
        }

        Matrix vp = view * proj;
        STDRenderer::instance->setViewProj(vp);

        RenderState rs;

        QSize ds = oxygine::GameWindow::getWindow()->size();


        RectF clip(0.0f, 0.0f, (float)ds.width(), (float)ds.height());
        rs.clip = &clip;

        if (m_clipOuter)
        {
            driver->setScissorRect(&m_viewport);
            clip = m_viewport.cast<RectF>();
        }

        Actor::render(rs);
        STDRenderer::getCurrent()->flush();

        Material::null->apply();
    }

    void Stage::renderStage(const QColor& clearColor, const Rect& viewport)
    {
        //initialize projection and view matrix
        Matrix proj;
        Matrix::orthoLH(proj, (float)viewport.getWidth(), (float)viewport.getHeight(), 0.2f, 10000);
        Matrix view = makeViewMatrix(viewport.getWidth(), viewport.getHeight());
        renderStage(&clearColor, viewport, view, proj);
    }

    void Stage::cleanup()
    {
    }

    void Stage::updateStage()
    {
        timeMS t = Clock::getTimeMS();
        UpdateState us;
        Actor::update(us);

        m_statUpdate = Clock::getTimeMS() - t;
    }
}
