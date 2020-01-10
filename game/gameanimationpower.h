#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H

#include "game/gameanimation.h"
#include "game/GameEnums.h"
#include <QTimer>

class GameAnimationPower : public GameAnimation
{
    Q_OBJECT
public:
    static GameAnimationPower* createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, QString coid);

    virtual ~GameAnimationPower();

    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigRightClick();
public slots:
    void rightClick();
private:
    GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, QString coid);
    QTimer endTimer;

    static GameAnimationPower* m_pGameAnimationPower;
};

#endif // GAMEANIMATIONPOWER_H
