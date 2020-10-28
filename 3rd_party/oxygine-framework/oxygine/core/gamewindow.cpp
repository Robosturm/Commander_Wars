#include "gamewindow.h"

#include <cmath>

#include "../KeyEvent.h"

#include <QMouseEvent>
#include <QTimerEvent>

#include "../actor/Stage.h"

#include "../STDRenderDelegate.h"
#include "../PostProcess.h"
#include "oxygine.h"

#include "qapplication.h"

#include "../MaterialCache.h"
#include "../PostProcess.h"
#include "../STDRenderDelegate.h"
#include "../actor/Stage.h"
#include "../actor/TextField.h"
#include "../res/CreateResourceContext.h"
#include "../res/ResAtlas.h"
#include "../res/ResBuffer.h"
#include "../res/ResFontBM.h"
#include "../res/Resources.h"
#include "gl/VideoDriverGLES20.h"

#include "../STDRenderer.h"

#include "../Clock.h"

#include "QMutexLocker"


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

        connect(this, &GameWindow::sigLoadSingleResAnim, this, &GameWindow::loadSingleResAnim, Qt::BlockingQueuedConnection);
    }

    GameWindow::~GameWindow()
    {
        _dispatcher->removeAllEventListeners();

        rsCache().reset();
        rsCache().setDriver(nullptr);

        clearPostProcessItems();
        PostProcess::freeShaders();

        mc().clear();

        STDRenderer::release();
        delete STDRenderDelegate::instance;
        STDRenderDelegate::instance = nullptr;

        delete IVideoDriver::instance;
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
        if (m_Mutex.tryLock())
        {
            oxygine::getStage()->update();
            m_Mutex.unlock();
        }
        if (beginRendering())
        {
            QColor clearColor(181, 255, 32, 255);
            QSize size = oxygine::GameWindow::getWindow()->size();
            oxygine::Rect viewport(oxygine::Point(0, 0), oxygine::Point(size.width(), size.height()));
            // Render all actors inside the stage. Actor::render will also be called for all its children
            if (m_Mutex.tryLock())
            {
                oxygine::getStage()->render(clearColor, viewport);
                m_Mutex.unlock();
            }
            swapDisplayBuffers();
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
            return false;
        return STDRenderer::isReady();
    }

    void GameWindow::swapDisplayBuffers()
    {
        IVideoDriver::_stats.duration = Clock::getTimeMS() - IVideoDriver::_stats.start;
        //sleep(1000/50);
    }
    
    qint32 GameWindow::getLockCounter() const
    {
        return m_lockCounter;
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

        rsCache().setDriver(IVideoDriver::instance);

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
        QMutexLocker lock(&m_Mutex);
        qDebug("core::restore()");
        IVideoDriver::instance->restore();
        STDRenderer::restore();
        Restorable::restoreAll();
        oxygine::Stage::instance->setSize(w, h);
        qDebug("core::restore() done");
    }

    void GameWindow::loadResAnim(oxygine::spResAnim pAnim, const QImage & image)
    {
        emit sigLoadSingleResAnim(pAnim, image);
    }

    void GameWindow::loadSingleResAnim(oxygine::spResAnim pAnim, const QImage & image)
    {
        pAnim->init(image);
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
