#include "game/gameanimationfactory.h"

#include "game/gameanimation.h"

#include "game/gamemap.h"

#include "game/gameanimationwalk.h"

#include "game/gameanimationcapture.h"

#include "game/unit.h"

#include "game/gameaction.h"

#include "coreengine/mainapp.h"

GameAnimationFactory* GameAnimationFactory::m_pInstance = nullptr;
QVector<spGameAnimation> GameAnimationFactory::m_Animations;

GameAnimationFactory::GameAnimationFactory()
    : QObject()
{

}

void GameAnimationFactory::queueAnimation(GameAnimation* pGameAnimation)
{
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pGameAnimation)
        {
            GameMap::getInstance()->removeChild(m_Animations[i]);
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
            GameMap::getInstance()->addChild(m_Animations[i]);
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
    // pGameAnimationWalk->setPosition(pUnit->getX() * GameMap::Imagesize, pUnit->getY() * GameMap::Imagesize);
    pGameAnimationWalk->setPriority(static_cast<short>(Mainapp::ZOrder::Animation));
    GameMap::getInstance()->addChild(pGameAnimationWalk);
    m_Animations.append(pGameAnimationWalk);
    return pGameAnimationWalk;
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
    for (qint32 i = 0; i < m_Animations.size(); i++)
    {
        if (m_Animations[i].get() == pAnimation)
        {
            spGameAnimation spAnimation = m_Animations[i];
            GameMap::getInstance()->removeChild(spAnimation);
            m_Animations.removeAt(i);
            break;
        }
    }
    if (m_Animations.size() == 0)
    {
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
}

void GameAnimationFactory::finishAllAnimations()
{
    while (m_Animations.size() > 0)
    {
        spGameAnimation spAnimation = m_Animations[m_Animations.size() - 1];
        spAnimation->onFinished();
    }
}
