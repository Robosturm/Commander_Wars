#ifndef BATTLEANIMATION_H
#define BATTLEANIMATION_H

#include <QObject>
#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/gameanimation/battleanimationsprite.h"

#include "coreengine/LUPDATE_MACROS.h"

class BattleAnimation;
using spBattleAnimation = oxygine::intrusive_ptr<BattleAnimation>;

class BattleAnimation final : public GameAnimation
{
    Q_OBJECT
public:
    struct CoFace
    {
        oxygine::ResAnim* anim{nullptr};
        oxygine::spSprite coSprite;
    };
    static constexpr qint32 spriteWidth = 127;
    static constexpr qint32 healthBarSize = 20;
    static constexpr qint32 spriteHeigth = 192;

    static const short priorityBackground = -2;
    static const short priorityBack = -1;
    static const short priorityFront = 1;
    static const short priorityCOBack = 2;
    static const short priorityCO = 3;
    static const short priorityCOFront = 4;
    ENUM_CLASS AnimationProgress
    {
        MoveIn,
        MoveStop,
        WaitAfterIn,
        AttackerFire,
        AttackerFired,
        AttackerImpact,
        AttackerDying,
        DefenderFire,
        DefenderFired,
        DefenderImpact,
        DefenderDying,
        WaitAfterBattle,
        Finished,
    };

    BattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                    Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage,
                    GameMap* pMap);
    ~BattleAnimation() = default;
    virtual void restart() override;
    virtual void stop() override;
    /**
     * @brief onFinished called when the animation is finished
     */
    Q_INVOKABLE virtual bool onFinished(bool skipping) override;
    /**
     * @brief stopSound
     */
    Q_INVOKABLE void stopSound(bool forceStop = false);
    /**
     * @brief getPAtkUnit
     * @return
     */
    Q_INVOKABLE Unit *getAtkUnit() const;
    /**
     * @brief getPDefUnit
     * @return
     */
    Q_INVOKABLE Unit *getDefUnit() const;
    /**
     * @brief addScreenshake
     * @param startIntensity
     * @param decay
     * @param durationMs
     * @param shakePauseMs
     */
    Q_INVOKABLE void addBattleViewScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs = 0, qint32 shakePauseMs = 30);
private:
    /**
     * @brief getIsRight
     * @param pUnit1
     * @param pUnit2
     * @return
     */
    bool getIsLeft(Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief setSpritePosition
     * @param pSprite
     * @param pAtkUnit
     * @param pDefUnit
     */
    void setSpritePosition(oxygine::spActor pSprite, Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief setSpriteFlipped
     * @param pSprite
     * @param pUnit1
     * @param pUnit2
     */
    void setSpriteFlipped(oxygine::spSprite pSprite, Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief setCOMood
     * @param hp1
     * @param hp2
     */
    void setCOMood(CoFace & CoFace, float hp1, float hp2);
    /**
     * @brief loadTerrainSprite
     * @param pUnit
     * @return
     */
    oxygine::spSprite loadTerrainSprite(Unit* pUnit, Unit* pDefender);
    /**
     * @brief setHealthBarColor
     * @param pColorBar
     * @param hp
     */
    QColor getHealthBarColor(float hp);
    /**
     * @brief loadMoveInAnimation
     * @param pSprite
     */
    void loadMoveInAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon);
    /**
     * @brief loadStopAnimation
     * @param pSprite
     * @param pUnit1
     * @param pUnit2
     * @param weapon
     */
    void loadStopAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon);
    /**
     * @brief loadFireAnimation
     * @param pSprite
     */
    void loadFireAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon);
    /**
     * @brief loadFiredAnimation
     * @param pSprite
     * @param pUnit1
     * @param pUnit2
     * @param weapon
     */
    qint32 loadFiredAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon);
    /**
     * @brief loadImpactAnimation
     * @param pUnit1
     * @param pUnit2
     * @param pSprite
     * @param pColorRect
     * @param endHp
     * @param pCO0
     * @param pCO1
     * @param enemyHp
     */
    void loadImpactAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite & pSprite, spBattleAnimationSprite & pAttackerSprite,
                             oxygine::spBox9Sprite & pHealthbar, oxygine::spTextField & pHealthtext, float startHp, float endHp, qint32 weapon, float enemyHp);
    /**
     * @brief loadDyingAnimation
     * @param pUnit1
     * @param pUnit2
     * @param pSprite
     * @param endHp
     * @param weapon
     */
    void loadDyingAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite & pSprite, qint32 weapon);
    /**
     * @brief nextAnimatinStep
     */
    void nextAnimatinStep();
    /**
     * @brief loadDyingFadeoutAnimation
     * @param pSprite
     */
    void loadDyingFadeoutAnimation(spBattleAnimationSprite & pSprite);
    /**
     * @brief createBattleFrame
     * @param pAtkUnit
     * @param pDefUnit
     */
    void createBattleFrame(Unit* pAtkUnit, Unit* pDefUnit);
    /**
     * @brief createCoInfoBackImages
     * @param pAtkUnit
     * @param atkStartHp
     * @param pDefUnit
     * @param defStartHp
     */
    void createCoInfoBackImages(Unit* pAtkUnit, float atkStartHp, Unit* pDefUnit, float defStartHp);
    /**
     * @brief createCoInfoFrontImages
     * @param pAtkUnit
     * @param pDefUnit
     */
    void createCoInfoFrontImages(Unit* pAtkUnit, Unit* pDefUnit);
    /**
     * @brief createHealthbar
     * @param pAtkUnit
     * @param atkStartHp
     * @param pDefUnit
     * @param defStartHp
     */
    void createHealthbar(Unit* pAtkUnit, float atkStartHp, Unit* pDefUnit, float defStartHp);
    /**
     * @brief startBattleTimer
     * @param duration
     */
    void startBattleTimer(qint32 duration);
    /**
     * @brief addWeatherOverlay
     * @param actor
     * @param pTerrain
     */
    void addWeatherOverlay(oxygine::spActor & actor, Terrain* pTerrain);
private:
    QTimer m_battleTimer;

    CoFace m_AtkCO0;
    CoFace m_AtkCO1;
    CoFace m_DefCO0;
    CoFace m_DefCO1;

    oxygine::spBox9Sprite m_HealthBar0;
    oxygine::spBox9Sprite m_HealthBar1;
    oxygine::spTextField m_hpInfo0;
    oxygine::spTextField m_hpInfo1;

    spBattleAnimationSprite m_pAttackerAnimation;
    spBattleAnimationSprite m_pDefenderAnimation;

    oxygine::spActor m_pAttackerSprite;
    oxygine::spActor m_pDefenderSprite;


    Terrain* m_pAtkTerrain;
    Unit* m_pAtkUnit;
    float m_atkStartHp;
    float m_atkEndHp;
    qint32 m_AtkWeapon{0};
    Terrain* m_pDefTerrain;
    Unit* m_pDefUnit;
    float m_defStartHp;
    float m_defEndHp;
    qint32 m_DefWeapon{0};
    float m_DefenderDamage{-1};

    AnimationProgress m_currentState{AnimationProgress::MoveIn};
};

Q_DECLARE_INTERFACE(BattleAnimation, "BattleAnimation");

#endif // BATTLEANIMATION_H
