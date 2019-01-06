#ifndef GAMEANIMATION_H
#define GAMEANIMATION_H

#include <QObject>

#include <QVector>

#include "oxygine-framework.h"

#include "game/smartpointers.h"

class GameAnimation : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit GameAnimation(quint32 frameTime);

signals:
    void sigFinished();
public slots:
    /**
     * @brief queueAnimation
     * @param pGameAnimation
     */
    void queueAnimation(GameAnimation* pGameAnimation);
    /**
     * @brief addSprite
     * @param spriteID
     */
    void addSprite(QString spriteID, float offsetX, float offsetY);
    /**
     * @brief onFinished called when the animation is finished
     */
    virtual void onFinished();
private:
    quint32 m_frameTime{0};
    QVector<GameAnimation*> m_QueuedAnimations;
};

#endif // GAMEANIMATION_H
