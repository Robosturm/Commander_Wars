#ifndef GAMEANIMATIONNEXTDAY_H
#define GAMEANIMATIONNEXTDAY_H

#include "game/gameanimation.h"

#include <QTimer>

class GameAnimationNextDay : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationNextDay(Player* pPlayer, quint32 frameTime);

    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigRightClick();
public slots:
    void rightClick();
private:
    QTimer endTimer;
};

#endif // GAMEANIMATIONNEXTDAY_H
