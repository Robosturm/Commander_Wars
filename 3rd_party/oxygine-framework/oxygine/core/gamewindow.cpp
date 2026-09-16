#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"

#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"


#include <QMouseEvent>
#include <QTimerEvent>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#include <QColorSpace>
#else
#include <QCoreApplication>
#endif

#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"

namespace oxygine
{
    GameWindow* GameWindow::m_window(nullptr);

    GameWindow::GameWindow()
    {
#ifdef GRAPHICSUPPORT
        setObjectName("GameWindow");
        QSurfaceFormat newFormat = format();

        // set OpenGL-related parameters
        newFormat.setProfile(QSurfaceFormat::CoreProfile);
        newFormat.setRenderableType(getRenderableType());
        newFormat.setSamples(2);

        // set color format
        newFormat.setColorSpace(QColorSpace::NamedColorSpace::SRgb);
        newFormat.setRedBufferSize(8);
        newFormat.setBlueBufferSize(8);
        newFormat.setGreenBufferSize(8);
        newFormat.setAlphaBufferSize(0);

        // set framebuffer parameters
        newFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        newFormat.setSwapInterval(1);
        newFormat.setDepthBufferSize(0);
        newFormat.setStencilBufferSize(0);

        setFormat(newFormat);
#endif
        m_window = this;
        QObject::connect(this, &GameWindow::sigLoadSingleResAnim, &m_renderer, &Renderer::loadSingleResAnim, Qt::BlockingQueuedConnection);
        QObject::connect(this, &GameWindow::sigLoadRessources, this, &GameWindow::loadRessources, Qt::QueuedConnection);
        QObject::connect(this, &GameWindow::sigQuit, this, &GameWindow::quit, Qt::QueuedConnection);
        QObject::connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &GameWindow::quitApp);
        QObject::connect(this, &GameWindow::sigShowKeyboard, this, &GameWindow::showKeyboard, Qt::QueuedConnection);
        // event loop handling
        connect(this, &GameWindow::sigMouseMoveEvent, &m_renderer, &Renderer::mouseMoveEvent, Qt::QueuedConnection);
        connect(this, &GameWindow::sigWheelEvent, &m_renderer, &Renderer::wheelEvent, Qt::QueuedConnection);
        connect(this, &GameWindow::sigMousePressEvent, &m_renderer, &Renderer::mousePressEvent, Qt::QueuedConnection);
        connect(this, &GameWindow::sigMouseReleaseEvent, &m_renderer, &Renderer::mouseReleaseEvent, Qt::QueuedConnection);
        
        // start debounce timer
        m_debounceTimer.leftDown.start();
        m_debounceTimer.rightDown.start();
        m_debounceTimer.middleDown.start();
        m_debounceTimer.leftUp.start();
        m_debounceTimer.middleUp.start();
        m_debounceTimer.rightUp.start();
    }

    void GameWindow::shutdown()
    {
        QCoreApplication::processEvents(QEventLoop::ProcessEventsFlag::AllEvents, 5);
        rsCache().reset();
        rsCache().setDriver(nullptr);
        MaterialCache::mc().release();
        STDRenderer::release();
        RenderDelegate::instance.reset();
        VideoDriver::instance.reset();
        Material::null = spMaterial();
        Material::current = spMaterial();
        Input::getInstance().cleanup();
        Stage::setStage(spStage());
        Resources::unregisterResourceType("atlas");
        m_window = nullptr;
    }

    void GameWindow::updateData()
    {
        if (isActive() && FocusableObject::getFocusedObject() == nullptr)
        {
            showKeyboard(false);
        }
    }

    void GameWindow::quitApp()
    {
        m_shuttingDown = true;
        onQuit();
    }

    void GameWindow::quit(qint32 exitCode)
    {
        CONSOLE_PRINT("Quitting game with " + QString::number(exitCode), GameConsole::eDEBUG);
        QCoreApplication::exit(exitCode);
    }

    bool GameWindow::isReady2Render()
    {
        if (!m_renderEnabled)
        {
            return false;
        }
        return STDRenderer::isReady();
    }

    float GameWindow::getGamma() const
    {
        return m_gamma;
    }

    void GameWindow::setGamma(float gamma)
    {
        m_gamma = gamma;
    }

    float GameWindow::getBrightness() const
    {
        return m_brightness;
    }

    void GameWindow::setBrightness(float brightness)
    {
        m_brightness = brightness / 100.0f;
    }

    void GameWindow::registerResourceTypes()
    {
        Resources::registerResourceType(ResAtlas::create, "atlas");
    }

    void GameWindow::initStage()
    {
        if (oxygine::Stage::getStage().get() != nullptr)
        {
            auto ratio = Settings::getInstance()->getGameScale();
            auto width = Settings::getInstance()->getWidth();
            auto heigth = Settings::getInstance()->getHeight();
            oxygine::Stage::getStage()->init(QSize(width / ratio, heigth / ratio), ratio);
        }
    }

    void GameWindow::launchGame()
    {
        if (!m_launched)
        {
            m_launched = true;
            registerResourceTypes();
            // Create the stage. Stage is a root node for all updateable and drawable objects
            oxygine::Stage::setStage(MemoryManagement::create<oxygine::Stage>());
            initStage();
            emit sigLoadRessources();
        }
    }

    void GameWindow::loadResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool clamp2Edge, quint32 linearFilter)
    {
        if (!m_shuttingDown && !m_noUi)
        {
            if (isRenderThread())
            {
                m_renderer.loadSingleResAnim(pAnim, image, columns, rows, scaleFactor, clamp2Edge, linearFilter);
            }
            else
            {
                emit sigLoadSingleResAnim(pAnim, image, columns, rows, scaleFactor, clamp2Edge, linearFilter);
            }
        }
    }

    void GameWindow::mousePressEvent(QMouseEvent *event)
    {
        auto debounceTime = Settings::getInstance()->getDebounceTime();
        MouseButton b = MouseButton_Left;
        switch (event->button())
        {
            case Qt::MouseButton::LeftButton:
            {
                if (m_debounceTimer.leftDown.elapsed() >= debounceTime)
                {
                    b = MouseButton_Left;
                    m_debounceTimer.leftDown.start();
                }
                else
                {
                    return;
                }
                break;
            }
            case Qt::MouseButton::MiddleButton:
            {
                if (m_debounceTimer.middleDown.elapsed() >= debounceTime)
                {
                    b = MouseButton_Middle;
                    m_debounceTimer.middleDown.start();
                }
                else
                {
                    return;
                }
                break;
            }
            case Qt::MouseButton::RightButton:
            {
                if (m_debounceTimer.rightDown.elapsed() >= debounceTime)
                {
                    b = MouseButton_Right;
                    m_debounceTimer.rightDown.start();
                }
                else
                {
                    return;
                }
                break;
            }
            default:
            {
                // do nothing
            }
        }
        emit sigMousePressEvent(b, event->position().x(), event->position().y());
    }

    void GameWindow::mouseReleaseEvent(QMouseEvent *event)
    {
        auto debounceTime = Settings::getInstance()->getDebounceTime();
        MouseButton b = MouseButton_Left;
        switch (event->button())
        {
            case Qt::MouseButton::LeftButton:
            {
                if (m_debounceTimer.leftUp.elapsed() >= debounceTime)
                {
                    b = MouseButton_Left;
                    m_debounceTimer.leftUp.start();
                }
                else
                {
                    return;
                }
                break;
            }
            case Qt::MouseButton::MiddleButton:
            {
                if (m_debounceTimer.middleUp.elapsed() >= debounceTime)
                {
                    b = MouseButton_Middle;
                    m_debounceTimer.middleUp.start();
                }
                else
                {
                    return;
                }
                break;
            }
            case Qt::MouseButton::RightButton:
            {
                if (m_debounceTimer.rightUp.elapsed() >= debounceTime)
                {
                    b = MouseButton_Right;
                    m_debounceTimer.rightUp.start();
                }
                else
                {
                    return;
                }
                break;
            }
            default:
            {
                // do nothing
            }
        }
        emit sigMouseReleaseEvent(b, event->position().x(), event->position().y());
    }

    void GameWindow::wheelEvent(QWheelEvent *event)
    {
        emit sigWheelEvent(event->angleDelta().x(), event->angleDelta().y());
    }

    void GameWindow::mouseMoveEvent(QMouseEvent *event)
    {
        emit sigMouseMoveEvent(event->position().x(), event->position().y());
    }

    bool GameWindow::isConsoleTouchEvent(QList<QTouchEvent::TouchPoint> & touchPoints)
    {
        constexpr qint32 RECT_SIZE = 150;
        const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
        const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
        auto & stage = oxygine::Stage::getStage();
        if (touchPoint0.position().x() < RECT_SIZE && touchPoint0.position().y() < RECT_SIZE &&
            touchPoint1.position().x() > stage->getWidth() - RECT_SIZE && touchPoint1.position().y() > stage->getHeight() - RECT_SIZE)
        {
            return true;
        }
        return false;
    }

    void GameWindow::touchEvent(QTouchEvent *event)
    {
        QList<QTouchEvent::TouchPoint> touchPoints = event->points();
        switch (event->type())
        {
        case QEvent::TouchBegin:
        {
            m_longPress = true;
            if (touchPoints.count() == 1)
            {
                const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                emit sigMousePressEvent(MouseButton_Left, touchPoint0.position().x(), touchPoint0.position().y());
                m_longPressSent = false;
                m_touchMousePressSent = true;
            }
            else if (isConsoleTouchEvent(touchPoints))
            {
                m_consolePress = true;
            }
        }
        case QEvent::TouchUpdate:
        {
            handleZoomGesture(touchPoints);
            if (touchPoints.count() == 1 && !m_longPressSent)
            {
                const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                if (sameTouchpoint(touchPoint0.pressPosition(), touchPoint0.position()) &&
                    touchPoint0.timeHeld() >= 0.5)
                {
                    emit sigMousePressEvent(MouseButton_Right, touchPoint0.position().x(), touchPoint0.position().y());
                    emit sigMouseReleaseEvent(MouseButton_Right, touchPoint0.position().x(), touchPoint0.position().y());
                    m_longPressSent = true;
                }
                else
                {
                    emit sigMouseMoveEvent(touchPoint0.position().x(), touchPoint0.position().y());
                }
            }
            else if (isConsoleTouchEvent(touchPoints))
            {
                const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
                if (sameTouchpoint(touchPoint0.pressPosition(), touchPoint0.position()) &&
                    touchPoint0.timeHeld() >= 0.5 && sameTouchpoint(touchPoint1.pressPosition(), touchPoint1.position()) &&
                    touchPoint1.timeHeld() >= 0.5)
                {
                    m_longPress = true;
                }                
            }
            break;
        }
        case QEvent::TouchEnd:
        {
            if (touchPoints.count() == 1 && !m_longPressSent)
            {
                const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                if (sameTouchpoint(touchPoint0.pressPosition(), touchPoint0.position()))
                {
                    if (touchPoint0.timeHeld() >= 0.5)
                    {
                        emit sigMousePressEvent(MouseButton_Right, touchPoint0.position().x(), touchPoint0.position().y());                        
                        emit sigMouseReleaseEvent(MouseButton_Right, touchPoint0.position().x(), touchPoint0.position().y());
                        m_longPressSent = true;
                    }
                }
            }
            else if (m_consolePress && m_longPress && isConsoleTouchEvent(touchPoints))
            {
                emit GameConsole::getInstance()->sigToggleView();            
            }
            if (m_touchMousePressSent && !m_longPressSent)
            {
                const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                emit sigMouseReleaseEvent(MouseButton_Left, touchPoint0.position().x(), touchPoint0.position().y());
            }
            m_longPress = false;
            m_consolePress = false;
            m_touchMousePressSent = false;
            m_longPressSent = false;
            m_lastZoomValue = 1.0f;
        }
        default:
                break;
        }
    }

    bool GameWindow::sameTouchpoint(const QPointF & pos1, const QPointF & pos2) const
    {
        return qAbs(pos1.x() - pos2.x()) + qAbs(pos1.y() - pos2.y()) <= Settings::getInstance()->getTouchPointSensitivity();
    }

    QThread * GameWindow::getMainThread() const
    {
        return m_pMainThread;
    }

    void GameWindow::handleZoomGesture(QList<QTouchEvent::TouchPoint> & touchPoints)
    {
        if (touchPoints.count() == 2)
        {
            constexpr float minZoomOutGesture = 0.3f;
            constexpr float minZoomInGesture = 0.3f;
            // determine scale factor
            const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
            const QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();
            qreal scale = QLineF(touchPoint0.position(), touchPoint1.position()).length() /
                          QLineF(touchPoint0.pressPosition(), touchPoint1.pressPosition()).length();
            if (scale > m_lastZoomValue + 1 / (1 - minZoomInGesture))
            {
                m_lastZoomValue = scale;
                emit sigWheelEvent(1, 1);
            }
            else if (scale < m_lastZoomValue - minZoomOutGesture)
            {
                m_lastZoomValue = scale;
                emit sigWheelEvent(-1, -1);
            }
        }
    }

    void GameWindow::setShuttingDown(bool newShuttingDown)
    {
        m_shuttingDown = newShuttingDown;
    }

    bool GameWindow::getShuttingDown() const
    {
        return m_shuttingDown;
    }

    bool GameWindow::isEvenScale(qint32 width1, qint32 width2)
    {
        float scale1 = static_cast<float>(width1) / static_cast<float>(width2);
        float scale2 = static_cast<float>(width2) / static_cast<float>(width1);
        if ((fmodf(scale1, 2.0f) == 0.0f) ||
            (fmodf(scale2, 2.0f) == 0.0f))
        {
            return true;
        }
        return false;
    }

    bool GameWindow::hasCursor() const
    {
#ifdef GRAPHICSUPPORT
        QPoint position = cursor().pos();
        if (position.x() < x() || position.y() < y() ||
            position.x() > x() + width() || position.y() > y() + height())
        {
            return false;
        }
        return true;
#else
        return false;
#endif
    }

    void GameWindow::showKeyboard(bool visible)
    {
#ifdef GRAPHICSUPPORT
        auto virtualKeyboard = QApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            if (visible)
            {
                CONSOLE_PRINT("Show virtual keyboard", GameConsole::eDEBUG);
                virtualKeyboard->show();
            }
            else
            {
                if (virtualKeyboard->isVisible())
                {
                    CONSOLE_PRINT("Hide virtual keyboard", GameConsole::eDEBUG);
                    virtualKeyboard->hide();
                }
            }
        }
#endif
    }

}
