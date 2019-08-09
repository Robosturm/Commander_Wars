#include "battleanimation.h"

#include "gamemap.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "resource_management/cospritemanager.h"

#include "game/player.h"

#include "game/co.h"

BattleAnimation::BattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp,
                                 Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime)),
      m_pAtkTerrain(pAtkTerrain),
      m_pAtkUnit(pAtkUnit),
      m_atkStartHp(atkStartHp),
      m_atkEndHp(atkEndHp),
      m_pDefTerrain(pDefTerrain),
      m_pDefUnit(pDefUnit),
      m_defStartHp(defStartHp),
      m_defEndHp(defEndHp)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

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

    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
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
        oxygine::ResAnim* pAnimCO = pCOSpriteManager->getResAnim(resAnim.toStdString().c_str());
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
        oxygine::ResAnim* pAnimCO = pCOSpriteManager->getResAnim(resAnim.toStdString().c_str());
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
        oxygine::ResAnim* pAnimCO = pCOSpriteManager->getResAnim(resAnim.toStdString().c_str());
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
        oxygine::ResAnim* pAnimCO = pCOSpriteManager->getResAnim(resAnim.toStdString().c_str());
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
    m_HealthBar0->setSize(127 * atkStartHp / 10.0f, 9);
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
    m_HealthBar1->setSize(127 * defStartHp / 10.0f, 9);
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
                                                                               Mainapp::roundUp(atkStartHp));
    setSpritePosition(m_pAttackerAnimation, pAtkUnit, pDefUnit);
    addChild(m_pAttackerAnimation);
    m_pDefenderAnimation = new BattleAnimationSprite(pDefUnit, pDefTerrain, BattleAnimationSprite::standingAnimation,
                                                                               Mainapp::roundUp(defStartHp));
    if (!m_pAttackerAnimation->hasMoveInAnimation())
    {
        // skip move in
        currentState = AnimationProgress::WaitAfterIn;
    }

    setSpritePosition(m_pDefenderAnimation, pDefUnit, pAtkUnit);
    addChild(m_pDefenderAnimation);

    // battleTimer
    battleTimer.setSingleShot(false);
    connect(&battleTimer, &QTimer::timeout, this, &BattleAnimation::nextAnimatinStep, Qt::QueuedConnection);
    nextAnimatinStep();
}

bool BattleAnimation::getIsLeft(Unit* pUnit1, Unit* pUnit2)
{
    if ((pUnit1->getX() < pUnit2->getX()) ||
        (pUnit1->getY() < pUnit2->getY()))
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
    }
    else
    {
        pSprite->setPosition(162, 34);
        // flip all childdren
        pSprite->flipActorsX(pSprite, true);
    }
}

oxygine::spSprite BattleAnimation::loadTerrainSprite(Unit* pUnit)
{
    oxygine::spSprite ret = new oxygine::Sprite();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::ResAnim* pAnimBase = nullptr;
    oxygine::ResAnim* pAnimFore = nullptr;
    oxygine::ResAnim* pAnimBack = nullptr;
    pAnimBase = pGameManager->getResAnim(pUnit->getTerrainAnimationBase().toStdString().c_str(), oxygine::ep_ignore_error);
    pAnimFore = pGameManager->getResAnim(pUnit->getTerrainAnimationForeground().toStdString().c_str(), oxygine::ep_ignore_error);
    pAnimBack = pGameManager->getResAnim(pUnit->getTerrainAnimationBackground().toStdString().c_str(), oxygine::ep_ignore_error);
    oxygine::spSprite pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBase);
    ret->addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimFore);
    pSprite->setPriority(1);
    ret->addChild(pSprite);
    pSprite = new oxygine::Sprite();
    pSprite->setResAnim(pAnimBack);
    pSprite->setPriority(2);
    ret->addChild(pSprite);
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

oxygine::Color BattleAnimation::getHealthBarColor(float hp)
{
    float divider = hp / 10.0f;
    if (divider > 2.0f / 3.0f)
    {
        return oxygine::Color(0, 255, 0, 255);
    }
    else if (divider > 1.0f / 3.0f)
    {
        return oxygine::Color(255, 128, 0, 255);
    }
    else
    {
        return oxygine::Color(255, 0, 0, 255);
    }
}

void BattleAnimation::restart()
{
    GameMenue::getInstance()->addChild(this);
    battleTimer.start();
}

void BattleAnimation::stop()
{
    battleTimer.stop();
}

void BattleAnimation::nextAnimatinStep()
{
    switch (currentState)
    {
        case AnimationProgress::MoveIn:
        {
            loadMoveInAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit);
            break;
        }
        case AnimationProgress::WaitAfterIn:
        {
            battleTimer.start(500 / Settings::getAnimationSpeed());
            break;
        }
        case AnimationProgress::AttackerFire:
        {
            loadFireAnimation(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit);
            break;
        }
        case AnimationProgress::AttackerImpact:
        {
            loadImpactAnimation(m_pDefUnit, m_pAtkUnit, m_pDefenderAnimation, m_HealthBar1, m_defEndHp,
                                m_DefCO0, m_DefCO1, m_atkStartHp);
            break;
        }
        case AnimationProgress::DefenderFire:
        {
            m_pDefenderAnimation->setHpRounded(Mainapp::roundUp(m_defEndHp));
            m_pDefenderAnimation->loadAnimation(BattleAnimationSprite::standingAnimation);
            setSpritePosition(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit);
            if (m_atkEndHp != m_atkStartHp)
            {
                loadFireAnimation(m_pDefenderAnimation, m_pDefUnit, m_pAtkUnit);
            }
            else
            {
                currentState = AnimationProgress::WaitAfterBattle;
                battleTimer.start(500 / Settings::getAnimationSpeed());
            }
            break;
        }
        case AnimationProgress::DefenderImpact:
        {
            loadImpactAnimation(m_pAtkUnit, m_pDefUnit, m_pAttackerAnimation, m_HealthBar0, m_atkEndHp,
                                m_AtkCO0, m_AtkCO1, m_defEndHp);
            break;
        }
        case AnimationProgress::WaitAfterBattle:
        {
            m_pAttackerAnimation->setHpRounded(Mainapp::roundUp(m_atkEndHp));
            m_pAttackerAnimation->loadAnimation(BattleAnimationSprite::standingAnimation);
            setSpritePosition(m_pAttackerAnimation, m_pAtkUnit, m_pDefUnit);
            battleTimer.start(500 / Settings::getAnimationSpeed());
            break;
        }
        case AnimationProgress::Finished:
        {
            onFinished();
            break;
        }
    }
    currentState = static_cast<AnimationProgress>(static_cast<qint32>(currentState) + 1);
}

void BattleAnimation::loadMoveInAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2)
{
    pSprite->loadAnimation(BattleAnimationSprite::moveInAnimation, pUnit1);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    battleTimer.start(pSprite->getMoveInDurationMS() / static_cast<qint32>(Settings::getAnimationSpeed()));
}


void BattleAnimation::loadFireAnimation(spBattleAnimationSprite pSprite, Unit* pUnit1, Unit* pUnit2)
{
    pSprite->loadAnimation(BattleAnimationSprite::fireAnimation, pUnit1);
    setSpritePosition(pSprite, pUnit1, pUnit2);
    battleTimer.start(pSprite->getFireDurationMS() / static_cast<qint32>(Settings::getAnimationSpeed()));
}

void BattleAnimation::loadImpactAnimation(Unit* pUnit1, Unit* pUnit2, spBattleAnimationSprite pSprite, oxygine::spColorRectSprite pColorRect, float endHp,
                                          oxygine::spSprite pCO0, oxygine::spSprite pCO1, float enemyHp)
{
    if (endHp < 0.0f)
    {
        endHp = 0.0f;
    }
    oxygine::ColorRectSprite::TweenColor tweenColor(getHealthBarColor(endHp));
    oxygine::spTween colorTween = oxygine::createTween(tweenColor, 800 / Settings::getAnimationSpeed());
    pColorRect->addTween(colorTween);
    oxygine::spTween posTween = oxygine::createTween(oxygine::Actor::TweenWidth(127.0f * endHp / 10.0f), 800 / Settings::getAnimationSpeed());
    pColorRect->addTween(posTween);
    // add impact image
    oxygine::ColorRectSprite::TweenColor tweenColor2(oxygine::Color(255, 0, 0));
    oxygine::spActor child = pSprite->getClipActor()->getFirstChild();
    while (child)
    {
        oxygine::spTween colorTween2 = oxygine::createTween(tweenColor2, 500 / Settings::getAnimationSpeed(), 1, true, 100);
        child->addTween(colorTween2);
        child = child->getNextSibling();
    }
    setCOMood(pCO0, endHp, enemyHp);
    setCOMood(pCO1, endHp, enemyHp);

    pSprite->loadAnimation(BattleAnimationSprite::impactAnimation, pUnit2, false);
    setSpritePosition(pSprite, pUnit1, pUnit2);

    battleTimer.start(pSprite->getImpactDurationMS(pUnit1) / static_cast<qint32>(Settings::getAnimationSpeed()));
}
