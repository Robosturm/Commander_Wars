#pragma once
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
#include <QMutex>

namespace oxygine
{
    class WindowBase : public QOpenGLWindow, public QOpenGLFunctions
    {
        Q_OBJECT
    public:
        WindowBase() = default;
        virtual ~WindowBase() = default;
        void redrawUi();
    public slots:
        virtual void initializeGL() override;
        virtual void launchGame() = 0;
    protected:
        virtual void resizeGL(qint32 w, qint32 h) override;
        virtual void paintGL() override;
        virtual void updateData() = 0;
        bool beginRendering();
    private:
        void swapDisplayBuffers();
    protected:
        bool m_renderEnabled{true};
        qint32 m_repeatedFramesDropped{0};
        qint32 m_pausedCounter{0};
        QMutex m_renderSync;
        bool m_quit{false};
        bool m_terminating{false};
        bool m_noUi{false};
    };
}
