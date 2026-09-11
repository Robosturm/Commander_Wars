#pragma once
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QMutex>
#include <atomic>

#include "3rd_party/oxygine-framework/oxygine/core/renderer.h"

using spQThread = std::shared_ptr<QThread>;

namespace oxygine
{
    class WindowBase : public QOpenGLWindow, public QOpenGLFunctions
    {
        Q_OBJECT
    public:
        WindowBase();
        virtual ~WindowBase();
        void setupRendering();
        Renderer & getRenderer()
        {
            return m_renderer;
        }
    public slots:
        virtual void initializeGL() override;
        virtual void launchGame() = 0;
        void redrawUi();
    protected:
        virtual void resizeGL(qint32 w, qint32 h) override;
        virtual void paintGL() override;
        virtual void updateData() = 0;
        static QSurfaceFormat::RenderableType getRenderableType();
    private:
        void swapDisplayBuffers();
    protected:
        friend Renderer;
        bool m_renderEnabled{true};
        std::atomic<quint8> m_pausedCounter{0};
        QMutex m_renderSync;
        bool m_quit{false};
        bool m_terminating{false};
        bool m_noUi{false};
        bool m_renderingInitialized{false};
        spQThread m_renderThread;
        Renderer m_renderer;
    };
}
