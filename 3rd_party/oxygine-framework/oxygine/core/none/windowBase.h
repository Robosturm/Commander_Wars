#pragma once
#include <QObject>
#include <QKeyEvent>
#include <QMutex>

namespace oxygine
{
    class WindowBase : public QObject
    {
        Q_OBJECT
    public:
        WindowBase() = default;
        ~WindowBase() = default;

        QSize size() const;
        qint32 x() const;
        qint32 y() const;
        QPoint position() const;
        void setPosition(qint32 x, qint32 y);
        void setPosition(QPoint pos);
        void setTitle(const QString & title);
        bool isActive() const;
        float devicePixelRatio() const;
        QPoint mapFromGlobal(QPoint pos) const;
        QPoint mapToGlobal(QPoint pos) const;
        void redrawUi();
    public slots:
        virtual void launchGame() = 0;
    signals:
        void activeChanged();

    protected:
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);
        virtual void updateData() = 0;
        virtual void timerEvent(QTimerEvent *);
        // input events
        virtual void mousePressEvent(QMouseEvent *event) = 0;
        virtual void mouseReleaseEvent(QMouseEvent *event) = 0;
        virtual void wheelEvent(QWheelEvent *event) = 0;
        virtual void mouseMoveEvent(QMouseEvent *event) = 0;
        virtual void touchEvent(QTouchEvent *event) = 0;

        bool beginRendering();
    protected:
        bool m_renderEnabled{true};
        qint32 m_repeatedFramesDropped{0};
        QMutex m_pauseMutex;
        qint32 m_pausedCounter{0};
        bool m_quit{false};
        bool m_noUi{false};
    };
}
