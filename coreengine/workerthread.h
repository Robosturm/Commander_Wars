#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>
#include <QElapsedTimer>
#include "3rd_party/oxygine-framework/oxygine/PointerState.h"

class WorkerThread;
using spWorkerThread = std::shared_ptr<WorkerThread>;

class WorkerThread final : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread();
    virtual ~WorkerThread();
    bool getStarted() const;
signals:
    void sigStart();
    void sigShowMainwindow();
    void sigStartSlaveGame();
public slots:
    void start();
    void showMainwindow();
    void startSlaveGame();
private slots:
    void mousePressEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void mouseReleaseEvent(oxygine::MouseButton button, qint32 x, qint32 y);
    void mouseMoveEvent(qint32 x, qint32 y);
    void wheelEvent(qint32 x, qint32 y);
private:
    bool m_started{false};
};

#endif // RENDERTHREAD_H
