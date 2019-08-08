#include "battleanimation.h"

#include "gamemap.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"

#include "resource_management/gamemanager.h"

#include "resource_management/cospritemanager.h"

#include "game/battleanimationsprite.h"

#include "game/player.h"

#include "game/co.h"

BattleAnimation::BattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp,
                                 Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp)
    : GameAnimation(static_cast<quint32>(GameMap::frameTime))
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

    // dummy impl for standing units
    spBattleAnimationSprite pDummyAnimationSprite1 = new BattleAnimationSprite(pAtkUnit, pAtkTerrain, BattleAnimationSprite::standingAnimation,
                                                                               Mainapp::roundUp(atkStartHp));
    setSpritePosition(pDummyAnimationSprite1, pAtkUnit, pDefUnit);
    addChild(pDummyAnimationSprite1);
    spBattleAnimationSprite pDummyAnimationSprite2 = new BattleAnimationSprite(pDefUnit, pDefTerrain, BattleAnimationSprite::standingAnimation,
                                                                               Mainapp::roundUp(defStartHp));
    setSpritePosition(pDummyAnimationSprite2, pDefUnit, pAtkUnit);
    addChild(pDummyAnimationSprite2);

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
        m_AtkCO0->setPosition(pSprite->getX() + 17, pSprite->getY() + 17);
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
        m_AtkCO1->setPosition(pSprite->getX() + 17, pSprite->getY() + 17);
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
        setCOMood(m_DefCO0, atkStartHp, defStartHp);
        m_DefCO0->setPosition(pSprite->getX() + 17, pSprite->getY() + 17);
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
        setCOMood(m_DefCO1, atkStartHp, defStartHp);
        m_DefCO1->setPosition(pSprite->getX() + 17, pSprite->getY() + 17);
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
    if (getIsRight(pAtkUnit, pDefUnit))
    {
        m_HealthBar0->setPosition(31, 25);
    }
    else
    {
        m_HealthBar0->setPosition(162, 25);
    }
    setHealthBarColor(m_HealthBar0, atkStartHp);
    addChild(m_HealthBar0);
    m_HealthBar1 = new oxygine::ColorRectSprite();
    m_HealthBar1->setSize(127 * defStartHp / 10.0f, 9);
    if (getIsRight(pDefUnit, pAtkUnit))
    {
        m_HealthBar1->setPosition(31, 25);
    }
    else
    {
        m_HealthBar1->setPosition(162, 25);
    }
    setHealthBarColor(m_HealthBar1, defStartHp);
    addChild(m_HealthBar1);

    // dummy
    endTimer.setSingleShot(true);
    endTimer.setInterval(1000 / static_cast<qint32>(Settings::getAnimationSpeed()));
    connect(&endTimer, &QTimer::timeout, this, &BattleAnimation::onFinished, Qt::QueuedConnection);
    endTimer.start();
}

bool BattleAnimation::getIsRight(Unit* pUnit1, Unit* pUnit2)
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
    if (getIsRight(pUnit1, pUnit2))
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

void BattleAnimation::setHealthBarColor(oxygine::spColorRectSprite pColorBar, float hp)
{
    float divider = hp / 10.0f;
    if (divider > 2.0f / 3.0f)
    {
        pColorBar->setColor(0, 255, 0, 255);
    }
    else if (divider > 1.0f / 3.0f)
    {
        pColorBar->setColor(255, 128, 0, 255);
    }
    else
    {
        pColorBar->setColor(255, 0, 0, 255);
    }
}

void BattleAnimation::restart()
{
    GameMenue::getInstance()->addChild(this);
    endTimer.start();
}


void BattleAnimation::stop()
{
    endTimer.stop();
}
