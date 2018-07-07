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

    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        oxygine::Actor::addChild(new GameMap(width, heigth));
    }


    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit sigMouseWheel(pTouchEvent->wheelDirection.y);
        }
    });
    connect(this, SIGNAL(sigMouseWheel(qint32)), this, SLOT(mouseWheel(qint32)));
    addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Middle)
            {
                m_moveMap = true;
                m_MoveMapMousePoint.setX(pTouchEvent->getPointer()->getPosition().x);
                m_MoveMapMousePoint.setY(pTouchEvent->getPointer()->getPosition().y);
            }
        }
    });
    addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Middle)
            {
                m_moveMap = false;
            }
        }
    });
    addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (m_moveMap)
            {
                qint32 x = m_MoveMapMousePoint.x() - pTouchEvent->getPointer()->getPosition().x;
                qint32 y = m_MoveMapMousePoint.y() - pTouchEvent->getPointer()->getPosition().y;
                m_MoveMapMousePoint.setX(pTouchEvent->getPointer()->getPosition().x);
                m_MoveMapMousePoint.setX(pTouchEvent->getPointer()->getPosition().y);
                Console::print("Position x: " + QString::number(pTouchEvent->getPointer()->getPosition().x )+ " y: " + QString::number(pTouchEvent->getPointer()->getPosition().y), Console::eDEBUG);
                emit sigMoveMap(x, y);
            }
        }
    });
    connect(this, SIGNAL(sigMoveMap(qint32,qint32)), this, SLOT(MoveMap(qint32,qint32)));

}

InGameMenue::~InGameMenue()
{

}

void InGameMenue::mouseWheel(qint32 direction)
{
    GameMap::getInstance()->zoom(direction);
}

void InGameMenue::MoveMap(qint32 x, qint32 y)
{
    GameMap::getInstance()->moveMap(x, y);
}
