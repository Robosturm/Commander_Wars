#ifndef GAMEANIMATIONFACTORY_H
#define GAMEANIMATIONFACTORY_H

#include <QObject>

#include <QVector>

#include "game/gamemap.h"

#include "game/gameanimation.h"

class GameAnimationWalk;
class GameAnimationCapture;
class GameAnimationDialog;
class GameAnimationPower;
class GameAnimationNextDay;
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
    /**
     * @brief clearAllAnimations
     */
    static void clearAllAnimations();
signals:
    void animationsFinished();
public slots:
    /**
     * @brief createAnimation creates a new animation and returns it
     * @param x in map coordinates
     * @param y in map coordinates
     * @param frameTime in ms
     * @return
     */
    static GameAnimation* createAnimation(quint32 x, quint32 y, quint32 frameTime = GameMap::frameTime, bool mapPosition = true);
    /**
     * @brief createBattleAnimation
     * @param pAtkTerrain
     * @param pAtkUnit
     * @param atkStartHp
     * @param atkEndHp
     * @param pDefTerrain
     * @param pDefUnit
     * @param defStartHp
     * @param defEndHp
     * @return
     */
    static GameAnimation* createBattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon);
    /**
     * @brief createAnimationPower
     * @param color
     * @param superpower
     * @param coid
     * @param frameTime
     * @return
     */
    static GameAnimationPower* createAnimationPower(QColor color, GameEnums::PowerMode powerMode, QString coid, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createGameAnimationDialog
     * @param text
     * @param coid
     * @param mood
     * @param color
     * @return
     */
    static GameAnimationDialog* createGameAnimationDialog(QString text, QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime = GameMap::frameTime);
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
    static GameAnimationCapture* createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints);
    /**
     * @brief createGameAnimationNextDay
     * @param pPlayer
     * @param frameTime
     * @return
     */
    static GameAnimationNextDay* createGameAnimationNextDay(Player* pPlayer, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief getAnimationCount
     * @return the current amount of running animations
     */
    static qint32 getAnimationCount();
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
