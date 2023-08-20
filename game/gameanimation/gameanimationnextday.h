#ifndef GAMEANIMATIONNEXTDAY_H
#define GAMEANIMATIONNEXTDAY_H

#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/player.h"

class GameAnimationNextDay;
using spGameAnimationNextDay = std::shared_ptr<GameAnimationNextDay>;

class GameAnimationNextDay final : public GameAnimation
{
    Q_OBJECT
public:
    explicit GameAnimationNextDay(GameMap* pMap, Player* pPlayer, quint32 frameTime, bool permanent, quint32 uptimeMs = 2000);
    ~GameAnimationNextDay() = default;
    virtual void restart() override;
    virtual void stop() override;
    Q_INVOKABLE virtual bool onFinished(bool skipping) override;
signals:
    void sigShowSaveAndExit();
    void sigRightClick();
public slots:
    void rightClick();
private:
    QTimer m_endTimer;
    bool m_permanent;
};

Q_DECLARE_INTERFACE(GameAnimationNextDay, "GameAnimationNextDay");

#endif // GAMEANIMATIONNEXTDAY_H
