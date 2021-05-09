#ifndef GAMEANIMATIONFACTORY_H
#define GAMEANIMATIONFACTORY_H

#include <QObject>
#include <QVector>

#include "game/gamemap.h"
#include "game/gameanimation/gameanimation.h"

class GameAnimationWalk;
class GameAnimationCapture;
class GameAnimationDialog;
class GameAnimationPower;
class GameAnimationNextDay;
class GameAction;
class CO;

class GameAnimationFactory;
using spGameAnimationFactory = oxygine::intrusive_ptr<GameAnimationFactory>;

class GameAnimationFactory : public QObject, public oxygine::ref_counter
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
    /**
     * @brief removeAnimationFromQueue
     * @param pAnimation
     */
    static void removeAnimationFromQueue(spGameAnimation pAnimation);
    /**
     * @brief removeAnimation
     * @param pAnimation
     * @param skipping
     * @param removeFromQueue
     */
    static void removeAnimation(GameAnimation* pAnimation, bool skipping, bool removeFromQueue);
signals:
    void animationsFinished();
public slots:
    /**
     * @brief createAnimation creates an animation and returns it
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
                                                Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage);
    /**
     * @brief createAnimationPower
     * @param color
     * @param superpower
     * @param coid
     * @param frameTime
     * @return
     */
    static GameAnimationPower* createAnimationPower(QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime = GameMap::frameTime);
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
    static GameAnimationNextDay* createGameAnimationNextDay(Player* pPlayer, quint32 frameTime = GameMap::frameTime, quint32 uptimeMs = 2000);
    /**
     * @brief getAnimationCount
     * @return the current amount of running animations
     */
    static qint32 getAnimationCount();
    /**
     * @brief getAnimation
     * @param index
     * @return
     */
    static GameAnimation* getAnimation(qint32 index);
    /**
     * @brief removeAnimation deletes the given animation
     * @param pAnimation pointer of the animation
     */
    static void removeAnimation(GameAnimation* pAnimation, bool skipping = false);
    /**
     * @brief removeAllAnimations removes all animations and calls onFinished of them
     */
    static void finishAllAnimations();
private:
    friend class oxygine::intrusive_ptr<GameAnimationFactory>;
    explicit GameAnimationFactory();

private:
    static spGameAnimationFactory m_pInstance;
    static QVector<spGameAnimation> m_Animations;
};

#endif // GameAnimationFactory_H
