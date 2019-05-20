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

    inline Interpreter *getInterpreter() const
    {
        return m_pInterpreter;
    }
signals:
    void sigStart();
protected slots:
    void start();
private:
    Interpreter* m_pInterpreter{nullptr};
    bool started{false};
};

#endif // RENDERTHREAD_H
