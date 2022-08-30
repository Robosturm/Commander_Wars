#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "qthread.h"
#include "qtimer.h"

#include "coreengine/interpreter.h"

#include "3rd_party/oxygine-framework/oxygine/PointerState.h"

class WorkerThread : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread();
    virtual ~WorkerThread() = default;
    bool getStarted() const;
signals:
    void sigStart();
    void sigShowMainwindow();
    void sigStartSlaveGame();
public slots:
    void start();
    void showMainwindow();
    void startSlaveGame();
    void mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void wheelEvent(qint32 x, qint32 y);
    void mouseMoveEvent(qint32 x, qint32 y);
    void onQuit();
private:
    bool m_started{false};
};

#endif // RENDERTHREAD_H
