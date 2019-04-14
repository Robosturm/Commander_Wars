#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H

#include "game/gameanimation.h"
#include <QTimer>

class GameAnimationPower : public GameAnimation
{
    Q_OBJECT
public:
    GameAnimationPower(quint32 frameTime, QColor color, bool superpower, QString coid);
    virtual void restart() override;
    virtual void stop() override;
private:
    QTimer endTimer;
};

#endif // GAMEANIMATIONPOWER_H
