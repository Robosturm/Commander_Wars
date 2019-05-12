#include "game/gameanimationfactory.h"

#include "game/gameanimation.h"

#include "game/gamemap.h"

#include "game/gameanimationwalk.h"

#include "game/gameanimationcapture.h"

#include "game/gameanimationdialog.h"

#include "game/gameanimationpower.h"

#include "game/gameanimationnextday.h"

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
    return animation.get();
}

GameAnimationWalk* GameAnimationFactory::createWalkingAnimation(Unit* pUnit, GameAction* pAction)
{
    GameAnimationWalk* pGameAnimationWalk = new GameAnimationWalk(pUnit, pAction->getMovePath());
    pGameAnimationWalk->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(pGameAnimationWalk);
    m_Animations.append(pGameAnimationWalk);
    return pGameAnimationWalk;
}

GameAnimationPower* GameAnimationFactory::createAnimationPower(QColor color, GameEnums::PowerMode powerMode, QString coid, quint32 frameTime)
{
    GameAnimationPower* pAnim = new GameAnimationPower(frameTime, color, powerMode, coid);
    pAnim->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    GameMenue::getInstance()->addChild(pAnim);
    m_Animations.append(pAnim);
    return pAnim;
}

GameAnimationDialog* GameAnimationFactory::createGameAnimationDialog(QString text, QString coid, GameEnums::COMood mood, QColor color, quint32 frameTime)
{
    GameAnimationDialog* pAnim = new GameAnimationDialog(frameTime);
    pAnim->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    pAnim->setDialog(text);
    pAnim->setCO(coid, mood);
    pAnim->setColor(color);

    GameMenue::getInstance()->addChild(pAnim);
    m_Animations.append(pAnim);
    return pAnim;
}

GameAnimationNextDay* GameAnimationFactory::createGameAnimationNextDay(Player* pPlayer, quint32 frameTime)
{
    GameAnimationNextDay* pAnim = new GameAnimationNextDay(pPlayer, frameTime);
    GameMenue::getInstance()->addChild(pAnim);
    m_Animations.append(pAnim);
    return pAnim;
}

GameAnimationCapture* GameAnimationFactory::createGameAnimationCapture(qint32 x, qint32 y, qint32 startPoints, qint32 endPoints, qint32 maxPoints)
{
    GameAnimationCapture* pGameAnimationCapture = new GameAnimationCapture(startPoints, endPoints, maxPoints);
    pGameAnimationCapture->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    pGameAnimationCapture->setPosition(x, y);
    GameMap::getInstance()->addChild(pGameAnimationCapture);
    m_Animations.append(pGameAnimationCapture);
    return pGameAnimationCapture;
}

qint32 GameAnimationFactory::getAnimationCount()
{
    return m_Animations.size();
}

void GameAnimationFactory::removeAnimation(GameAnimation* pAnimation)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pAnimation)
        {
            spGameAnimation spAnimation = m_Animations[i];
            spAnimation->detach();
            m_Animations.removeAt(i);
            break;
        }
    }
    pApp->continueThread();
    if (m_Animations.size() == 0)
    {
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}

void GameAnimationFactory::clearAllAnimations()
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        m_Animations[i]->detach();
    }
    m_Animations.clear();
}

void GameAnimationFactory::finishAllAnimations()
{
    qint32 i = 0;
    while (i < m_Animations.size())
    {
        spGameAnimation spAnimation = m_Animations[i];
        if (!spAnimation->onFinished())
        {
            i++;
        }
    }
}
