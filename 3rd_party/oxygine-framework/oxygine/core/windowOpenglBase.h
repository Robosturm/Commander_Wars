#pragma once
#include <QOpenGLWindow>
#include <qopenglfunctions.h>

class WindowBaseClass : public QOpenGLWindow, public QOpenGLFunctions
{
    Q_OBJECT
public:
    WindowBaseClass() = default;
    ~WindowBaseClass() = default;
};
