#ifndef GAMEANIMATIONCAPTURE_H
#define GAMEANIMATIONCAPTURE_H

#include "game/gameanimation.h"

class GameAnimationCapture : public GameAnimation
{
    Q_OBJECT
public:
    explicit GameAnimationCapture(qint32 startPoints, qint32 endPoints);

public slots:
    void addBackgroundSprite(QString spriteID);
private:
    qint32 m_startPoints{0};
    qint32 m_endPoints{0};
};

#endif // GAMEANIMATIONCAPTURE_H
