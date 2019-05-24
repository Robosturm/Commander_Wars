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
    GameMap::getInstance()->addChild(m_Cursor);
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
        else if (cur == Settings::getKey_confirm())
        {
            emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_cancel())
        {
            emit sigRightClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
    }
}

void InGameMenue::calcNewMousePosition(qint32 x, qint32 y)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(x, y))
    {
        qint32 MousePosX = x * (GameMap::Imagesize * pMap->getZoom()) + pMap->getPosition().x + (GameMap::Imagesize * pMap->getZoom()) / 2;
        qint32 MousePosY = y * (GameMap::Imagesize * pMap->getZoom()) + pMap->getPosition().y + (GameMap::Imagesize * pMap->getZoom()) / 2;
        SDL_WarpMouseInWindow(oxygine::core::getWindow(), MousePosX, MousePosY);
        emit m_Cursor->sigUpdatePosition(MousePosX, MousePosY);
    }
}
