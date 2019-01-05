#include "gameanimation.h"

#include "game/gameanimationfactory.h"

#include "resource_management/gameanimationmanager.h"

#include "coreengine/console.h"

GameAnimation::GameAnimation(quint32 frameTime)
    : QObject(),
      m_frameTime(frameTime)
{
    connect(this, SIGNAL(sigFinished()), this, SLOT(onFinished()), Qt::QueuedConnection);
}

void GameAnimation::addSprite(QString spriteID, float offsetX, float offsetY)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * m_frameTime, 1);
        pSprite->addTween(tween);
        this->addChild(pSprite);
        pSprite->setPosition(offsetX, offsetY);

        tween->setDoneCallback([=](oxygine::Event *)->void
        {
            emit sigFinished();
        });
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}

void GameAnimation::onFinished()
{
    GameAnimationFactory::removeAnimation(this);
}
