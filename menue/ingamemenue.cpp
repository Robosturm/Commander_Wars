#include <qcursor.h>
#include <qguiapplication.h>

#include "menue/ingamemenue.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

InGameMenue* InGameMenue::m_pInstance = nullptr;

InGameMenue::InGameMenue()
{
    m_MapMoveThread.setObjectName("MapMoveThread");
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pInstance = this;
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
}

InGameMenue::InGameMenue(qint32 width, qint32 heigth, QString map, bool savegame)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->pauseRendering();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pInstance = this;
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        spGameMap::create(width, heigth, 4);
    }
    else
    {
        spGameMap::create(map, false, false, savegame);
    }
    loadHandling();
}

InGameMenue::~InGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    QCursor cursor = pApp->cursor();
    cursor.setShape(Qt::CursorShape::ArrowCursor);
    m_MapMover = nullptr;
    m_MapMoveThread.exit();
    m_MapMoveThread.wait();
    m_pInstance = nullptr;
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
    spGameMap pMap = GameMap::getInstance();
    Cursor* pCursor = m_Cursor.get();
    pMap->addEventListener(oxygine::TouchEvent::MOVE, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            //pEvent->stopPropagation();
            if (m_Focused)
            {
                qint32 curX = static_cast<qint32>(pTouchEvent->getPointer()->getPosition().x);
                qint32 curY = static_cast<qint32>(pTouchEvent->getPointer()->getPosition().y);
                emit pCursor->sigUpdatePosition(curX, curY);
            }
            else
            {
                emit sigMouseMove(pTouchEvent->localPosition.x, pTouchEvent->localPosition.y);
            }
        }
    });
    pMap->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            pEvent->stopPropagation();
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClick(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClick(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
        }
    });
    pMap->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClickDown(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClickDown(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
        }
    });
    pMap->addEventListener(oxygine::TouchEvent::TOUCH_UP, [=](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = dynamic_cast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Right)
            {
                emit sigRightClickUp(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
            else if (pTouchEvent->mouseButton == oxygine::MouseButton::MouseButton_Left)
            {
                emit sigLeftClickUp(pCursor->getMapPointX(), pCursor->getMapPointY());
            }
        }
    });
    pMap->addEventListener(oxygine::TouchEvent::OUTX, [=](oxygine::Event *)->void
    {
        QCursor cursor = pApp->cursor();
        cursor.setShape(Qt::CursorShape::BlankCursor);
    });
    pMap->addEventListener(oxygine::TouchEvent::OVER, [=](oxygine::Event *)->void
    {
        if (!Settings::getShowCursor())
        {
            QCursor cursor = pApp->cursor();
            cursor.setShape(Qt::CursorShape::ArrowCursor);
        }
    });
    pMap->addChild(m_Cursor);
}

oxygine::spSlidingActorNoClipRect InGameMenue::getMapSliding() const
{
    return m_mapSliding;
}

oxygine::spActor InGameMenue::getMapSlidingActor() const
{
    return m_mapSlidingActor;
}

void InGameMenue::autoScroll(QPoint cursorPosition)
{
    Mainapp* pApp = Mainapp::getInstance();
    if (QGuiApplication::focusWindow() == pApp &&
        m_Focused &&
        Settings::getAutoScrolling())
    {
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr &&
            m_mapSliding.get() != nullptr &&
            m_mapSlidingActor.get() != nullptr)
        {
            qint32 moveX = 0;
            qint32 moveY = 0;
            qint32 mapX = pMap->getX() + m_mapSliding->getX() + m_mapSlidingActor->getX();
            qint32 mapY = pMap->getY() + m_mapSliding->getY() + m_mapSlidingActor->getY();
            if ((cursorPosition.x() < m_autoScrollBorder.x()) &&
                (mapX < m_autoScrollBorder.x()))
            {
                moveX = GameMap::getImageSize() * pMap->getZoom();
            }
            else if ((cursorPosition.x() < Settings::getWidth() - m_autoScrollBorder.width()) &&
                     (cursorPosition.x() > Settings::getWidth() - m_autoScrollBorder.width() - 50) &&
                     (mapX + pMap->getMapWidth() * pMap->getZoom() * GameMap::getImageSize() > Settings::getWidth() - m_autoScrollBorder.width() - 50))
            {
                moveX = -GameMap::getImageSize() * pMap->getZoom();
            }

            if ((cursorPosition.y() < m_autoScrollBorder.y()) &&
                (mapY < m_autoScrollBorder.y()))
            {
                moveY = GameMap::getImageSize() * pMap->getZoom();
            }
            else if ((cursorPosition.y() > Settings::getHeight() - m_autoScrollBorder.height()) &&
                     (mapY + pMap->getMapHeight() * pMap->getZoom() * GameMap::getImageSize() > Settings::getHeight() - m_autoScrollBorder.height()))
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
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        pMap->moveMap(x, y);
    }
}

void InGameMenue::setFocused(bool Focused)
{
    if (!Focused)
    {
        m_moveMap = false;
    }
    Basemenu::setFocused(Focused);
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
        qint32 mapX = pMap->getX() + m_mapSliding->getX() + m_mapSlidingActor->getX();
        qint32 mapY = pMap->getY() + m_mapSliding->getY() + m_mapSlidingActor->getY();

        qint32 MousePosX = x * (GameMap::getImageSize() * pMap->getZoom()) + mapX + (GameMap::getImageSize() * pMap->getZoom()) / 2;
        qint32 MousePosY = y * (GameMap::getImageSize() * pMap->getZoom()) + mapY + (GameMap::getImageSize() * pMap->getZoom()) / 2;
        return QPoint(MousePosX, MousePosY);
    }
    return QPoint(0, 0);
}

void InGameMenue::calcNewMousePosition(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr && pMap->onMap(x, y) && pApp->hasCursor())
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
        if (Settings::getAutoMoveCursor())
        {
            QPoint curPos = pApp->mapToGlobal(QPoint(MousePosX, MousePosY));
            pApp->cursor().setPos(curPos);
        }
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

void InGameMenue::initSlidingActor(qint32 x, qint32 y, qint32 width, qint32 height)
{
    Console::print("InGameMenue::initSlidingActor() x " + QString::number(x) + " y " + QString::number(y) + " width " + QString::number(width) + " height "  + QString::number(height), Console::eDEBUG);
    if (m_mapSliding.get() == nullptr)
    {
        m_mapSliding = oxygine::spSlidingActorNoClipRect::create();
        m_mapSlidingActor = oxygine::spActor::create();
        addChild(m_mapSliding);
        m_mapSliding->setContent(m_mapSlidingActor);
        m_mapSlidingActor->addEventListener(oxygine::Draggable::DragMoveEvent, [=](oxygine::Event*)
        {
            emit GameMap::getInstance()->sigMovedMap();
        });
        m_mapSliding->addEventListener(oxygine::SlidingEvent::SLIDING, [=](oxygine::Event*)
        {
            emit GameMap::getInstance()->sigMovedMap();
        });
    }

    m_mapSliding->setPosition(x, y);
    m_mapSliding->setSize(width, height);
    m_mapSliding->setPriority(static_cast<qint32>(Mainapp::ZOrder::Map));
    updateSlidingActorSize();
}

void InGameMenue::updateSlidingActorSize()
{
    if (m_mapSlidingActor.get() != nullptr &&
        m_mapSliding.get() != nullptr)
    {
        spGameMap pMap = GameMap::getInstance();
        qint32 mapWidth = pMap->getScaledWidth();
        qint32 mapHeight = pMap->getScaledHeight();
        Console::print("InGameMenue::updateSlidingActorSize() width " + QString::number(mapWidth) + " height "  + QString::number(mapHeight), Console::eDEBUG);
        if (mapWidth < m_mapSliding->getWidth())
        {
            m_mapSlidingActor->setWidth(m_mapSliding->getWidth());
            pMap->setX((m_mapSliding->getWidth() -  mapWidth) / 2);
        }
        else
        {
            pMap->setX(0);
            m_mapSlidingActor->setWidth(mapWidth);
        }
        if (mapHeight < m_mapSliding->getHeight())
        {
            pMap->setY((m_mapSliding->getHeight() -  mapHeight) / 2);
            m_mapSlidingActor->setHeight(m_mapSliding->getHeight());
        }
        else
        {
            pMap->setY(0);
            m_mapSlidingActor->setHeight(mapHeight);
        }
        m_mapSliding->updateDragBounds();
        qint32 x = m_mapSlidingActor->getX();
        qint32 y = m_mapSlidingActor->getY();
        pMap->limitPosition(this, x, y);
        m_mapSlidingActor->setPosition(x, y);
    }
}
