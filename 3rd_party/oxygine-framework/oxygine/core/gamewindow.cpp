#include "3rd_party/oxygine-framework/oxygine/core/gamewindow.h"
#include "3rd_party/oxygine-framework/oxygine/core/oxygine.h"
#include "3rd_party/oxygine-framework/oxygine/core/gl/VideoDriverGLES20.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/MaterialCache.h"
#include "3rd_party/oxygine-framework/oxygine/PostProcess.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/res/CreateResourceContext.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResAtlas.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResBuffer.h"
#include "3rd_party/oxygine-framework/oxygine/res/ResFontBM.h"
#include "3rd_party/oxygine-framework/oxygine/res/Resources.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderer.h"
#include "3rd_party/oxygine-framework/oxygine/Clock.h"
#include "3rd_party/oxygine-framework/oxygine/STDRenderDelegate.h"

#include <QMouseEvent>
#include <QTimerEvent>
#include <QMutexLocker>
#include <qapplication.h>


namespace oxygine
{
    GameWindow* GameWindow::_window = nullptr;

    GameWindow::GameWindow()
        : _dispatcher(new EventDispatcher())
    {
        QSurfaceFormat newFormat = format();
        newFormat.setSamples(2);    // Set the number of samples used for multisampling
        setFormat(newFormat);

        _window = this;
        m_mainHandle = QThread::currentThreadId();
        connect(this, &GameWindow::sigLoadSingleResAnim, this, &GameWindow::loadSingleResAnim, Qt::BlockingQueuedConnection);
    }

    GameWindow::~GameWindow()
    {
        _dispatcher->removeAllEventListeners();

        rsCache().reset();
        rsCache().setDriver(nullptr);

        clearPostProcessItems();
        PostProcess::freeShaders();

        MaterialCache::mc().clear();

        STDRenderer::release();
        STDRenderDelegate::instance = nullptr;
        IVideoDriver::instance = nullptr;

        Input::instance.cleanup();

        if (Stage::instance)
        {
            Stage::instance->cleanup();
        }
        Stage::instance = nullptr;

        Resources::unregisterResourceType("atlas");
        Resources::unregisterResourceType("buffer");
        Resources::unregisterResourceType("font");
        Resources::unregisterResourceType("bmfc_font");
        Resources::unregisterResourceType("sdfont");
    }

    void GameWindow::timerEvent(QTimerEvent *)
    {
        // Request an update
        update();
    }

    void GameWindow::updateData()
    {
        timeMS duration = IVideoDriver::_stats.duration;
        IVideoDriver::_stats = IVideoDriver::Stats();
        IVideoDriver::_stats.duration = duration;
    }

    void GameWindow::paintGL()
    {
        updateData();
        if (m_pauseMutex.tryLock())
        {
            oxygine::getStage()->update();
            if (beginRendering())
            {
                QColor clearColor(181, 255, 32, 255);
                QSize size = oxygine::GameWindow::getWindow()->size();
                oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(size.width(), size.height()));
                // Render all actors inside the stage. Actor::render will also be called for all its children
                oxygine::getStage()->render(clearColor, viewport);
                swapDisplayBuffers();
            }
            m_pauseMutex.unlock();
        }
        // check for termination
        if (m_quit)
        {
            QApplication::exit();
        }
    }

    bool GameWindow::beginRendering()
    {
        if (!_renderEnabled)
        {
            return false;
        }
        //        if (!QWindow::isActive())
        //        {
        //           return false;
        //        }

        bool ready = STDRenderer::isReady();
        if (ready)
        {
            rsCache().reset();
            IVideoDriver::_stats.start = Clock::getTimeMS();
            updatePortProcessItems();
            rsCache().reset();
        }
        else
        {
            qDebug("!ready");
        }

        return ready;
    }

    bool GameWindow::isReady2Render()
    {
        if (!_renderEnabled)
        {
            return false;
        }
        return STDRenderer::isReady();
    }

    void GameWindow::swapDisplayBuffers()
    {
        IVideoDriver::_stats.duration = Clock::getTimeMS() - IVideoDriver::_stats.start;
        //sleep(1000/50);
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
        Resources::registerResourceType(ResBuffer::create, "buffer");
        Resources::registerResourceType(ResFontBM::create, "font");
        Resources::registerResourceType(ResFontBM::createBM, "bmfc_font");
        Resources::registerResourceType(ResFontBM::createSD, "sdfont");
    }

    void GameWindow::initializeGL()
    {
        initializeOpenGLFunctions();
        if (!hasOpenGLFeature(QOpenGLFunctions::Shaders))
        {
            qWarning("Shaders are not supported by open gl. This may result in a black screen.");
        }
        if (!hasOpenGLFeature(QOpenGLFunctions::Multitexture))
        {
            qWarning("Multitextures are not supported by open gl. This may result in a black screen.");
        }
        // init oxygine engine
        qDebug("initialize oxygine");
        qDebug("Qt build");
        IVideoDriver::instance = new VideoDriverGLES20();

        IVideoDriver::instance->setDefaultSettings();

        rsCache().setDriver(IVideoDriver::instance.get());

        STDRenderer::initialize();

        registerResourceTypes();

        STDRenderer::instance = new STDRenderer;
        STDRenderDelegate::instance = new STDRenderDelegate;
        Material::null       = new NullMaterialX;
        Material::current = Material::null;

        STDRenderer::current = STDRenderer::instance;

        // Create the stage. Stage is a root node for all updateable and drawable objects
        oxygine::Stage::instance = new oxygine::Stage();
        QSize size = GameWindow::size();
        oxygine::getStage()->setSize(size.width(), size.height());

        loadRessources();
    }

    void GameWindow::resizeGL(int w, int h)
    {
        qDebug("core::restore()");
        IVideoDriver::instance->restore();
        STDRenderer::restore();
        Restorable::restoreAll();
        oxygine::Stage::instance->setSize(w, h);
        qDebug("core::restore() done");
    }

    void GameWindow::loadResAnim(oxygine::ResAnim* pAnim, const QImage & image, qint32 columns, qint32 rows, float scaleFactor)
    {
        if (QThread::currentThreadId() == m_mainHandle)
        {
            loadSingleResAnim(pAnim, image, columns, rows, scaleFactor);
        }
        else
        {
            emit sigLoadSingleResAnim(pAnim, image, columns, rows, scaleFactor);
        }
    }

    void GameWindow::loadSingleResAnim(oxygine::ResAnim* pAnim, const QImage & image, qint32 columns, qint32 rows, float scaleFactor)
    {
        pAnim->init(image, columns, rows, scaleFactor);
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
        emit sigMousePressEvent(b, event->x(), event->y());
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
        emit sigMouseReleaseEvent(b, event->x(), event->y());
    }

    void GameWindow::wheelEvent(QWheelEvent *event)
    {
        emit sigWheelEvent(event->angleDelta().x(), event->angleDelta().y());
    }
    void GameWindow::mouseMoveEvent(QMouseEvent *event)
    {
        emit sigMouseMoveEvent(event->x(), event->y());
    }

    spEventDispatcher GameWindow::getDispatcher()
    {
        return _dispatcher;
    }

    QOpenGLContext* GameWindow::getGLContext()
    {
        return _window->context();
    }

    GameWindow* GameWindow::getWindow()
    {
        return _window;
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
}
