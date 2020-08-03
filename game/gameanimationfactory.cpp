#include "game/gameanimationfactory.h"

#include "game/gamemap.h"

#include "game/gameanimationwalk.h"

#include "game/gameanimationcapture.h"

#include "game/gameanimationdialog.h"

#include "game/gameanimationpower.h"

#include "game/gameanimationnextday.h"

#include "game/battleanimation.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "menue/gamemenue.h"

#include "coreengine/mainapp.h"

GameAnimationFactory* GameAnimationFactory::m_pInstance = nullptr;
QVector<spGameAnimation> GameAnimationFactory::m_Animations;

GameAnimationFactory::GameAnimationFactory()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void GameAnimationFactory::queueAnimation(GameAnimation* pGameAnimation)
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pGameAnimation)
        {
           m_Animations[i]->detach();
           m_Animations[i]->stop();
           break;
        }
    }
}

void GameAnimationFactory::startQueuedAnimation(GameAnimation* pGameAnimation)
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pGameAnimation)
        {
            m_Animations[i]->restart();
            break;
        }
    }
}

GameAnimationFactory* GameAnimationFactory::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameAnimationFactory();
    }
    return m_pInstance;
}

GameAnimation* GameAnimationFactory::createAnimation(quint32 x, quint32 y, quint32 frameTime, bool mapPosition)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    spGameAnimation animation = new GameAnimation(frameTime);
    if (mapPosition)
    {
        animation->setPosition(x * GameMap::Imagesize, y * GameMap::Imagesize);
    }
    else
    {
        animation->setPosition(x, y);
    }
    animation->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(animation);
    m_Animations.append(animation);
    pApp->continueThread();
    return animation.get();
}

GameAnimationWalk* GameAnimationFactory::createWalkingAnimation(Unit* pUnit, GameAction* pAction)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationWalk* pGameAnimationWalk = new GameAnimationWalk(pUnit, pAction->getMovePath());
    pGameAnimationWalk->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(pGameAnimationWalk);
    m_Animations.append(pGameAnimationWalk);
    pApp->continueThread();
    return pGameAnimationWalk;
}

GameAnimationPower* GameAnimationFactory::createAnimationPower(QColor color, GameEnums::PowerMode powerMode, CO* pCO, quint32 frameTime)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationPower* pAnim = GameAnimationPower::createGameAnimationPower(frameTime, color, powerMode, pCO);
    pAnim->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(pAnim);
    }
    m_Animations.append(pAnim);
    pApp->continueThread();
    return pAnim;
}

GameAnimationDialog* GameAnimationFactory::createGameAnimationDialog(QString text, QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationDialog* pAnim = new GameAnimationDialog(frameTime);
    pAnim->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pAnim->setDialog(text);
    pAnim->setCO(coid, mood);
    pAnim->setColor(color);
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        pGameMenue->addChild(pAnim);
    }
    m_Animations.append(pAnim);
    pApp->continueThread();
    return pAnim;
}

GameAnimationNextDay* GameAnimationFactory::createGameAnimationNextDay(Player* pPlayer, quint32 frameTime)
{
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        GameAnimationNextDay* pAnim = new GameAnimationNextDay(pPlayer, frameTime, false);
        pGameMenue->addChild(pAnim);
        m_Animations.append(pAnim);
        pApp->continueThread();
        return pAnim;
    }
    return nullptr;
}

GameAnimationCapture* GameAnimationFactory::createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationCapture* pGameAnimationCapture = new GameAnimationCapture(startPoints, endPoints, maxPoints);
    pGameAnimationCapture->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    pGameAnimationCapture->setPosition(x, y);
    GameMap::getInstance()->addChild(pGameAnimationCapture);
    m_Animations.append(pGameAnimationCapture);
    pApp->continueThread();
    return pGameAnimationCapture;
}

GameAnimation* GameAnimationFactory::createBattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp, qint32 atkWeapon,
                                                           Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp, qint32 defWeapon, float defenderDamage)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimation* pRet = nullptr;
    if (pDefUnit != nullptr)
    {
        // log this attack to our battle log
        qint32 atkDamage = Mainapp::roundUp(defStartHp) - Mainapp::roundUp(defEndHp);
        qint32 defDamage = Mainapp::roundUp(atkStartHp) - Mainapp::roundUp(atkEndHp);
        spGameMap pMap = GameMap::getInstance();
        pMap->getGameRecorder()->logAttack(pMap->getCurrentDay(),
                                           atkDamage, pAtkTerrain->getX(), pAtkTerrain->getY(), pAtkUnit->getUnitID(), pAtkUnit->getOwner()->getPlayerID(), (atkEndHp <= 0),
                                           defDamage, pDefTerrain->getX(), pDefTerrain->getY(), pDefUnit->getUnitID(), pDefUnit->getOwner()->getPlayerID(), (defEndHp <= 0),
                                           pDefUnit->getOwner()->getFieldVisible(pAtkTerrain->getX(), pAtkTerrain->getY()));

        if (Settings::getBattleAnimations() == GameEnums::BattleAnimationMode_Detail)
        {
            pRet = new BattleAnimation(pAtkTerrain, pAtkUnit, atkStartHp, atkEndHp, atkWeapon,
                                       pDefTerrain, pDefUnit, defStartHp, defEndHp, defWeapon, defenderDamage);
            pRet->setScale(2.0f);
            pRet->setPosition(Settings::getWidth() / 2 - pRet->getScaledWidth() / 2,
                              Settings::getHeight() / 2 - pRet->getScaledHeight() / 2);
            GameMenue::getInstance()->addChild(pRet);
            m_Animations.append(pRet);
        }
        else
        {
            // attacking unit
            GameAnimation* pAtk = createAnimation(pDefTerrain->getX(), pDefTerrain->getY(), 70);
            pAtk->addSprite("blackhole_shot", -GameMap::Imagesize * 0.5f, -GameMap::Imagesize * 0.5f, 0, 1.5f);
            pAtk->setSound("talongunhit.wav", 1);            
            GameAnimation* pDmgTextAtk = createAnimation(pDefTerrain->getX(), pDefTerrain->getY());
            pDmgTextAtk->addText(QString::number(atkDamage) + " Hp", -8, 0, 1.5f, Qt::GlobalColor::red);
            pDmgTextAtk->addTweenPosition(QPoint(pDefTerrain->getX() * GameMap::Imagesize, (pDefTerrain->getY() - 2) * GameMap::Imagesize), 1000);
            pDmgTextAtk->addTweenWait(1500);
            pAtk->queueAnimation(pDmgTextAtk);
            if (defenderDamage >= 0)
            {
                // counter damage
                pRet = createAnimation(pAtkTerrain->getX(), pAtkTerrain->getY(), 70);
                pRet->addSprite("blackhole_shot", -GameMap::Imagesize * 0.5f, -GameMap::Imagesize * 0.5f, 0, 1.5f);
                pRet->setSound("talongunhit.wav", 1);
                pDmgTextAtk->queueAnimation(pRet);
                GameAnimation* pDmgTextDef = createAnimation(pAtkTerrain->getX(), pAtkTerrain->getY());
                pDmgTextDef->addText(QString::number(defDamage) + " Hp", -8, 0, 1.5f, Qt::GlobalColor::red);
                pDmgTextDef->addTweenPosition(QPoint(pAtkTerrain->getX() * GameMap::Imagesize, (pAtkTerrain->getY() - 2) * GameMap::Imagesize), 1000);
                pDmgTextDef->addTweenWait(1500);
                pRet->queueAnimation(pDmgTextDef);
                pRet = pDmgTextDef;
            }
            else
            {
                pRet = pDmgTextAtk;
            }
        }
    }
    else
    {
        // attacking building or terrain
        pRet = createAnimation(pDefTerrain->getX(), pDefTerrain->getY(), 70);
        pRet->addSprite("blackhole_shot", -GameMap::Imagesize * 0.5f, -GameMap::Imagesize * 0.5f, 0, 1.5f);
        pRet->setSound("talongunhit.wav", 1);
    }
    pApp->continueThread();
    return pRet;
}

qint32 GameAnimationFactory::getAnimationCount()
{
    return m_Animations.size();
}

GameAnimation* GameAnimationFactory::getAnimation(qint32 index)
{
    if ((index >= 0) && (index < m_Animations.size()))
    {
        return m_Animations[index].get();
    }
    return nullptr;
}

void GameAnimationFactory::removeAnimation(GameAnimation* pAnimation)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        if (m_Animations[i].get() == pAnimation)
        {
            spGameAnimation spAnimation = m_Animations[i];
            spAnimation->detach();
            m_Animations.removeAt(i);
        }
        else
        {
             i++;
        }
    }
    if (m_Animations.size() == 0)
    {
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
    pApp->continueThread();
}

void GameAnimationFactory::clearAllAnimations()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        m_Animations[i]->detach();
    }
    m_Animations.clear();
    pApp->continueThread();
}

void GameAnimationFactory::finishAllAnimations()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        spGameAnimation spAnimation = m_Animations[i];
        if (!spAnimation->onFinished())
        {
            i++;
        }
    }
    pApp->continueThread();
}
