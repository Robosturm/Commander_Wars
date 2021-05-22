#include <qcursor.h>
#include <qguiapplication.h>

#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

InGameMenue::InGameMenue()
{
    setObjectName("InGameMenue");
    m_MapMoveThread.setObjectName("MapMoveThread");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
}

InGameMenue::InGameMenue(qint32 width, qint32 heigth, QString map, bool savegame)
{
    setObjectName("InGameMenue");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        oxygine::Actor::addChild(spGameMap::create(width, heigth, 4));
    }
    else
    {
        oxygine::Actor::addChild(spGameMap::create(map, false, false, savegame));
    }
    loadHandling();

}

void InGameMenue::loadBackground()
{
    Console::print("Entering In Game Menue", Console::eDEBUG);
    // load background
    m_backgroundSprite = oxygine::spSprite::create();
    oxygine::Actor::addChild(m_backgroundSprite);
    changeBackground("gamemenu");
}

void InGameMenue::changeBackground(QString background)
{
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim(background);
    m_backgroundSprite->setResAnim(pBackground);
    // background should be last to draw
    m_backgroundSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
    m_backgroundSprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
    m_backgroundSprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
}

void InGameMenue::loadHandling()
{
    if (!m_handlingLoaded)
    {
        m_handlingLoaded = true;
        Mainapp* pApp = Mainapp::getInstance();
        addEventListener(oxygine::TouchEvent::WHEEL_DIR, [=](oxygine::Event *pEvent )->void
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                if (m_Focused)
                {
                    pEvent->stopPropagation();
                    emit sigMouseWheel(pTouchEvent->wheelDirection.y);
                }
            }
        });
        addEventListener(oxygine::TouchEvent::TOUCH_SCROLL, [=](oxygine::Event *pEvent )->void
        {
            oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                if (m_Focused)
                {
                    pEvent->stopPropagation();
                    emit sigMouseWheel(pTouchEvent->wheelDirection.y);
                }
            }
        });
        connect(this, &InGameMenue::sigMouseWheel, m_MapMover.get(), &MapMover::mouseWheel, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyDown, this, &InGameMenue::keyInput, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyUp, this, &InGameMenue::keyUp, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyDown, m_MapMover.get(), &MapMover::keyInput, Qt::QueuedConnection);
        connectMapCursor();
    }
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
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::TOUCH_DOWN_LONG, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr && Settings::getSimpleDeselect())
        {
            pEvent->stopPropagation();
            emit sigRightClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
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
        QCursor cursor = pApp->cursor();
        cursor.setShape(Qt::CursorShape::BlankCursor);
    });
    GameMap::getInstance()->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
    {
        if (!Settings::getShowCursor())
        {
            QCursor cursor = pApp->cursor();
            cursor.setShape(Qt::CursorShape::ArrowCursor);
        }
    });
    GameMap::getInstance()->addChild(m_Cursor);
}

void InGameMenue::autoScroll()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (QGuiApplication::focusWindow() == pApp &&
        m_Focused &&
        Settings::getAutoScrolling())
    {
        QPoint curPos = pApp->mapFromGlobal(pApp->cursor().pos());
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr)
        {
            qint32 moveX = 0;
            qint32 moveY = 0;
            if ((curPos.x() < m_autoScrollBorder.x()) &&
                (pMap->getX() < m_autoScrollBorder.x()))
            {
                moveX = GameMap::getImageSize() * pMap->getZoom();
            }
            else if ((curPos.x() < Settings::getWidth() - m_autoScrollBorder.width()) &&
                     (curPos.x() > Settings::getWidth() - m_autoScrollBorder.width() - 50) &&
                     (pMap->getX() + pMap->getMapWidth() * pMap->getZoom() * GameMap::getImageSize() > Settings::getWidth() - m_autoScrollBorder.width() - 50))
            {
                moveX = -GameMap::getImageSize() * pMap->getZoom();
            }

            if ((curPos.y() < m_autoScrollBorder.y()) &&
                (pMap->getY() < m_autoScrollBorder.y()))
            {
                moveY = GameMap::getImageSize() * pMap->getZoom();
            }
            else if ((curPos.y() > Settings::getHeight() - m_autoScrollBorder.height()) &&
                     (pMap->getY() + pMap->getMapHeight() * pMap->getZoom() * GameMap::getImageSize() > Settings::getHeight() - m_autoScrollBorder.height()))
            {
                moveY = -GameMap::getImageSize() * pMap->getZoom();
            }

            if (moveX != 0 || moveY != 0)
            {
                MoveMap(moveX , moveY);
            }
        }
    }
}

void InGameMenue::MoveMap(qint32 x, qint32 y)
{
    GameMap::getInstance()->moveMap(x, y);
}

void InGameMenue::setFocused(bool Focused)
{
    if (!Focused)
    {
        m_moveMap = false;
    }
    Basemenu::setFocused(Focused);
}

InGameMenue::~InGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    QCursor cursor = pApp->cursor();
    cursor.setShape(Qt::CursorShape::ArrowCursor);
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
    if (m_Focused && !event.getContinousPress())
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (cur == Settings::getKey_confirm() ||
            cur == Settings::getKey_confirm2())
        {
            emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_cancel() ||
                 cur == Settings::getKey_cancel2())
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
        if (cur == Settings::getKey_confirm() ||
            cur == Settings::getKey_confirm2())
        {
            emit sigLeftClickUp(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
        else if (cur == Settings::getKey_cancel() ||
                 cur == Settings::getKey_cancel2())
        {
            emit sigRightClickUp(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        }
    }
}

QPoint InGameMenue::getMousePos(qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 MousePosX = x * (GameMap::getImageSize() * pMap->getZoom()) + pMap->getPosition().x + (GameMap::getImageSize() * pMap->getZoom()) / 2;
        qint32 MousePosY = y * (GameMap::getImageSize() * pMap->getZoom()) + pMap->getPosition().y + (GameMap::getImageSize() * pMap->getZoom()) / 2;
        return QPoint(MousePosX, MousePosY);
    }
    return QPoint(0, 0);
}

void InGameMenue::calcNewMousePosition(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr && pMap->onMap(x, y))
    {
        QPoint mousePos = getMousePos(x, y);
        qint32 MousePosX = mousePos.x();
        qint32 MousePosY = mousePos.y();
        if (MousePosX < m_autoScrollBorder.x())
        {
            qint32 moveX = GameMap::getImageSize() * pMap->getZoom();
            pMap->moveMap(moveX, 0);
            MousePosX += moveX;
        }
        if (MousePosX > Settings::getWidth() - m_autoScrollBorder.width())
        {
            qint32 moveX = -GameMap::getImageSize() * pMap->getZoom();
            pMap->moveMap(moveX, 0);
            MousePosX += moveX;
        }

        if (MousePosY < m_autoScrollBorder.y())
        {
            qint32 moveY = GameMap::getImageSize() * pMap->getZoom();
            pMap->moveMap(0, moveY);
            MousePosY += moveY;
        }
        if (MousePosY > Settings::getHeight() - m_autoScrollBorder.height())
        {
            qint32 moveY = -GameMap::getImageSize() * pMap->getZoom();
            pMap->moveMap(0, moveY);
            MousePosY += moveY;
        }
        QPoint curPos = pApp->mapToGlobal(QPoint(MousePosX, MousePosY));
        pApp->cursor().setPos(curPos);
        emit m_Cursor->sigUpdatePosition(MousePosX, MousePosY);
    }
}

void InGameMenue::centerMapOnCursor()
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        pMap->centerMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
    }
}
