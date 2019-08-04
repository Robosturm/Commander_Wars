#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

#include "qcursor.h"



InGameMenue::InGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    loadBackground();
    oxygine::Actor::addChild(GameMap::getInstance());
    loadHandling();
}

InGameMenue::InGameMenue(qint32 width, qint32 heigth, QString map)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    loadBackground();
    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        oxygine::Actor::addChild(new GameMap(width, heigth, 4));
    }
    else
    {
        oxygine::Actor::addChild(new GameMap(map, false));
    }
    loadHandling();

}

void InGameMenue::loadBackground()
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
}

void InGameMenue::loadHandling()
{
    Mainapp* pApp = Mainapp::getInstance();
    addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (m_Focused)
            {
                pEvent->stopPropagation();
                emit this->sigMouseWheel(static_cast<qint32>(pTouchEvent->wheelDirection.y));
            }
        }
    });
    connect(this, &InGameMenue::sigMouseWheel, this, &InGameMenue::mouseWheel, Qt::QueuedConnection);
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Middle)
            {
                if (m_Focused)
                {
                    pEvent->stopPropagation();
                    this->m_moveMap = true;
                    this->m_MoveMapMousePoint.setX(static_cast<qint32>(pTouchEvent->getPointer()->getPosition().x));
                    this->m_MoveMapMousePoint.setY(static_cast<qint32>(pTouchEvent->getPointer()->getPosition().y));
                }
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
                pEvent->stopPropagation();
                this->m_moveMap = false;
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {            
            if (m_Focused)
            {
                pEvent->stopPropagation();
                qint32 curX = static_cast<qint32>(pTouchEvent->getPointer()->getPosition().x);
                qint32 curY = static_cast<qint32>(pTouchEvent->getPointer()->getPosition().y);

                if (this->m_moveMap)
                {
                    qint32 resX = static_cast<qint32>((this->m_MoveMapMousePoint.x() - curX) * pApp->getSettings()->getMouseSensitivity());
                    qint32 resY = static_cast<qint32>((this->m_MoveMapMousePoint.y() - curY) * pApp->getSettings()->getMouseSensitivity());
                    this->m_MoveMapMousePoint.setX(curX);
                    this->m_MoveMapMousePoint.setY(curY);
                    emit this->sigMoveMap(resX , resY);
                }
                emit m_Cursor->sigUpdatePosition(curX, curY);
            }
            else
            {
                emit sigMouseMove(pTouchEvent->localPosition.x, pTouchEvent->localPosition.y);
            }
        }
    });
    connect(this, &InGameMenue::sigMoveMap, this, &InGameMenue::MoveMap, Qt::QueuedConnection);

    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            pEvent->stopPropagation();
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            pEvent->stopPropagation();
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            pEvent->stopPropagation();
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClickUp(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event *)->void
    {
        SDL_ShowCursor(SDL_ENABLE);
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
    {
        if (!Settings::getShowCursor())
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
    });

    connect(pApp, &Mainapp::sigKeyDown, this, &InGameMenue::keyInput, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyUp, this, &InGameMenue::keyUp, Qt::QueuedConnection);
    GameMap::getInstance()->addChild(m_Cursor);

    connect(&scrollTimer, &QTimer::timeout, this, &InGameMenue::autoScroll, Qt::QueuedConnection);
    scrollTimer.setSingleShot(false);
    scrollTimer.start(100);
}

void InGameMenue::autoScroll()
{
    if (SDL_GetWindowFlags(oxygine::core::getWindow()) & SDL_WINDOW_MOUSE_FOCUS &&
        m_Focused)
    {
        qint32 curX = 0;
        qint32 curY = 0;
        SDL_GetMouseState(&curX, &curY);
        GameMap* pMap = GameMap::getInstance();

        qint32 moveX = 0;
        qint32 moveY = 0;
        if ((curX < autoScrollBorder.x()) &&
            (pMap->getX() < autoScrollBorder.x()))
        {
            moveX = GameMap::Imagesize * pMap->getZoom();
        }
        if ((curX > Settings::getWidth() - autoScrollBorder.width()) &&
            (curX < Settings::getWidth() - autoScrollBorder.width() + 100) &&
            (pMap->getX() + pMap->getMapWidth() * pMap->getZoom() * GameMap::Imagesize > Settings::getWidth() - autoScrollBorder.width()))

        {
            moveX = -GameMap::Imagesize * pMap->getZoom();
        }
        if ((curY < autoScrollBorder.y()) &&
            (pMap->getY() < autoScrollBorder.y()))
        {
            moveY = GameMap::Imagesize * pMap->getZoom();
        }
        if ((curY > Settings::getHeight() - autoScrollBorder.height()) &&
            (pMap->getY() + pMap->getMapHeight() * pMap->getZoom() * GameMap::Imagesize > Settings::getHeight() - autoScrollBorder.height()))
        {
            moveY = -GameMap::Imagesize * pMap->getZoom();
        }
        if (moveX != 0 || moveY != 0)
        {
            MoveMap(moveX , moveY);
        }
    }
}

bool InGameMenue::getFocused() const
{
    return m_Focused;
}

void InGameMenue::setFocused(bool Focused)
{
    if (!Focused)
    {
        m_moveMap = false;
    }
    m_Focused = Focused;
}

InGameMenue::~InGameMenue()
{
    SDL_ShowCursor(SDL_ENABLE);
}

void InGameMenue::mouseWheel(qint32 direction)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap::getInstance()->zoom(direction);
    pApp->continueThread();
}

void InGameMenue::MoveMap(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap::getInstance()->moveMap(x, y);
    pApp->continueThread();
}

Cursor* InGameMenue::getCursor()
{
    return m_Cursor.get();
}

void InGameMenue::keyInput(SDL_Event event)
{
    if (m_Focused)
    {
        // for debugging
        SDL_Keycode cur = event.key.keysym.sym;
        if (cur == Settings::getKey_up())
        {
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY() - 1);
        }
        else if (cur == Settings::getKey_down())
        {
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY() + 1);
        }
        else if (cur == Settings::getKey_left())
        {
            calcNewMousePosition(m_Cursor->getMapPointX() - 1, m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_right())
        {
            calcNewMousePosition(m_Cursor->getMapPointX() + 1, m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapUp())
        {
            GameMap::getInstance()->moveMap(0, -GameMap::Imagesize);
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapDown())
        {
            GameMap::getInstance()->moveMap(0, GameMap::Imagesize);
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapRight())
        {
            GameMap::getInstance()->moveMap(GameMap::Imagesize, 0);
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapLeft())
        {
            GameMap::getInstance()->moveMap(-GameMap::Imagesize, 0);
            calcNewMousePosition(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_confirm())
        {
            emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_cancel())
        {
            emit sigRightClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
    }
}

void InGameMenue::keyUp(SDL_Event event)
{
    if (m_Focused)
    {
        // for debugging
        SDL_Keycode cur = event.key.keysym.sym;
        if (cur == Settings::getKey_cancel())
        {
            emit sigRightClickUp(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
    }
}

void InGameMenue::calcNewMousePosition(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        qint32 MousePosX = x * (GameMap::Imagesize * pMap->getZoom()) + pMap->getPosition().x + (GameMap::Imagesize * pMap->getZoom()) / 2;
        if (MousePosX < autoScrollBorder.x())
        {
            qint32 moveX = GameMap::Imagesize * pMap->getZoom();
            pMap->moveMap(moveX, 0);
            MousePosX += moveX;
        }
        if (MousePosX > Settings::getWidth() - autoScrollBorder.width())
        {
            qint32 moveX = -GameMap::Imagesize * pMap->getZoom();
            pMap->moveMap(moveX, 0);
            MousePosX += moveX;
        }
        qint32 MousePosY = y * (GameMap::Imagesize * pMap->getZoom()) + pMap->getPosition().y + (GameMap::Imagesize * pMap->getZoom()) / 2;
        if (MousePosY < autoScrollBorder.y())
        {
            qint32 moveY = GameMap::Imagesize * pMap->getZoom();
            pMap->moveMap(0, moveY);
            MousePosY += moveY;
        }
        if (MousePosY > Settings::getHeight() - autoScrollBorder.height())
        {
            qint32 moveY = -GameMap::Imagesize * pMap->getZoom();
            pMap->moveMap(0, moveY);
            MousePosY += moveY;
        }
        SDL_WarpMouseInWindow(oxygine::core::getWindow(), MousePosX, MousePosY);
        emit m_Cursor->sigUpdatePosition(MousePosX, MousePosY);
    }
    pApp->continueThread();
}
