#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"

#include "game/battleanimation.h"
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
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);

    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("battle_back");
    setSize(pAnim->getSize());
    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityBack);
    addChild(pSprite);

    oxygine::spSprite pAtkTerrainSprite = loadTerrainSprite(pAtkUnit);
    setSpritePosition(pAtkTerrainSprite, pAtkUnit, pDefUnit);
    addChild(pAtkTerrainSprite);
    oxygine::spSprite pDefTerrainSprite = loadTerrainSprite(pDefUnit);
    setSpritePosition(pDefTerrainSprite, pDefUnit, pAtkUnit);
    addChild(pDefTerrainSprite);

    pAnim = pGameManager->getResAnim("battle_front");
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityFront);
    addChild(pSprite);

    Player* pPlayer = pAtkUnit->getOwner();
    CO* pCO = pPlayer->getCO(0);

    // create co info back image + co image
    pAnim = pGameManager->getResAnim("co_back");
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);
    m_AtkCO0 = new oxygine::Sprite();
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
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);
    m_AtkCO1 = new oxygine::Sprite();
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
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, - 30);
    addChild(pSprite);
    pCO = pPlayer->getCO(0);
    m_DefCO0 = new oxygine::Sprite();
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
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);
    pCO = pPlayer->getCO(1);
    m_DefCO1 = new oxygine::Sprite();
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
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, - 30);
    addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);

    // create health bar
    m_HealthBar0 = new oxygine::ColorRectSprite();
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
    m_HealthBar1 = new oxygine::ColorRectSprite();
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
    m_pAttackerAnimation = new BattleAnimationSprite(pAtkUnit, pAtkTerrain, BattleAnimationSprite::standingAnimation,
                                                                               GlobalUtils::roundUp(atkStartHp));
    setSpritePosition(m_pAttackerAnimation, pAtkUnit, pDefUnit);
    m_pAttackerAnimation->clear();
    m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pAtkUnit, pDefUnit, m_AtkWeapon);
    setSpritePosition(m_pAttackerAnimation, pAtkUnit, pDefUnit);

    addChild(m_pAttackerAnimation);
    m_pDefenderAnimation = new BattleAnimationSprite(pDefUnit, pDefTerrain, BattleAnimationSprite::standingAnimation,
                                                                               GlobalUtils::roundUp(defStartHp));
    if (!m_pAttackerAnimation->hasMoveInAnimation())
    {
        // skip move in
        currentState = AnimationProgress::WaitAfterIn;
    }
    setSpritePosition(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    m_pDefenderAnimation->clear();
    m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pDefUnit, pAtkUnit, m_DefWeapon);
    setSpritePosition(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    addChild(m_pDefenderAnimation);

//    // battleTimer
    battleTimer.setSingleShot(false);
    connect(&battleTimer, &QTimer::timeout, this, &BattleAnimation::nextAnimatinStep, Qt::QueuedConnection);
    nextAnimatinStep();
}

bool BattleAnimation::getIsLeft(Unit* pUnit1, Unit* pUnit2)
{
    if (pUnit1->getX() < pUnit2->getX())
    {
        return true;
    }
    else if ((pUnit1->getY() < pUnit2->getY()) &&
             (pUnit1->getX() == pUnit2->getX()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BattleAnimation::setSpritePosition(oxygine::spSprite pSprite, Unit* pUnit1, Unit* pUnit2)
{
    if (getIsLeft(pUnit1, pUnit2))
    {
        pSprite->setPosition(31, 34);
        // flip all childdren
        pSprite->flipActorsX(false);
    }
    else
    {
        pSprite->setPosition(162, 34);
        // flip all childdren
        pSprite->flipActorsX(true);
    }
}

oxygine::spSprite BattleAnimation::loadTerrainSprite(Unit* pUnit)
{
    oxygine::spSprite ret = new oxygine::Sprite();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
    pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase(), oxygine::ep_ignore_error);
    pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground(), oxygine::ep_ignore_error);
    pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground(), oxygine::ep_ignore_error);
    float speed = pUnit->getTerrainAnimationMoveSpeed();
    oxygine::spSlidingSprite pSprite = new oxygine::SlidingSprite();
    pSprite->setSize(spriteWidth, spriteHeigth);
    pSprite->setResAnim(pAnimBase);
    pSprite->setSpeed(speed);
    ret->addChild(pSprite);
    pSprite = new oxygine::SlidingSprite();
    pSprite->setSize(spriteWidth, spriteHeigth);
    pSprite->setResAnim(pAnimBack);
    pSprite->setSpeed(speed);
    ret->addChild(pSprite);
    pSprite = new oxygine::SlidingSprite();
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
        battleTimer.start();
    }
}

void BattleAnimation::stop()
{
    battleTimer.stop();
}

bool BattleAnimation::onFinished(bool skipping)
{
    if (m_pAttackerAnimation.get() != nullptr)
    {
        m_pAttackerAnimation->stopSound();
    }
    if (m_pDefenderAnimation.get() != nullptr)
    {
        m_pDefenderAnimation->stopSound();
    }
    return GameAnimation::onFinished(skipping);
}

void BattleAnimation::nextAnimatinStep()
{
    switch (currentState)
    {
        case AnimationProgress::MoveIn:
        {
            loadMoveInAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            loadStopAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            break;
        }
        case AnimationProgress::MoveStop:
        {
            m_pAttackerAnimation->startNextFrame();
            battleTimer.start(m_pAttackerAnimation->getStopDurationMS() / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
            break;
        }
        case AnimationProgress::WaitAfterIn:
        {
            battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            break;
        }
        case AnimationProgress::AttackerFire:
        {
            loadFireAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            break;
        }
        case AnimationProgress::AttackerImpact:
        {
            m_pAttackerAnimation->startNextFrame();
            // load impact
            loadImpactAnimation(m_pDefUnit, m_pAtkUnit, m_pDefenderAnimation, m_pAttackerAnimation,
                                m_HealthBar1, m_defEndHp, m_AtkWeapon, m_atkStartHp);
            break;
        }
        case AnimationProgress::AttackerDying:
        {
            qint32 count = m_pDefenderAnimation->getAnimationUnitCount();
            if (m_pDefenderAnimation->hasDyingAnimation() &&
                m_pDefenderAnimation->getUnitCount(count, GlobalUtils::roundUp(m_defEndHp)) < m_pDefenderAnimation->getUnitCount(count, GlobalUtils::roundUp(m_defStartHp)))
            {
                loadDyingAnimation(m_pDefUnit, m_pAtkUnit, m_pDefenderAnimation, m_defEndHp, m_DefWeapon);
                break;
            }
            else
            {
                currentState = static_cast<AnimationProgress>(static_cast<qint32>(currentState) + 1);
            }
        }
        case AnimationProgress::DefenderFire:
        {
            m_pDefenderAnimation->setHpRounded(GlobalUtils::roundUp(m_defEndHp));
            if (m_DefenderDamage >= 0)
            {
                loadFireAnimation(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
            }
            else
            {
                m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
                setSpritePosition(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit);
                currentState = AnimationProgress::WaitAfterBattle;
                battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            }
            break;
        }
        case AnimationProgress::DefenderImpact:
        {
            // remove firing frames
            m_pDefenderAnimation->startNextFrame();
            loadImpactAnimation(m_pAtkUnit, m_pDefUnit, m_pAttackerAnimation, m_pDefenderAnimation,
                                m_HealthBar0, m_atkEndHp, m_DefWeapon, m_defEndHp);
            break;
        }
        case AnimationProgress::DefenderDying:
        {
            qint32 count = m_pAttackerAnimation->getAnimationUnitCount();
            if (m_pAttackerAnimation->hasDyingAnimation() &&
                m_pAttackerAnimation->getUnitCount(count, GlobalUtils::roundUp(m_atkEndHp)) < m_pAttackerAnimation->getUnitCount(count, GlobalUtils::roundUp(m_atkStartHp)))
            {
                loadDyingAnimation(m_pAtkUnit, m_pDefUnit, m_pAttackerAnimation, m_atkEndHp, m_AtkWeapon);
                break;
            }
            else
            {
                currentState = static_cast<AnimationProgress>(static_cast<qint32>(currentState) + 1);
            }
        }
        case AnimationProgress::WaitAfterBattle:
        {
            m_pAttackerAnimation->setHpRounded(GlobalUtils::roundUp(m_atkEndHp));
            m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingFiredAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            setSpritePosition(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit);
            battleTimer.start(500 / Settings::getBattleAnimationSpeed());
            break;
        }
        case AnimationProgress::Finished:
        {
            break;
        }
    }
    currentState = static_cast<AnimationProgress>(static_cast<qint32>(currentState) + 1);
    
    if (currentState >= AnimationProgress::Finished)
    {
        onFinished(false);
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
    pSprite->loadAnimation(BattleAnimationSprite::moveInAnimation, pUnit1, pUnit2, weapon);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    battleTimer.start(pSprite->getMoveInDurationMS() / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
    
}

void BattleAnimation::loadStopAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::stopAnimation, pUnit1, pUnit2, weapon);
    setSpritePosition(pSprite, pUnit1, pUnit2);  
}


void BattleAnimation::loadFireAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    
    pSprite->loadAnimation(BattleAnimationSprite::fireAnimation, pUnit1, pUnit2, weapon);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    pSprite->loadAnimation(BattleAnimationSprite::standingFiredAnimation, pUnit1, pUnit2, weapon);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    battleTimer.start(pSprite->getFireDurationMS() / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
    
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
    // add impact image
    oxygine::ColorRectSprite::TweenColor tweenColor2(QColor(255, 0, 0));
    oxygine::spActor child = pSprite->getClipActor()->getFirstChild();
    while (child)
    {
        oxygine::spTween colorTween2 = oxygine::createTween(tweenColor2, oxygine::timeMS(static_cast<qint64>(500 / Settings::getBattleAnimationSpeed())), 1, true, oxygine::timeMS(100));
        child->addTween(colorTween2);
        child = child->getNextSibling();
    }
    if (currentState <= AnimationProgress::AttackerImpact)
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
    pSprite->setMaxUnitCount(pAttackerSprite->getMaxUnitCount());
    pSprite->loadAnimation(BattleAnimationSprite::impactAnimation, pUnit2, pUnit1, weapon, false);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    // restore sprite data
    pSprite->setMaxUnitCount(-1);
    pSprite->setHpRounded(curHp);
    battleTimer.start(pSprite->getImpactDurationMS(pUnit2) / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}


void BattleAnimation::loadDyingAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, float, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::dyingAnimation, pUnit1, pUnit2, weapon);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    battleTimer.start(pSprite->getDyingDurationMS() / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
    
}
