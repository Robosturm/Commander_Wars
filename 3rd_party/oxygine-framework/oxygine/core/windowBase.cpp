#include "3rd_party/oxygine-framework/oxygine/core/windowBase.h"

QSize WindowBaseClass::size() const
{
    return QSize();
}
qint32 WindowBaseClass::x() const
{
    return 0;
}

qint32 WindowBaseClass::y() const
{
    return 0;
}

QPoint WindowBaseClass::position() const
{
    return QPoint();
}

void WindowBaseClass::setPosition(qint32 x, qint32 y)
{
}

void WindowBaseClass::setTitle(const QString & title)
{
}

void WindowBaseClass::keyPressEvent(QKeyEvent *event)
{
}

void WindowBaseClass::keyReleaseEvent(QKeyEvent *event)
{
}

bool WindowBaseClass::isActive() const
{
    return false;
}

float WindowBaseClass::devicePixelRatio() const
{
    return 1.0;
}

void WindowBaseClass::setPosition(QPoint pos)
{
}

QPoint WindowBaseClass::mapFromGlobal(QPoint pos) const
{
    return pos;
}

QPoint WindowBaseClass::mapToGlobal(QPoint pos) const
{
    return pos;
}
