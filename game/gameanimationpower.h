#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H

#include "game/gameanimation.h"
#include "game/GameEnums.h"
#include <QTimer>

class GameAnimationPower : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, QString coid);
    virtual void restart() override;
    virtual void stop() override;

signals:
    void sigRightClick();
public slots:
    void rightClick();
private:
    QTimer endTimer;
};

#endif // GAMEANIMATIONPOWER_H
