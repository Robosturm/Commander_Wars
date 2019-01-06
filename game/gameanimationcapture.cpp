#include "game/gameanimationcapture.h"

#include "resource_management/gameanimationmanager.h"

#include "game/gamemap.h"

#include "coreengine/console.h"

GameAnimationCapture::GameAnimationCapture(qint32 startPoints, qint32 endPoints)
    : GameAnimation(GameMap::frameTime),
      m_startPoints(startPoints),
      m_endPoints(endPoints)
{
}





void GameAnimationCapture::addBackgroundSprite(QString spriteID)
{
    GameAnimationManager* pGameAnimationManager = GameAnimationManager::getInstance();
    oxygine::ResAnim* pAnim = pGameAnimationManager->getResAnim(spriteID.toStdString());
    if (pAnim != nullptr)
    {
        oxygine::spSprite pSprite = new oxygine::Sprite();
        pSprite->setResAnim(pAnim);
        this->addChild(pSprite);
    }
    else
    {
        Console::print("Unable to load animation sprite: " + spriteID, Console::eERROR);
    }
}
