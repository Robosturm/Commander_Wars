#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/RenderDelegate.h"

#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/Input.h"


#include <QMouseEvent>
#include <QTimerEvent>
#include <QMutexLocker>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#else
#include <QCoreApplication>
#endif
#include <limits>

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
        newFormat.setProfile(QSurfaceFormat::CoreProfile);
        newFormat.setSamples(2);    // Set the number of samples used for multisampling
        setFormat(newFormat);
#endif
        m_window = this;
        QObject::connect(this, &GameWindow::sigLoadSingleResAnim, this, &GameWindow::loadSingleResAnim, Qt::BlockingQueuedConnection);
        QObject::connect(this, &GameWindow::sigLoadRessources, this, &GameWindow::loadRessources, Qt::QueuedConnection);
        QObject::connect(this, &GameWindow::sigQuit, this, &GameWindow::quit, Qt::QueuedConnection);
        QObject::connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, this, &GameWindow::quitApp);
        QObject::connect(this, &GameWindow::sigShowKeyboard, this, &GameWindow::showKeyboard, Qt::QueuedConnection);
    }

    void GameWindow::shutdown()
    {
        QCoreApplication::processEvents();
        m_timerCycle = -1;
        m_Timer.stop();
        rsCache().reset();
        rsCache().setDriver(nullptr);
        MaterialCache::mc().clear();
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

    void GameWindow::loadResAnim(oxygine::spResAnim pAnim, QImage & image, qint32 columns, qint32 rows, float scaleFactor)
    {
        if (!m_shuttingDown && !m_noUi)
        {
            if (isMainThread())
            {
                loadSingleResAnim(pAnim, image, columns, rows, scaleFactor);
            }
            else
            {
                emit sigLoadSingleResAnim(pAnim, image, columns, rows, scaleFactor);
            }
        }
    }

    void GameWindow::loadSingleResAnim(oxygine::spResAnim pAnim, QImage image, qint32 columns, qint32 rows, float scaleFactor)
    {
        if (pAnim.get() != nullptr && !m_noUi)
        {
            pAnim->init(image, columns, rows, scaleFactor);
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

    bool GameWindow::sameTouchpoint(const QPointF & pos1, const QPointF & pos2) const
    {
        return qAbs(pos1.x() - pos2.x()) + qAbs(pos1.y() - pos2.y()) <= Settings::getInstance()->getTouchPointSensitivity();
    }

    QThread * GameWindow::getMainThread() const
    {
        return m_pMainThread;
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
