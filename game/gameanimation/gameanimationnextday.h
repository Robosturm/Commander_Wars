#ifndef GAMEANIMATIONNEXTDAY_H
#define GAMEANIMATIONNEXTDAY_H

#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/player.h"

class GameAnimationNextDay;
using spGameAnimationNextDay = oxygine::intrusive_ptr<GameAnimationNextDay>;

class GameAnimationNextDay : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationNextDay(Player* pPlayer, quint32 frameTime, bool permanent, quint32 uptimeMs = 2000);

    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigShowSaveAndExit();
    void sigRightClick();
public slots:
    void rightClick();
    virtual bool onFinished(bool skipping) override;
private:
    QTimer m_endTimer;
    bool m_permanent;
};

#endif // GAMEANIMATIONNEXTDAY_H
