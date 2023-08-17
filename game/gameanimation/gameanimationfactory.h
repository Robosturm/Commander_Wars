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
class BattleAnimation;

class GameAnimationFactory;
using spGameAnimationFactory = std::shared_ptr<GameAnimationFactory>;

class GameAnimationFactory final : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief getInstance
     * @return the singleton of the factory
     */
    static GameAnimationFactory* getInstance();
   virtual ~GameAnimationFactory() = default;
    void release();
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
    static void removeAnimation(spGameAnimation pAnimation, bool skipping, bool removeFromQueue);
signals:
    void animationsFinished();
public slots:
    /**
     * @brief removeAllAnimations removes all animations and calls onFinished of them and emits animationFinished
     */
    static void finishAllAnimationsWithEmitFinished();
public:
    /**
     * @brief createAnimation creates an animation and returns it
     * @param x in map coordinates
     * @param y in map coordinates
     * @param frameTime in ms
     * @return
     */
    Q_INVOKABLE static GameAnimation* createAnimation(GameMap* pMap, qint32 x, qint32 y, quint32 frameTime = GameMap::frameTime, bool mapPosition = true);
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
    Q_INVOKABLE static GameAnimation* createBattleAnimation(GameMap* pMap, Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                            Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage);
    /**
     * @brief createAnimationPower
     * @param color
     * @param superpower
     * @param coid
     * @param frameTime
     * @return
     */
    Q_INVOKABLE static GameAnimationPower* createAnimationPower(GameMap* pMap, QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createGameAnimationDialog
     * @param text
     * @param coid
     * @param mood
     * @param color
     * @return
     */
    Q_INVOKABLE static GameAnimationDialog* createGameAnimationDialog(GameMap* pMap, const QString text, const QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createWalkingAnimation creates a walking animation. This doesn't move the unit
     * @param pUnit the unit that will walk
     * @param movePath the path the unit will walk
     * @return
     */
    Q_INVOKABLE static GameAnimationWalk* createWalkingAnimation(GameMap* pMap, Unit* pUnit, GameAction* pAction);
    /**
     * @brief GameAnimationFactory::createWalkingAnimation
     * @param pMap
     * @param pUnit
     * @param movePath
     * @return
     */
    Q_INVOKABLE static GameAnimationWalk* createWalkingAnimationV2(GameMap* pMap, Unit* pUnit, const QVector<QPoint> movePath);
    /**
     * @brief createGameAnimationCapture
     * @return
     */
    Q_INVOKABLE static GameAnimationCapture* createGameAnimationCapture(GameMap* pMap, qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints);
    /**
     * @brief createGameAnimationNextDay
     * @param pPlayer
     * @param frameTime
     * @return
     */
    Q_INVOKABLE static GameAnimationNextDay* createGameAnimationNextDay(GameMap* pMap, Player* pPlayer, quint32 frameTime = GameMap::frameTime, quint32 uptimeMs = 2000);
    /**
     * @brief getAnimationCount
     * @return the current amount of running animations
     */
    Q_INVOKABLE static qint32 getAnimationCount();
    /**
     * @brief getAnimation
     * @param index
     * @return
     */
    Q_INVOKABLE static GameAnimation* getAnimation(qint32 index);
    /**
     * @brief removeAnimation deletes the given animation
     * @param pAnimation pointer of the animation
     */
    Q_INVOKABLE static void removeAnimation(GameAnimation* pAnimation, bool skipping = false);
    /**
     * @brief removeAllAnimations removes all animations and calls onFinished of them
     */
    Q_INVOKABLE static void finishAllAnimations();
    /**
     * @brief skipAllAnimations
     */
    Q_INVOKABLE static void skipAllAnimations();
    /**
     * @brief shouldSkipDialog
     * @param pDialogAnimation
     * @return
     */
    Q_INVOKABLE static bool shouldSkipDialog(GameAnimationDialog* pDialogAnimation);
    /**
     * @brief shouldSkipCapture
     * @param pGameAnimationCapture
     * @return
     */
    Q_INVOKABLE static bool shouldSkipCapture(GameAnimationCapture* pGameAnimationCapture);
    /**
     * @brief shouldSkipBattleAnimation
     * @param pBattleAnimation
     * @return
     */
    Q_INVOKABLE static bool shouldSkipBattleAnimation(BattleAnimation* pBattleAnimation);
    /**
     * @brief shouldSkipOtherAnimation
     * @param pBattleAnimation
     * @return
     */
    Q_INVOKABLE static bool shouldSkipOtherAnimation(GameAnimation* pBattleAnimation);
    /**
      * @brief shouldSkipDay2Day
      * @param pGameAnimationNextDay
      * @return
      */
    Q_INVOKABLE static bool shouldSkipDay2Day(GameAnimationNextDay* pGameAnimationNextDay);
    /**
      * @brief shouldSkipMovement
      * @param pGameAnimationNextDay
      * @return
      */
    Q_INVOKABLE static bool shouldSkipMovement(GameAnimationWalk* pGameAnimationWalk);
    /**
      * @brief printActiveAnimations
      */
    Q_INVOKABLE static void printActiveAnimations();
private:
    friend class MemoryManagement;
    explicit GameAnimationFactory();
    /**
     * @brief createOverworldBattleAnimation
     * @param pAtkTerrain
     * @param pAtkUnit
     * @param atkStartHp
     * @param atkEndHp
     * @param atkWeapon
     * @param pDefTerrain
     * @param pDefUnit
     * @param defStartHp
     * @param defEndHp
     * @param defWeapon
     * @param defenderDamage
     * @return
     */
    static GameAnimation* createOverworldBattleAnimation(GameMap* pMap, Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                         Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage);
private:
    static spGameAnimationFactory m_pInstance;
    static QVector<spGameAnimation> m_Animations;
};

Q_DECLARE_INTERFACE(GameAnimationFactory, "GameAnimationFactory");

#endif // GameAnimationFactory_H
