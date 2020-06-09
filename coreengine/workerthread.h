#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "qthread.h"
#include "qtimer.h"

#include "coreengine/interpreter.h"

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
protected slots:
    void start();
    void showMainwindow();
private:
    bool started{false};
};

#endif // RENDERTHREAD_H
