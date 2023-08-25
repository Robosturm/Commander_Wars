#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QObject>

class WorkerThread final : public QObject
{
    Q_OBJECT
public:
    explicit WorkerThread();
    ~WorkerThread() = default;
    bool getStarted() const;
signals:
    void sigStart();
    void sigShowMainwindow();
    void sigStartSlaveGame();
    void sigLoadScript(const QString & scriptPath);
public slots:
    void executeServerScript();
private slots:
    void start();
    void showMainwindow();
    void startSlaveGame();
    void onQuit();
    void loadScript(const QString & scriptPath);
private:
    void registerMainserver();
private:
    bool m_started{false};
};

#endif // RENDERTHREAD_H
