#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "qthread.h"
#include "qtimer.h"

#include "coreengine/interpreter.h"

class WorkerThread : public QThread
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
protected:
    virtual void run()  override;
private:
    Interpreter* m_pInterpreter{nullptr};
    bool started{false};
};

#endif // RENDERTHREAD_H
