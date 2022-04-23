#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"

#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"


#include <QMouseEvent>
#include <QTimerEvent>
#include <QMutexLocker>
#include <qapplication.h>
#include <limits>

#include "coreengine/console.h"
#include "coreengine/settings.h"

namespace oxygine
{
    GameWindow* GameWindow::m_window = nullptr;

    GameWindow::GameWindow()
    {
        setObjectName("GameWindow");
        QSurfaceFormat newFormat = format();
        newFormat.setProfile(QSurfaceFormat::CoreProfile);
        newFormat.setSamples(2);    // Set the number of samples used for multisampling
        setFormat(newFormat);
        m_window = this;
        m_mainHandle = QThread::currentThreadId();
        connect(this, &GameWindow::sigLoadSingleResAnim, this, &GameWindow::loadSingleResAnim, Qt::BlockingQueuedConnection);
        connect(this, &GameWindow::sigLoadRessources, this, &GameWindow::loadRessources, Qt::QueuedConnection);
        connect(this, &GameWindow::sigQuit, this, &GameWindow::quit, Qt::QueuedConnection);
        connect(QApplication::instance(), &QApplication::aboutToQuit, this, &GameWindow::quitApp);
        connect(this, &GameWindow::sigShowKeyboard, this, &GameWindow::showKeyboard, Qt::QueuedConnection);
    }

    void GameWindow::shutdown()
    {
        QApplication::processEvents();
        m_timerCycle = -1;
        m_Timer.stop();
        rsCache().reset();
        rsCache().setDriver(nullptr);
        MaterialCache::mc().clear();
        STDRenderer::release();
        RenderDelegate::instance = nullptr;
        VideoDriver::instance = nullptr;
        Material::null = spMaterial();
        Material::current = spMaterial();
        Input::instance.cleanup();
        Stage::setStage(spStage());
        Resources::unregisterResourceType("atlas");
        Resources::unregisterResourceType("font");;
        m_window = nullptr;
    }

    void GameWindow::timerEvent(QTimerEvent *)
    {
        // Request an update
        update();
    }

    void GameWindow::updateData()
    {
        timeMS duration = VideoDriver::m_stats.duration;
        VideoDriver::m_stats = VideoDriver::Stats();
        VideoDriver::m_stats.duration = duration;
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
        QCoreApplication::exit(exitCode);
    }

    void GameWindow::paintGL()
    {
        updateData();
        if (m_pauseMutex.tryLock())
        {
            oxygine::Stage::getStage()->updateStage();
            if (beginRendering())
            {
                QColor clearColor(0, 0, 0, 255);
                QSize size = oxygine::GameWindow::getWindow()->size();
                oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(size.width(), size.height()));
                // Render all actors inside the stage. Actor::render will also be called for all its children
                oxygine::Stage::getStage()->renderStage(clearColor, viewport);
                swapDisplayBuffers();
                m_repeatedFramesDropped = 0;
            }
            m_pauseMutex.unlock();
        }
        else
        {
            ++m_repeatedFramesDropped;
            if (m_repeatedFramesDropped > 10)
            {
                update();
            }
        }
        // check for termination
        if (m_quit)
        {
            QApplication::exit();
        }
    }

    bool GameWindow::beginRendering()
    {
        if (!m_renderEnabled)
        {
            return false;
        }

        bool ready = STDRenderer::isReady();
        if (ready)
        {
            rsCache().reset();
            VideoDriver::m_stats.start = Clock::getTimeMS();
            rsCache().reset();
        }
        else
        {
            CONSOLE_PRINT("!ready", Console::eDEBUG);
        }

        return ready;
    }

    bool GameWindow::isReady2Render()
    {
        if (!m_renderEnabled)
        {
            return false;
        }
        return STDRenderer::isReady();
    }

    void GameWindow::swapDisplayBuffers()
    {
        VideoDriver::m_stats.duration = Clock::getTimeMS() - VideoDriver::m_stats.start;
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
        Resources::registerResourceType(ResFontBM::create, "font");
    }

    void GameWindow::initializeGL()
    {
        initializeOpenGLFunctions();
        if (!hasOpenGLFeature(QOpenGLFunctions::Shaders))
        {
            CONSOLE_PRINT("Shaders are not supported by open gl. This may result in a black screen.", Console::eWARNING);
        }
        if (!hasOpenGLFeature(QOpenGLFunctions::Multitexture))
        {
            CONSOLE_PRINT("Multitextures are not supported by open gl. This may result in a black screen.", Console::eWARNING);
        }
        // init oxygine engine
        CONSOLE_PRINT("initialize oxygine", Console::eDEBUG);
        VideoDriver::instance = spVideoDriver::create();
        VideoDriver::instance->setDefaultSettings();
        rsCache().setDriver(VideoDriver::instance.get());

        STDRenderer::initialize();

        STDRenderer::instance = spSTDRenderer::create();
        RenderDelegate::instance = spRenderDelegate::create();
        Material::null = spMaterial::create();
        Material::current = Material::null;

        STDRenderer::current = STDRenderer::instance;
        launchGame();
    }

    void GameWindow::launchGame()
    {
        if (!m_launched)
        {
            registerResourceTypes();
            // Create the stage. Stage is a root node for all updateable and drawable objects
            oxygine::Stage::setStage(oxygine::spStage::create());
            emit sigLoadRessources();
        }
    }

    void GameWindow::resizeGL(qint32 w, qint32 h)
    {
        CONSOLE_PRINT("core::restore()", Console::eDEBUG);
        VideoDriver::instance->restore();
        STDRenderer::restore();
        CONSOLE_PRINT("core::restore() done", Console::eDEBUG);
    }

    void GameWindow::loadResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder)
    {
        if (!m_shuttingDown && !m_noUi)
        {
            if (isMainThread())
            {
                loadSingleResAnim(pAnim, image, columns, rows, scaleFactor, addTransparentBorder);
            }
            else
            {
                emit sigLoadSingleResAnim(pAnim, image, columns, rows, scaleFactor, addTransparentBorder);
            }
        }
    }

    void GameWindow::loadSingleResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor, bool addTransparentBorder)
    {
        if (pAnim.get() != nullptr && !m_noUi)
        {
            pAnim->init(image, columns, rows, scaleFactor, addTransparentBorder);
        }
    }

    void GameWindow::mousePressEvent(QMouseEvent *event)
    {
        MouseButton b = MouseButton_Left;
        switch (event->button())
        {
            case Qt::MouseButton::LeftButton:
            {
                b = MouseButton_Left;
                break;
            }
            case Qt::MouseButton::MiddleButton:
            {
                b = MouseButton_Middle;
                break;
            }
            case Qt::MouseButton::RightButton:
            {
                b = MouseButton_Right;
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
        MouseButton b = MouseButton_Left;
        switch (event->button())
        {
            case Qt::MouseButton::LeftButton:
            {
                b = MouseButton_Left;
                break;
            }
            case Qt::MouseButton::MiddleButton:
            {
                b = MouseButton_Middle;
                break;
            }
            case Qt::MouseButton::RightButton:
            {
                b = MouseButton_Right;
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

    void GameWindow::touchEvent(QTouchEvent *event)
    {
        QList<QTouchEvent::TouchPoint> touchPoints = event->points();
        switch (event->type())
        {
            case QEvent::TouchBegin:
            {
                if (touchPoints.count() == 1)
                {
                    const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                    emit sigMousePressEvent(MouseButton_Left, touchPoint0.position().x(), touchPoint0.position().y());
                    m_longPressSent = false;
                    m_touchMousePressSent = true;
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
                if (m_touchMousePressSent && !m_longPressSent)
                {
                    const QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
                    emit sigMouseReleaseEvent(MouseButton_Left, touchPoint0.position().x(), touchPoint0.position().y());
                }
                m_touchMousePressSent = false;
                m_longPressSent = false;
                m_lastZoomValue = 1.0f;
            }
            default:
                break;
        }
    }

    bool GameWindow::sameTouchpoint(QPointF pos1, QPointF pos2) const
    {
        return qAbs(pos1.x() - pos2.x()) + qAbs(pos1.y() - pos2.y()) <= Settings::getTouchPointSensitivity();
    }

    void GameWindow::setTimerCycle(qint32 newTimerCycle)
    {
        m_timerCycle = newTimerCycle;
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

    qint32 GameWindow::getTimerCycle() const
    {
        return m_timerCycle;
    }

    QOpenGLContext* GameWindow::getGLContext()
    {
        return m_window->context();
    }

    GameWindow* GameWindow::getWindow()
    {
        return m_window;
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

    bool GameWindow::hasCursor()
    {
        QPoint position = cursor().pos();
        if (position.x() < x() || position.y() < y() ||
            position.x() > x() + width() || position.y() > y() + height())
        {
            return false;
        }
        return true;
    }

    void GameWindow::showKeyboard(bool visible)
    {
        auto virtualKeyboard = QGuiApplication::inputMethod();
        if (virtualKeyboard != nullptr)
        {
            if (visible)
            {
                CONSOLE_PRINT("Show virtual keyboard", Console::eDEBUG);
                virtualKeyboard->show();
            }
            else
            {
                if (virtualKeyboard->isVisible())
                {
                    CONSOLE_PRINT("Hide virtual keyboard", Console::eDEBUG);
                    virtualKeyboard->hide();
                }
            }
        }
    }

}
