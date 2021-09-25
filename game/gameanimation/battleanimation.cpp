#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "resource_management/gamemanager.h"
#include "resource_management/cospritemanager.h"
#include "3rd_party/oxygine-framework/oxygine/tween/tweentogglevisibility.h"

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
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    createBattleFrame(pAtkUnit, pDefUnit);
    createCoInfoBackImages(pAtkUnit, atkStartHp, pDefUnit, defStartHp);
    createCoInfoFrontImages(pAtkUnit, pDefUnit);
    createHealthbar(pAtkUnit, atkStartHp, pDefUnit, defStartHp);

    // load attacker sprite
    m_pAttackerAnimation = spBattleAnimationSprite::create(spUnit(pAtkUnit), pAtkTerrain, BattleAnimationSprite::standingAnimation,
                                                           GlobalUtils::roundUp(atkStartHp));
    m_pAttackerAnimation->setDyingStartHp(atkStartHp);
    m_pAttackerAnimation->setDyingEndHp(atkEndHp);
    m_pAttackerAnimation->setFireHp(atkStartHp);
    // load defender sprite
    m_pDefenderAnimation = spBattleAnimationSprite::create(spUnit(pDefUnit), pDefTerrain, BattleAnimationSprite::standingAnimation,
                                                           GlobalUtils::roundUp(defStartHp));
    m_pDefenderAnimation->setDyingStartHp(defStartHp);
    m_pDefenderAnimation->setDyingEndHp(defEndHp);
    m_pDefenderAnimation->setFireHp(defEndHp);
    // set enemy sprites
    m_pDefenderAnimation->setEnemySprite(m_pAttackerAnimation.get());
    m_pAttackerAnimation->setEnemySprite(m_pDefenderAnimation.get());
    // load terrains
    oxygine::spSprite pAtkTerrainSprite = loadTerrainSprite(pAtkUnit, pDefUnit);
    setSpriteFlipped(pAtkTerrainSprite, pAtkUnit, pDefUnit);
    m_pAttackerSprite->addChild(pAtkTerrainSprite);
    m_pAttackerAnimation->setBackgroundSprite(pAtkTerrainSprite);
    oxygine::spSprite pDefTerrainSprite = loadTerrainSprite(pDefUnit, pAtkUnit);
    setSpriteFlipped(pDefTerrainSprite, pDefUnit, pAtkUnit);
    m_pDefenderSprite->addChild(pDefTerrainSprite);
    m_pDefenderAnimation->setBackgroundSprite(pDefTerrainSprite);
    // load initial attacker state
    m_pAttackerSprite->addChild(m_pAttackerAnimation);
    setSpriteFlipped(m_pAttackerAnimation, pAtkUnit, pDefUnit);
    m_pAttackerAnimation->clear();
    m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pAtkUnit, pDefUnit, m_AtkWeapon);
    setSpriteFlipped(m_pAttackerAnimation, pAtkUnit, pDefUnit);
    if (!m_pAttackerAnimation->hasMoveInAnimation(pAtkUnit, pDefUnit, atkWeapon))
    {
        // skip move in
        m_currentState = AnimationProgress::WaitAfterIn;
    }
    // load initial defender state
    setSpriteFlipped(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    m_pDefenderAnimation->clear();
    m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation, pDefUnit, pAtkUnit, m_DefWeapon);
    setSpriteFlipped(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    m_pDefenderSprite->addChild(m_pDefenderAnimation);

    // battleTimer
    m_battleTimer.setSingleShot(false);
    connect(&m_battleTimer, &QTimer::timeout, this, &BattleAnimation::nextAnimatinStep, Qt::QueuedConnection);
    nextAnimatinStep();
}

void BattleAnimation::createBattleFrame(Unit* pAtkUnit, Unit* pDefUnit)
{
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("battle_back+mask", oxygine::ep_ignore_error);
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityBack);
    pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    addChild(pSprite);
    pAnim = pGameManager->getResAnim("battle_back");
    if (pAnim != nullptr)
    {
        setSize(pAnim->getSize());
    }
    pSprite = oxygine::spSprite::create();
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

    pAnim = pGameManager->getResAnim("battle_front");
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityFront);
    addChild(pSprite);

    pAnim = pGameManager->getResAnim("battle_front+mask", oxygine::ep_ignore_error);
    if (pAnim != nullptr)
    {
        pSprite = oxygine::spSprite::create();
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(priorityFront);
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
        addChild(pSprite);
    }
}

void BattleAnimation::createCoInfoBackImages(Unit* pAtkUnit, float atkStartHp, Unit* pDefUnit, float defStartHp)
{
    GameManager* pGameManager = GameManager::getInstance();
    Player* pPlayer = pAtkUnit->getOwner();
    CO* pCO = pPlayer->getCO(0);

    // create co info back image + co image
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("co_back");
    oxygine::ResAnim* pAnimMask = pGameManager->getResAnim("co_back+mask", oxygine::ep_ignore_error);
    /*********************************************************************************************/
    // co on the left upper corner
    /*********************************************************************************************/
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, -30);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);

    m_AtkCO0.coSprite = oxygine::spSprite::create();
    float coScale = 22.0f / 24.0f;
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        m_AtkCO0.anim = pCO->getResAnim(resAnim);
        m_AtkCO0.coSprite->setResAnim(m_AtkCO0.anim);
        setCOMood(m_AtkCO0, atkStartHp, defStartHp);
        if (getIsLeft(pAtkUnit, pDefUnit))
        {
            m_AtkCO0.coSprite->setPosition(-18, -13);
        }
        else
        {
            m_AtkCO0.coSprite->setPosition(getWidth() - 28, -13);
        }
        m_AtkCO0.coSprite->setPriority(priorityCO);
        m_AtkCO0.coSprite->setScale(coScale);
        addChild(m_AtkCO0.coSprite);
    }
    /*********************************************************************************************/
    // co on the left lower corner
    /*********************************************************************************************/
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, getHeight() - 45);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);

    m_AtkCO1.coSprite = oxygine::spSprite::create();
    pCO = pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        m_AtkCO1.anim = pCO->getResAnim(resAnim);
        m_AtkCO1.coSprite->setResAnim(m_AtkCO1.anim);
        setCOMood(m_AtkCO1, atkStartHp, defStartHp);
        if (getIsLeft(pAtkUnit, pDefUnit))
        {
            m_AtkCO1.coSprite->setPosition(-18, getHeight() - 28);
        }
        else
        {
            m_AtkCO1.coSprite->setPosition(getWidth() - 28, getHeight() - 28);
        }
        m_AtkCO1.coSprite->setPriority(priorityCO);
        m_AtkCO1.coSprite->setScale(coScale);
        addChild(m_AtkCO1.coSprite);
    }
    /*********************************************************************************************/
    // co on the right upper corner
    /*********************************************************************************************/
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, -30);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pPlayer = pDefUnit->getOwner();
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, -30);
    addChild(pSprite);

    pCO = pPlayer->getCO(0);
    m_DefCO0.coSprite = oxygine::spSprite::create();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        m_DefCO0.anim = pCO->getResAnim(resAnim);
        m_DefCO0.coSprite->setResAnim(m_DefCO0.anim);
        setCOMood(m_DefCO0, defStartHp, atkStartHp);
        if (getIsLeft(pDefUnit, pAtkUnit))
        {
            m_DefCO0.coSprite->setPosition(-18, -13);
        }
        else
        {
            m_DefCO0.coSprite->setPosition(getWidth() - 28, -13);
        }
        m_DefCO0.coSprite->setPriority(priorityCO);
        m_DefCO0.coSprite->setScale(coScale);
        addChild(m_DefCO0.coSprite);
    }
    /*********************************************************************************************/
    // co on the right lower corner
    /*********************************************************************************************/
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOBack);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);

    pCO = pPlayer->getCO(1);
    m_DefCO1.coSprite = oxygine::spSprite::create();
    if (pCO != nullptr)
    {
        QString resAnim = pCO->getCoID().toLower() + "+face";
        m_DefCO1.anim = pCO->getResAnim(resAnim);
        m_DefCO1.coSprite->setResAnim(m_DefCO1.anim);
        setCOMood(m_DefCO1, defStartHp, atkStartHp);
        if (getIsLeft(pDefUnit, pAtkUnit))
        {
            m_DefCO1.coSprite->setPosition(-18, getHeight() - 28);
        }
        else
        {
            m_DefCO1.coSprite->setPosition(getWidth() - 28, getHeight() - 28);
        }
        m_DefCO1.coSprite->setPriority(priorityCO);
        m_DefCO1.coSprite->setScale(coScale);
        addChild(m_DefCO1.coSprite);
    }
}

void BattleAnimation::createCoInfoFrontImages(Unit* pAtkUnit, Unit* pDefUnit)
{
    GameManager* pGameManager = GameManager::getInstance();
    // create co info front image
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("co_front");
    oxygine::ResAnim* pAnimMask = pGameManager->getResAnim("co_front+mask", oxygine::ep_ignore_error);
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, -30);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, -30);
    addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, getHeight() - 45);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(-35, getHeight() - 45);
    addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, - 30);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, - 30);
    addChild(pSprite);

    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnimMask);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        pSprite->setColorTable(pDefUnit->getOwner()->getColorTableAnim(), true);
    }
    else
    {
        pSprite->setColorTable(pAtkUnit->getOwner()->getColorTableAnim(), true);
    }
    addChild(pSprite);
    pSprite = oxygine::spSprite::create();
    pSprite->setResAnim(pAnim);
    pSprite->setPriority(priorityCOFront);
    pSprite->setPosition(getWidth() - 45, getHeight() - 45);
    addChild(pSprite);
}

void BattleAnimation::createHealthbar(Unit* pAtkUnit, float atkStartHp, Unit* pDefUnit, float defStartHp)
{
    // create health bar
    auto* pAnim = GameManager::getInstance()->getResAnim("healthbar");
    m_HealthBar0 = oxygine::spBox9Sprite::create();
    m_HealthBar0->setResAnim(pAnim);
    m_HealthBar0->setSize((spriteWidth - 8) * atkStartHp / Unit::MAX_UNIT_HP + 8, 9);
    if (getIsLeft(pAtkUnit, pDefUnit))
    {
        m_HealthBar0->setPosition(31, 25);
    }
    else
    {
        m_HealthBar0->setPosition(162, 25);
        m_HealthBar0->setFlippedX(true);
    }
    m_HealthBar0->setColor(getHealthBarColor(atkStartHp));
    addChild(m_HealthBar0);
    m_HealthBar1 = oxygine::spBox9Sprite::create();
    m_HealthBar1->setResAnim(pAnim);
    m_HealthBar1->setSize((spriteWidth - 8) * defStartHp / Unit::MAX_UNIT_HP + 8, 9);
    if (getIsLeft(pDefUnit, pAtkUnit))
    {
        m_HealthBar1->setPosition(31, 25);
    }
    else
    {
        m_HealthBar1->setPosition(162, 25);
        m_HealthBar1->setFlippedX(true);
    }
    m_HealthBar1->setColor(getHealthBarColor(defStartHp));
    addChild(m_HealthBar1);
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

oxygine::spSprite BattleAnimation::loadTerrainSprite(Unit* pUnit, Unit* pDefender)
{
    oxygine::spSprite ret = oxygine::spSprite::create();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
    pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase(pDefender), oxygine::ep_ignore_error);
    pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground(pDefender), oxygine::ep_ignore_error);
    pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground(pDefender), oxygine::ep_ignore_error);
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
    qint32 terrainDefense = pUnit->getTerrainDefense();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("defenseStar");
    float defenseY = 5;
    float startDefenseX = 5;
    float defenseX = startDefenseX;
    for (qint32 i = 1; i <= terrainDefense; i++)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        if (getIsLeft(pUnit, pDefender))
        {
            pSprite->setPosition(defenseX, defenseY);
        }
        else
        {
            pSprite->setPosition(127 - defenseX - pAnim->getWidth(), defenseY);
        }
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(10);
        ret->addChild(pSprite);
        if (pAnim != nullptr)
        {
            if (i % 4 == 0)
            {
                defenseY += pAnim->getHeight() + 2;
                defenseX = startDefenseX;
            }
            else
            {
                defenseX += pAnim->getWidth();
            }
        }
    }

    return ret;
}

void BattleAnimation::setCOMood(CoFace & coFace, float hp1, float hp2)
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
    const oxygine::ResAnim* pAnim = coFace.anim;
    if (pAnim != nullptr)
    {
        if (pAnim->getColumns() > 0)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnimColumn(pAnim, static_cast<qint32>(mood)), oxygine::timeMS(static_cast<qint64>(pAnim->getColumns() * GameMap::frameTime)), -1);
            coFace.coSprite->addTween(tween);
        }
        else
        {
            coFace.coSprite->setResAnim(pAnim, static_cast<qint32>(mood));
        }
    }
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
        pGameMenue->addChild(spBattleAnimation(this));
        m_battleTimer.start();
    }
}

void BattleAnimation::stop()
{
    m_battleTimer.stop();
}

bool BattleAnimation::onFinished(bool skipping)
{
    stopSound(true);
    return GameAnimation::onFinished(skipping);
}

void BattleAnimation::stopSound(bool forceStop)
{
    if (m_pAttackerAnimation.get() != nullptr)
    {
        m_pAttackerAnimation->stopSound(forceStop);
    }
    if (m_pDefenderAnimation.get() != nullptr)
    {
        m_pDefenderAnimation->stopSound(forceStop);
    }
}

void BattleAnimation::nextAnimatinStep()
{
    Mainapp::getInstance()->pauseRendering();
    CONSOLE_PRINT("BattleAnimation::nextAnimatinStep " + QString::number(static_cast<qint32>(m_currentState)), Console::eDEBUG);
    switch (m_currentState)
    {
        case AnimationProgress::MoveIn:
        {
            loadMoveInAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            loadStopAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            m_pAttackerAnimation->startNextFrame();
            break;
        }
        case AnimationProgress::MoveStop:
        {
            m_pAttackerAnimation->startNextFrame();
            qint32 stopDuration = m_pAttackerAnimation->getStopDurationMS(m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            if (stopDuration > 0)
            {
                startBattleTimer(stopDuration);
                break;
            }
            else
            {
                m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
            }
        }
        case AnimationProgress::WaitAfterIn:
        {
            stopSound(true);
            startBattleTimer(500);
            break;
        }
        case AnimationProgress::AttackerFire:
        {
            stopSound();
            loadFireAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            break;
        }
        case AnimationProgress::AttackerFired:
        {
            qint32 remainingDuration = loadFiredAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit, m_AtkWeapon);
            if (remainingDuration > 0)
            {

                startBattleTimer(remainingDuration);
                break;
            }
            else
            {
                m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
            }
        }
        case AnimationProgress::AttackerImpact:
        {
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
                startBattleTimer(500);
            }
            break;
        }
        case AnimationProgress::DefenderFired:
        {
            // remove firing frames
            qint32 remainingDuration = loadFiredAnimation(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit, m_DefWeapon);
            if (remainingDuration > 0)
            {

                m_battleTimer.start(remainingDuration / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
                break;
            }
            else
            {
                m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
            }
        }
        case AnimationProgress::DefenderImpact:
        {
            // remove firing frames
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
            startBattleTimer(500);
            break;
        }
        case AnimationProgress::Finished:
        {
            break;
        }
    }
    m_currentState = static_cast<AnimationProgress>(static_cast<qint32>(m_currentState) + 1);
    
    if (m_currentState > AnimationProgress::Finished)
    {
        BattleAnimation::onFinished(false);
    }
    Mainapp::getInstance()->continueRendering();
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
    startBattleTimer(pSprite->getMoveInDurationMS(pUnit1, pUnit2, weapon));
    
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
    qint32 firedDuration = pSprite->getFiredDurationMS(pUnit1, pUnit2, weapon);
    qint32 fireDuration = pSprite->getFireDurationMS(pUnit1, pUnit2, weapon);
    if (firedDuration > fireDuration || firedDuration <= 0)
    {
        firedDuration = fireDuration;
    }
    startBattleTimer(firedDuration);
}

void BattleAnimation::loadImpactAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, spBattleAnimationSprite pAttackerSprite,
                                          oxygine::spBox9Sprite pHealthbar, float endHp, qint32 weapon, float enemyHp)
{    
    if (endHp < 0.0f)
    {
        endHp = 0.0f;
        oxygine::spTween visibleTween = oxygine::createTween(TweenToggleVisibility(0, 0.9), oxygine::timeMS(1), 1, false, oxygine::timeMS(static_cast<qint64>(800 / Settings::getBattleAnimationSpeed())));
        pHealthbar->addTween(visibleTween);
    }
    oxygine::VStyleActor::TweenColor tweenColor(getHealthBarColor(endHp));
    oxygine::spTween colorTween = oxygine::createTween(tweenColor, oxygine::timeMS(static_cast<qint64>(800 / Settings::getBattleAnimationSpeed())));
    pHealthbar->addTween(colorTween);
    oxygine::spTween posTween = oxygine::createTween(oxygine::Actor::TweenWidth((spriteWidth -  8) * endHp / Unit::MAX_UNIT_HP + 8), oxygine::timeMS(static_cast<qint64>(800 / Settings::getBattleAnimationSpeed())));
    pHealthbar->addTween(posTween);
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
    startBattleTimer(pSprite->getImpactDurationMS(pUnit2, pUnit1, weapon));
}

qint32 BattleAnimation::loadFiredAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2, qint32 weapon)
{
    pSprite->setHasFired(true);
    pSprite->loadAnimation(BattleAnimationSprite::standingFiredAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);

    qint32 firedDuration = pSprite->getFiredDurationMS(pUnit1, pUnit2, weapon);
    qint32 fireDuration = pSprite->getFireDurationMS(pUnit1, pUnit2, weapon);
    if (firedDuration < 0 || firedDuration <= fireDuration)
    {
        return 0;
    }
    else
    {
        return fireDuration - firedDuration;
    }
}

void BattleAnimation::loadDyingAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, qint32 weapon)
{
    pSprite->loadAnimation(BattleAnimationSprite::dyingAnimation, pUnit1, pUnit2, weapon);
    setSpriteFlipped(pSprite, pUnit1, pUnit2);
    startBattleTimer(pSprite->getDyingDurationMS(pUnit1, pUnit2, weapon));
}

void BattleAnimation::loadDyingFadeoutAnimation(spBattleAnimationSprite pSprite)
{
    constexpr qint32 fadeoutTime = 1000;
    qint32 sleep = pSprite->loadDyingFadeOutAnimation(fadeoutTime - 100);
    startBattleTimer(fadeoutTime + sleep);
}

void BattleAnimation::startBattleTimer(qint32 duration)
{
    CONSOLE_PRINT("Starting battle timer with base duration: " + QString::number(duration), Console::eDEBUG);
    m_battleTimer.start(duration / static_cast<qint32>(Settings::getBattleAnimationSpeed()));
}

void BattleAnimation::addBattleViewScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs, qint32 shakePauseMs)
{
    oxygine::spTween tween = oxygine::createTween(TweenScreenshake(startIntensity, decay / Settings::getBattleAnimationSpeed(), oxygine::timeMS(shakePauseMs)),
                                                  oxygine::timeMS(static_cast<qint64>(durationMs / Settings::getBattleAnimationSpeed())), 1, false, oxygine::timeMS(static_cast<qint64>(delayMs / Settings::getBattleAnimationSpeed())));
    addTween(tween);
}
