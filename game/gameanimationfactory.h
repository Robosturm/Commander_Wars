#ifndef GAMEANIMATIONFACTORY_H
#define GAMEANIMATIONFACTORY_H

#include <QObject>

#include <QVector>

#include "game/smartpointers.h"

#include "game/gamemap.h"

class GameAnimation;
class GameAnimationWalk;
class GameAnimationCapture;
class GameAction;

class GameAnimationFactory : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance
     * @return the singleton of the factory
     */
    static GameAnimationFactory* getInstance();
    /**
     * @brief queueAnimation queues the given animation by removing it from the execution
     * @param pGameAnimation
     */
    void queueAnimation(GameAnimation* pGameAnimation);
    /**
     * @brief startQueuedAnimation appends the given animation to the execution
     * @param pGameAnimation
     */
    void startQueuedAnimation(GameAnimation* pGameAnimation);
signals:

public slots:
    /**
     * @brief createAnimation creates a new animation and returns it
     * @param x in map coordinates
     * @param y in map coordinates
     * @param frameTime in ms
     * @return
     */
    static GameAnimation* createAnimation(quint32 x, quint32 y, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createWalkingAnimation creates a walking animation. This doesn't move the unit
     * @param pUnit the unit that will walk
     * @param movePath the path the unit will walk
     * @return
     */
    static GameAnimationWalk* createWalkingAnimation(Unit* pUnit, GameAction* pAction);
    /**
     * @brief createGameAnimationCapture
     * @return
     */
    static GameAnimationCapture* createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints);
    /**
     * @brief getAnimationCount
     * @return the current amount of running animations
     */
    static quint32 getAnimationCount();
    /**
     * @brief removeAnimation deletes the given animation
     * @param pAnimation pointer of the animation
     */
    static void removeAnimation(GameAnimation* pAnimation);
    /**
     * @brief removeAllAnimations removes all animations and calls onFinished of them
     */
    static void finishAllAnimations();
private:
    explicit GameAnimationFactory();

    static GameAnimationFactory* m_pInstance;
    static QVector<spGameAnimation> m_Animations;
};

#endif // GameAnimationFactory_H
