#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

#include "qcursor.h"

#include <qguiapplication.h>


InGameMenue::InGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_MapMover = new MapMover(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
}

InGameMenue::InGameMenue(qint32 width, qint32 heigth, QString map)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_MapMover = new MapMover(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
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
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    // load background
    oxygine::spSprite sprite = new oxygine::Sprite();
    oxygine::Actor::addChild(sprite);
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim("Background+0");
    sprite->setResAnim(pBackground);
    sprite->setPosition(0, 0);
    // background should be last to draw
    sprite->setPriority(static_cast<short>(Mainapp::ZOrder::Background));
    sprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    sprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
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
                emit this->sigMouseWheel(static_cast<qint32>(pTouchEvent->wheelDirection.y / 100));
            }
        }
    });
    connect(this, &InGameMenue::sigMouseWheel, m_MapMover.get(), &MapMover::mouseWheel, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, this, &InGameMenue::keyInput, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, m_MapMover.get(), &MapMover::keyInput, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyUp, this, &InGameMenue::keyUp, Qt::QueuedConnection);
    connectMapCursor();
}

void InGameMenue::connectMapCursor()
{
    Mainapp* pApp = Mainapp::getInstance();
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
                    qint32 resX = static_cast<qint32>((this->m_MoveMapMousePoint.x() - curX) * Settings::getMouseSensitivity());
                    qint32 resY = static_cast<qint32>((this->m_MoveMapMousePoint.y() - curY) * Settings::getMouseSensitivity());
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
    connect(this, &InGameMenue::sigMoveMap, m_MapMover.get(), &MapMover::MoveMap, Qt::QueuedConnection);
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
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
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
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClickUp(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            }
        }
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event *)->void
    {
        pApp->cursor().setShape(Qt::CursorShape::BlankCursor);
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
    {
        if (!Settings::getShowCursor())
        {
            pApp->cursor().setShape(Qt::CursorShape::ArrowCursor);
        }
    });
    GameMap::getInstance()->addChild(m_Cursor);
}

void InGameMenue::autoScroll()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (QGuiApplication::focusWindow() == pApp &&
        m_Focused)
    {
        QPoint curPos = pApp->mapFromGlobal(pApp->cursor().pos());
        GameMap* pMap = GameMap::getInstance();

        qint32 moveX = 0;
        qint32 moveY = 0;
        if ((curPos.x() < autoScrollBorder.x()) &&
            (pMap->getX() < autoScrollBorder.x()))
        {
            moveX = GameMap::Imagesize * pMap->getZoom();
        }
        else if ((curPos.x() < Settings::getWidth() - autoScrollBorder.width()) &&
                 (curPos.x() > Settings::getWidth() - autoScrollBorder.width() - 50) &&
                 (pMap->getX() + pMap->getMapWidth() * pMap->getZoom() * GameMap::Imagesize > Settings::getWidth() - autoScrollBorder.width() - 50))
        {
            moveX = -GameMap::Imagesize * pMap->getZoom();
        }

        if ((curPos.y() < autoScrollBorder.y()) &&
            (pMap->getY() < autoScrollBorder.y()))
        {
            moveY = GameMap::Imagesize * pMap->getZoom();
        }
        else if ((curPos.y() > Settings::getHeight() - autoScrollBorder.height()) &&
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

void InGameMenue::MoveMap(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap::getInstance()->moveMap(x, y);
    pApp->continueThread();
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
    Mainapp* pApp = Mainapp::getInstance();
    pApp->cursor().setShape(Qt::CursorShape::ArrowCursor);
    m_MapMover = nullptr;
    m_MapMoveThread.exit();
    m_MapMoveThread.wait();
}

Cursor* InGameMenue::getCursor()
{
    return m_Cursor.get();
}


void InGameMenue::keyInput(oxygine::KeyEvent event)
{
    if (m_Focused)
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (cur == Settings::getKey_confirm())
        {
            emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_cancel())
        {
            emit sigRightClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
    }
}

void InGameMenue::keyUp(oxygine::KeyEvent event)
{
    if (m_Focused)
    {
        // for debugging
        Qt::Key cur = event.getKey();
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
        QPoint curPos = pApp->mapToGlobal(QPoint(MousePosX, MousePosY));
        pApp->cursor().setPos(curPos);
        emit m_Cursor->sigUpdatePosition(MousePosX, MousePosY);
    }
    pApp->continueThread();
}
