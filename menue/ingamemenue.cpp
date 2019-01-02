#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

InGameMenue::InGameMenue(qint32 width, qint32 heigth, QString map)
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
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(pApp->getSettings()->getWidth() / pBackground->getWidth());
    sprite->setScaleY(pApp->getSettings()->getHeight() / pBackground->getHeight());

    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        oxygine::Actor::addChild(new GameMap(width, heigth));
    }
    else
    {
        oxygine::Actor::addChild(new GameMap(map));
    }



    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            emit this->sigMouseWheel(pTouchEvent->wheelDirection.y);
        }
    });
    connect(this, SIGNAL(sigMouseWheel(qint32)), this, SLOT(mouseWheel(qint32)), Qt::QueuedConnection);
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Middle)
            {
                this->m_moveMap = true;
                this->m_MoveMapMousePoint.setX(pTouchEvent->getPointer()->getPosition().x);
                this->m_MoveMapMousePoint.setY(pTouchEvent->getPointer()->getPosition().y);
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Middle)
            {
                this->m_moveMap = false;
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            qint32 curX = pTouchEvent->getPointer()->getPosition().x;
            qint32 curY = pTouchEvent->getPointer()->getPosition().y;
            if (this->m_moveMap)
            {                
                qint32 resX = (this->m_MoveMapMousePoint.x() - curX) * pApp->getSettings()->getMouseSensitivity();
                qint32 resY = (this->m_MoveMapMousePoint.y() - curY) * pApp->getSettings()->getMouseSensitivity();
                this->m_MoveMapMousePoint.setX(curX);
                this->m_MoveMapMousePoint.setY(curY);
                emit this->sigMoveMap(resX , resY);
            }
            m_Cursor->updatePosition(curX, curY);
        }
    });
    connect(this, SIGNAL(sigMoveMap(qint32,qint32)), this, SLOT(MoveMap(qint32,qint32)), Qt::QueuedConnection);

    GameMap::getInstance()->addChild(m_Cursor);
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

Cursor* InGameMenue::getCursor()
{
    return m_Cursor.get();
}
