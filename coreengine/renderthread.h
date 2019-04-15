#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "qthread.h"
#include "qtimer.h"

class RenderThread : public QThread
{
    Q_OBJECT
public:
    RenderThread();
    virtual ~RenderThread();

public slots:
    void update();
protected:
    virtual void run()  override;
private:
    QTimer m_Timer;
};

#endif // RENDERTHREAD_H
