#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

InGameMenue::InGameMenue(qint32 width, qint32 heigth)
    : QObject()
{
    Console::print("Entering In Game Menue", Console::eDEBUG);
    Mainapp* pApp = Mainapp::getInstance();
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    oxygine::Actor::addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+0");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(-32000);
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());
    oxygine::Actor::addChild(new GameMap(width, heigth));

    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigMouseWheel(pTouchEvent->wheelDirection.y);
        }
    });
    connect(this, SIGNAL(sigMouseWheel(qint32)), this, SLOT(mouseWheel(qint32)));
}

InGameMenue::~InGameMenue()
{

}

void InGameMenue::mouseWheel(qint32 direction)
{
    GameMap::getInstance()->zoom(direction);
}
