#ifndef TIMER_H
#define TIMER_H

#include "QTimer"

/**
 * @brief The Timer class timer with pause resume function -> note only use this timer as a singleshot timer.
 */
class Timer final : public QTimer
{
    Q_OBJECT
public:
    explicit Timer(QObject* pParent);
    ~Timer() = default;
public slots:
    void pause();
    void resume();
private:
};

#endif // TIMER_H
