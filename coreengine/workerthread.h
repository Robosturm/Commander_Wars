#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>

class WorkerThread final : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread();
    ~WorkerThread();
    bool getStarted() const;
signals:
    void sigStart();
    void sigShowMainwindow();
    void sigStartSlaveGame();
public slots:
    void start();
    void showMainwindow();
    void startSlaveGame();
private:
    bool m_started{false};
};

#endif // RENDERTHREAD_H
