#include "3rd_party/oxygine-framework/oxygine/core/none/windowBase.h"

namespace oxygine
{
    QSize WindowBase::size() const
    {
        return QSize();
    }
    qint32 WindowBase::x() const
    {
        return 0;
    }

    qint32 WindowBase::y() const
    {
        return 0;
    }

    QPoint WindowBase::position() const
    {
        return QPoint();
    }

    void WindowBase::redrawUi()
    {
    }

    void WindowBase::setPosition(qint32 x, qint32 y)
    {
    }

    void WindowBase::setTitle(const QString & title)
    {
    }

    void WindowBase::keyPressEvent(QKeyEvent *event)
    {
    }

    void WindowBase::keyReleaseEvent(QKeyEvent *event)
    {
    }

    bool WindowBase::isActive() const
    {
        return false;
    }

    float WindowBase::devicePixelRatio() const
    {
        return 1.0;
    }

    void WindowBase::setPosition(QPoint pos)
    {
    }

    QPoint WindowBase::mapFromGlobal(QPoint pos) const
    {
        return pos;
    }

    QPoint WindowBase::mapToGlobal(QPoint pos) const
    {
        return pos;
    }

    bool WindowBase::beginRendering()
    {
        return false;
    }

    void WindowBase::timerEvent(QTimerEvent *)
    {
    }
}
