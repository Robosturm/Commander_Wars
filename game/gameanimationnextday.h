#ifndef GAMEANIMATIONNEXTDAY_H
#define GAMEANIMATIONNEXTDAY_H

#include "game/gameanimation.h"

#include <QTimer>

#include "player.h"

class GameAnimationNextDay : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationNextDay(Player* pPlayer, quint32 frameTime, bool permanent);

    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigRightClick();
public slots:
    void rightClick();
    virtual bool onFinished() override;
private:
    QTimer endTimer;
    bool m_permanent;
};

#endif // GAMEANIMATIONNEXTDAY_H
