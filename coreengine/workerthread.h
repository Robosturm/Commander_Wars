#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "qthread.h"
#include "qtimer.h"

#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    WorkerThread();
    virtual ~WorkerThread();
    bool getStarted() const;
signals:
    void sigStart();
    void sigShowMainwindow();
    void sigStartSlaveGame();
protected slots:
    void start();
    void showMainwindow();
    void startSlaveGame();
    void mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void mouseLongPressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void wheelEvent(qint32 x, qint32 y);
    void mouseMoveEvent(qint32 x, qint32 y);
    void touchZoomEvent(qint32 x, qint32 y);
    void touchScrollEvent(qint32 x, qint32 y);
private:
    bool m_started{false};
};

#endif // RENDERTHREAD_H
