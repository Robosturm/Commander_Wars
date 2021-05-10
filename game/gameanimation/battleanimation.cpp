#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "coreengine/tweens/tweenscreenshake.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"

#include "game/gameanimation/battleanimation.h"
#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

BattleAnimation::BattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                 Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_pAtkTerrain(pAtkTerrain),
      m_pAtkUnit(pAtkUnit),
      m_atkStartHp(atkStartHp),
      m_atkEndHp(atkEndHp),
      m_AtkWeapon(atkWeapon),
      m_pDefTerrain(pDefTerrain),
      m_pDefUnit(pDefUnit),
      m_defStartHp(defStartHp),
      m_defEndHp(defEndHp),
      m_DefWeapon(defWeapon),
      m_DefenderDamage(defenderDamage)
{
    setObjectName("BattleAnimation");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("battle_back");
    if (pAnim != nullptr)
    {
        setSize(pAnim->getSize());
    }
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityBack);
    addChild(pSprite);

    oxygine::spClipRectActor pAttackerClipRect = oxygine::spClipRectActor::create();
    pAttackerClipRect->setSize(127, 192);
    setSpritePosition(pAttackerClipRect, pAtkUnit, pDefUnit);
    m_pAttackerSprite = oxygine::spActor::create();
    pAttackerClipRect->addChild(m_pAttackerSprite);
    addChild(pAttackerClipRect);

    oxygine::spClipRectActor pDefenderClipRect = oxygine::spClipRectActor::create();
    pDefenderClipRect->setSize(127, 192);
    setSpritePosition(pDefenderClipRect, pDefUnit, pAtkUnit);
    m_pDefenderSprite = oxygine::spActor::create();
    pDefenderClipRect->addChild(m_pDefenderSprite);
    addChild(pDefenderClipRect);

    oxygine::spSprite pAtkTerrainSprite = loadTerrainSprite(pAtkUnit);
    setSpriteFlipped(pAtkTerrainSprite, pAtkUnit, pDefUnit);
    m_pAttackerSprite->addChild(pAtkTerrainSprite);
    oxygine::spSprite pDefTerrainSprite = loadTerrainSprite(pDefUnit);
    setSpriteFlipped(pDefTerrainSprite, pDefUnit, pAtkUnit);
    m_pDefenderSprite->addChild(pDefTerrainSprite);

    pAnim = pGameManager->getResAnim("battle_front");
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityFront);
    addChild(pSprite);

    Player* pPlayer = pAtkUnit->getOwner();
    CO* pCO = pPlayer->getCO(0);

    // create co info back image + co image
    pAnim = pGameManager->getResAnim("co_back");
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);
    m_AtkCO0 = oxygine::spSprite::create();
    float coScale = 22.0f / 24.0f;
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        oxygine::ResAnim* pAnimCO = pCO->getResAnim(resAnim);
        m_AtkCO0->setResAnim(pAnimCO);
        setCOMood(m_AtkCO0, atkStartHp, defStartHp);
        if (getIsLeft(pAtkUnit, pDefUnit))
        {
            m_AtkCO0->setPosition(-18, -13);
        }
        else
        {
            m_AtkCO0->setPosition(getWidth() - 28, -13);
        }
        m_AtkCO0->setPriority(priorityCO);
        m_AtkCO0->setScale(coScale);
        addChild(m_AtkCO0);
    }
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);
    m_AtkCO1 = oxygine::spSprite::create();
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        oxygine::ResAnim* pAnimCO = pCO->getResAnim(resAnim);
        m_AtkCO1->setResAnim(pAnimCO);
        setCOMood(m_AtkCO1, atkStartHp, defStartHp);
        if (getIsLeft(pAtkUnit, pDefUnit))
        {
            m_AtkCO1->setPosition(-18, getHeight() - 28);
        }
        else
        {
            m_AtkCO1->setPosition(getWidth() - 28, getHeight() - 28);
        }
        m_AtkCO1->setPriority(priorityCO);
        m_AtkCO1->setScale(coScale);
        addChild(m_AtkCO1);
    }
    pPlayer = pDefUnit->getOwner();
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, - 30);
    addChild(pSprite);
    pCO = pPlayer->getCO(0);
    m_DefCO0 = oxygine::spSprite::create();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        oxygine::ResAnim* pAnimCO = pCO->getResAnim(resAnim);
        m_DefCO0->setResAnim(pAnimCO);
        setCOMood(m_DefCO0, defStartHp, atkStartHp);
        if (getIsLeft(pDefUnit, pAtkUnit))
        {
            m_DefCO0->setPosition(-18, -13);
        }
        else
        {
            m_DefCO0->setPosition(getWidth() - 28, -13);
        }
        m_DefCO0->setPriority(priorityCO);
        m_DefCO0->setScale(coScale);
        addChild(m_DefCO0);
    }
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);
    pCO = pPlayer->getCO(1);
    m_DefCO1 = oxygine::spSprite::create();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        oxygine::ResAnim* pAnimCO = pCO->getResAnim(resAnim);
        m_DefCO1->setResAnim(pAnimCO);
        setCOMood(m_DefCO1, defStartHp, atkStartHp);
        if (getIsLeft(pDefUnit, pAtkUnit))
        {
            m_DefCO1->setPosition(-18, getHeight() - 28);
        }
        else
        {
            m_DefCO1->setPosition(getWidth() - 28, getHeight() - 28);
        }
        m_DefCO1->setPriority(priorityCO);
        m_DefCO1->setScale(coScale);
        addChild(m_DefCO1);
    }
    // create co info front image
    pAnim = pGameManager->getResAnim("co_front");
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, - 30);
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);

    // create health bar
    m_HealthBar0 = oxygine::spColorRectSprite::create();
    m_HealthBar0->setSize(spriteWidth * atkStartHp / Unit::MAX_UNIT_HP, 9);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        m_HealthBar0->setPosition(31, 25);
    }
    else
    {
        m_HealthBar0->setPosition(162, 25);
    }
    m_HealthBar0->setColor(getHealthBarColor(atkStartHp));
    addChild(m_HealthBar0);
    m_HealthBar1 = oxygine::spColorRectSprite::create();
    m_HealthBar1->setSize(spriteWidth * defStartHp / Unit::MAX_UNIT_HP, 9);
    if (getIsLeft(pDefUnit, pAtkUnit))
    {
        m_HealthBar1->setPosition(31, 25);
    }
    else
    {
        m_HealthBar1->setPosition(162, 25);
    }
    m_HealthBar1->setColor(getHealthBarColor(defStartHp));
    addChild(m_HealthBar1);

    // dummy impl for standing units
    m_pAttackerAnimation = spBattleAnimationSprite::create(pAtkUnit, pAtkTerrain, BattleAnimationSprite::standingAnimation,
                                                     GlobalUtils::roundUp(atkStartHp));
    m_pAttackerAnimation->setDyingStartHp(atkStartHp);
    m_pAttackerAnimation->setDyingEndHp(atkEndHp);
    setSpriteFlipped(m_pAttackerAnimation, pAtkUnit, pDefUnit);
    m_pAttackerAnimation->clear();
    m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pAtkUnit, pDefUnit, m_AtkWeapon);
    setSpriteFlipped(m_pAttackerAnimation, pAtkUnit, pDefUnit);

    m_pAttackerSprite->addChild(m_pAttackerAnimation);
    m_pDefenderAnimation = spBattleAnimationSprite::create(pDefUnit, pDefTerrain, BattleAnimationSprite::standingAnimation,
                                                     GlobalUtils::roundUp(defStartHp));
    m_pDefenderAnimation->setDyingStartHp(defStartHp);
    m_pDefenderAnimation->setDyingEndHp(defEndHp);
    if (!m_pAttackerAnimation->hasMoveInAnimation(pAtkUnit, pDefUnit, atkWeapon))
    {
        // skip move in
        m_currentState = AnimationProgress::WaitAfterIn;
    }
    setSpriteFlipped(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    m_pDefenderAnimation->clear();
    m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pDefUnit, pAtkUnit, m_DefWeapon);
    setSpriteFlipped(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    m_pDefenderSprite->addChild(m_pDefenderAnimation);

    //    // battleTimer
    m_battleTimer.setSingleShot(false);
    connect(&m_battleTimer, &QTimer::timeout, this, &BattleAnimation::nextAnimatinStep, Qt::QueuedConnection);
    nextAnimatinStep();
}

bool BattleAnimation::getIsLeft(Unit* pUnit1, Unit* pUnit2)
{
    if (pUnit1->Unit::getX() < pUnit2->Unit::getX())
    {
        return true;
    }
    else if ((pUnit1->Unit::getY() < pUnit2->Unit::getY()) &&
             (pUnit1->Unit::getX() == pUnit2->Unit::getX()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BattleAnimation::setSpritePosition(oxygine::spActor pSprite, Unit* pUnit1, Unit* pUnit2)
{
    if (getIsLeft(pUnit1, pUnit2))
    {
        pSprite->setPosition(31, 34);
    }
    else
    {
        pSprite->setPosition(162, 34);
    }
}

void BattleAnimation::setSpriteFlipped(oxygine::spSprite pSprite, Unit* pUnit1, Unit* pUnit2)
{
    if (getIsLeft(pUnit1, pUnit2))
    {
        // flip all childdren
        pSprite->flipActorsX(false);
    }
    else
    {
        // flip all childdren
        pSprite->flipActorsX(true);
    }
}

oxygine::spSprite BattleAnimation::loadTerrainSprite(Unit* pUnit)
{
    oxygine::spSprite ret = oxygine::spSprite::create();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
    pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase(), oxygine::ep_ignore_error);
    pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
    pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
    float speed = pUnit->getTerrainAnimationMoveSpeed();
    oxygine::spSlidingSprite pSprite = oxygine::spSlidingSprite::create();
    pSprite->setSize(spriteWidth, spriteHeigth);
    pSprite->setResAnim(pAnimBase);
    pSprite->setSpeed(speed);
    ret->addChild(pSprite);
    pSprite = oxygine::spSlidingSprite::create();
    pSprite->setSize(spriteWidth, spriteHeigth);
    pSprite->setResAnim(pAnimBack);
    pSprite->setSpeed(speed);
    ret->addChild(pSprite);
    pSprite = oxygine::spSlidingSprite::create();
    pSprite->setSize(spriteWidth, spriteHeigth);
    pSprite->setResAnim(pAnimFore);
    ret->addChild(pSprite);
    pSprite->setSpeed(speed);
    
    return ret;
}

void BattleAnimation::setCOMood(oxygine::spSprite pSprite, float hp1, float hp2)
{
    GameEnums::COMood mood = GameEnums::COMood_Normal;
    if (hp1 - hp2 > 3.0f)
    {
        mood = GameEnums::COMood_Happy;
    }
    else if (hp2 - hp1 > 3.0f)
    {
        mood = GameEnums::COMood_Sad;
    }
    else
    {
        mood = GameEnums::COMood_Normal;
    }
    pSprite->setColumn(static_cast<qint32>(mood));
}

QColor BattleAnimation::getHealthBarColor(float hp)
{
    float divider = hp / Unit::MAX_UNIT_HP;
    if (divider > 2.0f / 3.0f)
    {
        return QColor(0, 255, 0, 255);
    }
    else if (divider > 1.0f / 3.0f)
    {
        return QColor(255, 128, 0, 255);
    }
    else
    {
        return QColor(255, 0, 0, 255);
    }
}

void BattleAnimation::restart()
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(this);
        m_battleTimer.start();
    }
}

void BattleAnimation::stop()
{
    m_battleTimer.stop();
}

bool BattleAnimation::onFinished(bool skipping)
{
    stopSound();
    return GameAnimation::onFinished(skipping);
}

void BattleAnimation::stopSound()
{
    if (m_pAttackerAnimation.get() != nullptr)
    {
        m_pAttackerAnimation->stopSound();
    }
    if (m_pDefenderAnimation.get() != nullptr)
    {
        m_pDefenderAnimation->stopSound();
    }
}

void BattleAnimation::nextAnimatinStep()
{
    switch (m_currentState)
    {
        case AnimationProgress::MoveIn:
        {
            stopSound();
            loadMoveInAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            loadStopAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            m_pAttackerAnimation->startNextFrame();
            break;
        }
        case AnimationProgress::MoveStop:
        {
            m_pAttackerAnimation->startNextFrame();
            m_battleTimer.start(m_pAttackerAnimation->getStopDurationMS(m_pAtkUnit, m_pDefUnit, m_AtkWeapon) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
            break;
        }
        case AnimationProgress::WaitAfterIn:
        {
            stopSound();
            m_battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            break;
        }
        case AnimationProgress::AttackerFire:
        {
            stopSound();
            loadFireAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            break;
        }
        case AnimationProgress::AttackerImpact:
        {
            loadFiredAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            // load impact
            loadImpactAnimation(m_pDefUnit, m_pAtkUnit, m_pDefenderAnimation, m_pAttackerAnimation,
                                m_HealthBar1, m_defEndHp, m_AtkWeapon, m_atkStartHp);
            break;
        }
        case AnimationProgress::AttackerDying:
        {
            stopSound();
            qint32 count = m_pDefenderAnimation->getAnimationUnitCount();
            if (m_pDefenderAnimation->getUnitCount(count, GlobalUtils::roundUp(m_defEndHp)) < m_pDefenderAnimation->getUnitCount(count, GlobalUtils::roundUp(m_defStartHp)))
            {
                if (m_pDefenderAnimation->hasDyingAnimation())
                {
                    loadDyingAnimation(m_pDefUnit, m_pAtkUnit, m_pDefenderAnimation, m_DefWeapon);
                }
                else
                {
                    loadDyingFadeoutAnimation(m_pDefenderAnimation);
                }
                break;
            }
            else
            {
                m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
            }
        }
        case AnimationProgress::DefenderFire:
        {
            stopSound();
            m_pDefenderAnimation->setHpRounded(GlobalUtils::roundUp(m_defEndHp));
            if (m_DefenderDamage >= 0)
            {
                loadFireAnimation(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
            }
            else
            {
                m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
                setSpriteFlipped(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit);
                m_currentState = AnimationProgress::WaitAfterBattle;
                m_battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            }
            break;
        }
        case AnimationProgress::DefenderImpact:
        {
            // remove firing frames
            loadFiredAnimation(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
            loadImpactAnimation(m_pAtkUnit, m_pDefUnit, m_pAttackerAnimation, m_pDefenderAnimation,
                                m_HealthBar0, m_atkEndHp, m_DefWeapon, m_defEndHp);
            break;
        }
        case AnimationProgress::DefenderDying:
        {
            stopSound();
            qint32 count = m_pAttackerAnimation->getAnimationUnitCount();
            if (m_pAttackerAnimation->getUnitCount(count, GlobalUtils::roundUp(m_atkEndHp)) < m_pAttackerAnimation->getUnitCount(count, GlobalUtils::roundUp(m_atkStartHp)))
            {
                if (m_pAttackerAnimation->hasDyingAnimation())
                {
                    loadDyingAnimation(m_pAtkUnit, m_pDefUnit, m_pAttackerAnimation, m_AtkWeapon);
                }
                else
                {
                    loadDyingFadeoutAnimation(m_pAttackerAnimation);
                }
                break;
            }
            else
            {
                m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
            }
        }
        case AnimationProgress::WaitAfterBattle:
        {
            m_pAttackerAnimation->setHpRounded(GlobalUtils::roundUp(m_atkEndHp));
            m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingFiredAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            setSpriteFlipped(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit);
            m_battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            break;
        }
        case AnimationProgress::Finished:
        {
            break;
        }
    }
    m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
    
    if (m_currentState >= AnimationProgress::Finished)
    {
        BattleAnimation::onFinished(false);
    }
}

Unit *BattleAnimation::getDefUnit() const
{
    return m_pDefUnit;
}

Unit *BattleAnimation::getAtkUnit() const
{
    return m_pAtkUnit;
}

void BattleAnimation::loadMoveInAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    pSprite->clear();
    pSprite->setStartWithFraming(true);
    pSprite->loadAnimation(BattleAnimationSprite::moveInAnimation, pUnit1, pUnit2, weapon, true, false);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
    m_battleTimer.start(pSprite->getMoveInDurationMS(pUnit1, pUnit2, weapon) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
    
}

void BattleAnimation::loadStopAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::stopAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
}

void BattleAnimation::loadFireAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{    
    pSprite->loadAnimation(BattleAnimationSprite::fireAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
    m_battleTimer.start(pSprite->getFireDurationMS(pUnit1, pUnit2, weapon) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimation::loadImpactAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, spBattleAnimationSprite pAttackerSprite,
                                          oxygine::spColorRectSprite pColorRect, float endHp, qint32 weapon, float enemyHp)
{    
    if (endHp < 0.0f)
    {
        endHp = 0.0f;
    }
    oxygine::ColorRectSprite::TweenColor tweenColor(getHealthBarColor(endHp));
    oxygine::spTween colorTween = oxygine::createTween(tweenColor, oxygine::timeMS(static_cast<qint64>(800 / Settings::getBattleAnimationSpeed())));
    pColorRect->addTween(colorTween);
    oxygine::spTween posTween = oxygine::createTween(oxygine::Actor::TweenWidth(spriteWidth * endHp / Unit::MAX_UNIT_HP), oxygine::timeMS(static_cast<qint64>(800 / Settings::getBattleAnimationSpeed())));
    pColorRect->addTween(posTween);
    if (m_currentState <= AnimationProgress::AttackerImpact)
    {
        setCOMood(m_AtkCO0, m_atkStartHp, m_defEndHp);
        setCOMood(m_AtkCO1, m_atkStartHp, m_defEndHp);
        setCOMood(m_DefCO0, m_defEndHp, m_atkStartHp);
        setCOMood(m_DefCO1, m_defEndHp, m_atkStartHp);
    }
    else
    {
        setCOMood(m_AtkCO0, m_atkEndHp, m_defEndHp);
        setCOMood(m_AtkCO1, m_atkEndHp, m_defEndHp);
        setCOMood(m_DefCO0, m_defEndHp, m_atkEndHp);
        setCOMood(m_DefCO1, m_defEndHp, m_atkEndHp);
    }
    // buffer hp
    qint32 curHp = pSprite->getHpRounded();
    pSprite->setHpRounded(GlobalUtils::roundUp(enemyHp));
    pSprite->setInvertStartPosition(true);
    pSprite->setMaxUnitCount(pAttackerSprite->getMaxUnitCount());
    pSprite->loadAnimation(BattleAnimationSprite::impactUnitOverlayAnimation, pUnit2, pUnit1, weapon, false, false);
    pSprite->loadAnimation(BattleAnimationSprite::impactAnimation, pUnit2, pUnit1, weapon, false, false);
    pSprite->startNextFrame();
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
    // restore sprite data
    pSprite->setMaxUnitCount(-1);
    pSprite->setHpRounded(curHp);
    pSprite->setInvertStartPosition(false);
    m_battleTimer.start(pSprite->getImpactDurationMS(pUnit1, pUnit2, weapon) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimation::loadFiredAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::standingFiredAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
}

void BattleAnimation::loadDyingAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::dyingAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
    m_battleTimer.start(pSprite->getDyingDurationMS(pUnit1, pUnit2, weapon) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimation::loadDyingFadeoutAnimation(spBattleAnimationSprite pSprite)
{
    constexpr qint32 fadeoutTime = 1700;
    qint32 sleep = pSprite->loadDyingFadeOutAnimation(fadeoutTime - 400);
    m_battleTimer.start((fadeoutTime + sleep) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimation::addBattleViewScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    oxygine::spTween tween = oxygine::createTween(TweenScreenshake(startIntensity, decay / Settings::getBattleAnimationSpeed(), oxygine::timeMS(shakePauseMs)),
                                                  oxygine::timeMS(static_cast<qint64>(durationMs / Settings::getBattleAnimationSpeed())), 1, false, oxygine::timeMS(static_cast<qint64>(delayMs / Settings::getBattleAnimationSpeed())));
    addTween(tween);
}
