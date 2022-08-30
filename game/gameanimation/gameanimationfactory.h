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
    static GameAnimation* createAnimation(GameMap* pMap, qint32 x, qint32 y, quint32 frameTime = GameMap::frameTime, bool mapPosition = true);
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
    static GameAnimation* createBattleAnimation(GameMap* pMap, Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage);
    /**
     * @brief createAnimationPower
     * @param color
     * @param superpower
     * @param coid
     * @param frameTime
     * @return
     */
    static GameAnimationPower* createAnimationPower(GameMap* pMap, QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createGameAnimationDialog
     * @param text
     * @param coid
     * @param mood
     * @param color
     * @return
     */
    static GameAnimationDialog* createGameAnimationDialog(GameMap* pMap, const QString & text, const QString & coid, GameEnums::COMood mood, QColor color, quint32 frameTime = GameMap::frameTime);
    /**
     * @brief createWalkingAnimation creates a walking animation. This doesn't move the unit
     * @param pUnit the unit that will walk
     * @param movePath the path the unit will walk
     * @return
     */
    static GameAnimationWalk* createWalkingAnimation(GameMap* pMap, Unit* pUnit, GameAction* pAction);
    /**
     * @brief GameAnimationFactory::createWalkingAnimation
     * @param pMap
     * @param pUnit
     * @param movePath
     * @return
     */
    static GameAnimationWalk* createWalkingAnimationV2(GameMap* pMap, Unit* pUnit, const QVector<QPoint> & movePath);
    /**
     * @brief createGameAnimationCapture
     * @return
     */
    static GameAnimationCapture* createGameAnimationCapture(GameMap* pMap, qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints);
    /**
     * @brief createGameAnimationNextDay
     * @param pPlayer
     * @param frameTime
     * @return
     */
    static GameAnimationNextDay* createGameAnimationNextDay(GameMap* pMap, Player* pPlayer, quint32 frameTime = GameMap::frameTime, quint32 uptimeMs = 2000);
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
    /**
     * @brief removeAllAnimations removes all animations and calls onFinished of them and emits animationFinished
     */
    static void finishAllAnimationsWithEmitFinished();
    /**
     * @brief skipAllAnimations
     */
     static void skipAllAnimations();
    /**
     * @brief shouldSkipDialog
     * @param pDialogAnimation
     * @return
     */
     static bool shouldSkipDialog(GameAnimationDialog* pDialogAnimation);
    /**
     * @brief shouldSkipCapture
     * @param pGameAnimationCapture
     * @return
     */
     static bool shouldSkipCapture(GameAnimationCapture* pGameAnimationCapture);
    /**
     * @brief shouldSkipBattleAnimation
     * @param pBattleAnimation
     * @return
     */
     static bool shouldSkipBattleAnimation(BattleAnimation* pBattleAnimation);
    /**
     * @brief shouldSkipOtherAnimation
     * @param pBattleAnimation
     * @return
     */
     static bool shouldSkipOtherAnimation(GameAnimation* pBattleAnimation);
     /**
      * @brief shouldSkipDay2Day
      * @param pGameAnimationNextDay
      * @return
      */
     static bool shouldSkipDay2Day(GameAnimationNextDay* pGameAnimationNextDay);
     /**
      * @brief shouldSkipMovement
      * @param pGameAnimationNextDay
      * @return
      */
     static bool shouldSkipMovement(GameAnimationWalk* pGameAnimationWalk);
     /**
      * @brief printActiveAnimations
      */
     static void printActiveAnimations();
private:
    friend class oxygine::intrusive_ptr<GameAnimationFactory>;
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
