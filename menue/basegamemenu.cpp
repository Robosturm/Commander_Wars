#include <QCursor>
#ifdef GRAPHICSUPPORT
#include <QApplication>
#endif

#include "menue/basegamemenu.h"

#include "coreengine/interpreter.h"
#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/audiomanager.h"

#include "resource_management/backgroundmanager.h"

#include "game/gamemap.h"

BaseGamemenu::BaseGamemenu(spGameMap pMap, bool clearPlayerlist)
    : m_MapMoveThread(this),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    m_MapMoveThread.setObjectName("MapMoveThread");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    Interpreter::setCppOwnerShip(this);
    if (clearPlayerlist)
    {
        pApp->getAudioThread()->clearPlayList();
    }
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    m_Cursor = spCursor::create(m_pMap.get());
    loadBackground();
}

BaseGamemenu::BaseGamemenu(qint32 width, qint32 heigth, QString map, bool savegame)
    : m_MapMoveThread(this)
{
    Mainapp* pApp = Mainapp::getInstance();
    Interpreter::setCppOwnerShip(this);
    pApp->getAudioThread()->clearPlayList();
    m_MapMover = spMapMover::create(this);
    m_MapMover->moveToThread(&m_MapMoveThread);
    m_MapMoveThread.start();
    loadBackground();
    // check for map creation
    if ((width > 0) && (heigth > 0))
    {
        m_pMap = spGameMap::create(width, heigth, 4);
    }
    else
    {
        m_pMap = spGameMap::create(map, false, false, savegame);
    }
    m_Cursor = spCursor::create(m_pMap.get());
    loadHandling();
}

BaseGamemenu::~BaseGamemenu()
{
    CONSOLE_PRINT("Deleting BaseGamemenu", GameConsole::eDEBUG);
#ifdef GRAPHICSUPPORT
    Mainapp* pApp = Mainapp::getInstance();
    QCursor cursor = pApp->cursor();
    cursor.setShape(Qt::CursorShape::ArrowCursor);
#endif
    m_pMap->detach();
    m_pMap = nullptr;
    m_MapMover = nullptr;
    if (!m_jsName.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->deleteObject(m_jsName);
    }
    if (m_MapMoveThread.isRunning())
    {
        m_MapMoveThread.quit();
        m_MapMoveThread.wait();
    }
}

void BaseGamemenu::registerAtInterpreter(QString name)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue globals = pInterpreter->newQObject(this);
    pInterpreter->setGlobal(name, globals);
    m_jsName = name;
}

Player* BaseGamemenu::getCurrentViewPlayer()
{
    return nullptr;
}

void BaseGamemenu::loadBackground()
{
    CONSOLE_PRINT("Entering In Game Menue", GameConsole::eDEBUG);
    // load background
    m_backgroundSprite = oxygine::spSprite::create();
    oxygine::Actor::addChild(m_backgroundSprite);
    changeBackground("gamemenu");
}

void BaseGamemenu::changeBackground(QString background)
{
    BackgroundManager* pBackgroundManager = BackgroundManager::getInstance();
    oxygine::ResAnim* pBackground = pBackgroundManager->getResAnim(background);
    if (pBackground != nullptr)
    {
        m_backgroundSprite->setResAnim(pBackground);
        // background should be last to draw
        m_backgroundSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::Background));
        m_backgroundSprite->setScaleX(Settings::getWidth() / pBackground->getWidth());
        m_backgroundSprite->setScaleY(Settings::getHeight() / pBackground->getHeight());
    }
}

void BaseGamemenu::loadHandling()
{
    m_pMap->setMenu(this);
    if (!m_handlingLoaded)
    {
        m_handlingLoaded = true;
        Mainapp* pApp = Mainapp::getInstance();
        addEventListener(oxygine::TouchEvent::WHEEL_DIR, [this](oxygine::Event *pEvent )->void
        {
            oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
            if (pTouchEvent != nullptr)
            {
                if (m_Focused)
                {
                    pEvent->stopPropagation();
                    emit sigMouseWheel(pTouchEvent->wheelDirection.y);
                }
            }
        });
        connect(this, &BaseGamemenu::sigMouseWheel, m_MapMover.get(), &MapMover::mouseWheel, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyDown, this, &BaseGamemenu::keyInput, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyUp, this, &BaseGamemenu::keyUp, Qt::QueuedConnection);
        connectMapCursor();
    }
}

void BaseGamemenu::connectMapCursor()
{
    Cursor* pCursor = m_Cursor.get();
    m_pMap->addEventListener(oxygine::TouchEvent::MOVE, [this, pCursor](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
        if (pTouchEvent != nullptr)
        {
            //pEvent->stopPropagation();
            if (m_Focused)
            {
                qint32 curX = static_cast<qint32>(pTouchEvent->localPosition.x);
                qint32 curY = static_cast<qint32>(pTouchEvent->localPosition.y);
                pCursor->updatePosition(curX, curY);
            }
            else
            {
                emit sigMouseMove(pTouchEvent->localPosition.x, pTouchEvent->localPosition.y);
            }
        }
    });
    m_pMap->addEventListener(oxygine::TouchEvent::CLICK, [this, pCursor](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
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
    m_pMap->addEventListener(oxygine::TouchEvent::TOUCH_DOWN, [this, pCursor](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
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
    m_pMap->addEventListener(oxygine::TouchEvent::TOUCH_UP, [this, pCursor](oxygine::Event *pEvent )->void
    {
        oxygine::TouchEvent* pTouchEvent = oxygine::safeCast<oxygine::TouchEvent*>(pEvent);
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
#ifdef GRAPHICSUPPORT
    Mainapp* pApp = Mainapp::getInstance();
    m_pMap->addEventListener(oxygine::TouchEvent::OUTX, [pApp](oxygine::Event *)->void
    {
        QCursor cursor = pApp->cursor();
        cursor.setShape(Qt::CursorShape::BlankCursor);
    });
    m_pMap->addEventListener(oxygine::TouchEvent::OVER, [pApp](oxygine::Event *)->void
    {
        if (!Settings::getShowCursor())
        {
            QCursor cursor = pApp->cursor();
            cursor.setShape(Qt::CursorShape::ArrowCursor);
        }
    });
#endif
    m_pMap->addChild(m_Cursor);
}

GameMap* BaseGamemenu::getMap() const
{
    return m_pMap.get();
}

oxygine::spSlidingActorNoClipRect BaseGamemenu::getMapSliding() const
{
    return m_mapSliding;
}

oxygine::spActor BaseGamemenu::getMapSlidingActor() const
{
    return m_mapSlidingActor;
}

void BaseGamemenu::autoScroll(QPoint cursorPosition)
{
#ifdef GRAPHICSUPPORT
    Mainapp* pApp = Mainapp::getInstance();
    if (QApplication::focusWindow() == pApp &&
        m_Focused &&
        Settings::getAutoScrolling())
    {
        
        if (m_pMap.get() != nullptr &&
            m_mapSliding.get() != nullptr &&
            m_mapSlidingActor.get() != nullptr)
        {
            qint32 moveX = 0;
            qint32 moveY = 0;
            qint32 mapX = m_pMap->getX() + m_mapSliding->getX() + m_mapSlidingActor->getX();
            qint32 mapY = m_pMap->getY() + m_mapSliding->getY() + m_mapSlidingActor->getY();
            if ((cursorPosition.x() < m_autoScrollBorder.x()) &&
                (mapX < m_autoScrollBorder.x()))
            {
                moveX = GameMap::getImageSize() * m_pMap->getZoom();
            }
            else if ((cursorPosition.x() < Settings::getWidth() - m_autoScrollBorder.width()) &&
                     (cursorPosition.x() > Settings::getWidth() - m_autoScrollBorder.width() - 50) &&
                     (mapX + m_pMap->getMapWidth() * m_pMap->getZoom() * GameMap::getImageSize() > Settings::getWidth() - m_autoScrollBorder.width() - 50))
            {
                moveX = -GameMap::getImageSize() * m_pMap->getZoom();
            }

            if ((cursorPosition.y() < m_autoScrollBorder.y()) &&
                (mapY < m_autoScrollBorder.y()))
            {
                moveY = GameMap::getImageSize() * m_pMap->getZoom();
            }
            else if ((cursorPosition.y() > Settings::getHeight() - m_autoScrollBorder.height()) &&
                     (mapY + m_pMap->getMapHeight() * m_pMap->getZoom() * GameMap::getImageSize() > Settings::getHeight() - m_autoScrollBorder.height()))
            {
                moveY = -GameMap::getImageSize() * m_pMap->getZoom();
            }

            if (moveX != 0 || moveY != 0)
            {
                MoveMap(moveX , moveY);
            }
        }
    }
#endif
}

void BaseGamemenu::MoveMap(qint32 x, qint32 y)
{    
    if (m_pMap.get() != nullptr)
    {
        m_pMap->moveMap(x, y);
    }
}

void BaseGamemenu::setFocused(bool Focused)
{
    if (!Focused)
    {
        m_moveMap = false;
    }
    Basemenu::setFocused(Focused);
}

Cursor* BaseGamemenu::getCursor()
{
    return m_Cursor.get();
}

void BaseGamemenu::keyInput(oxygine::KeyEvent event)
{
    QPoint mapPoint = m_Cursor->getMapPoint();
    if (m_Focused && (!event.getContinousPress() || m_lastMapPoint != mapPoint))
    {
        // for debugging
        Qt::Key cur = event.getKey();
        if (cur == Settings::getKey_confirm() ||
            cur == Settings::getKey_confirm2())
        {
            emit sigLeftClick(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            m_lastMapPoint = mapPoint;
        }
        else if (cur == Settings::getKey_cancel() ||
                 cur == Settings::getKey_cancel2())
        {
            emit sigRightClickDown(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            m_lastMapPoint = mapPoint;
        }
        else if (cur == Settings::getKey_mapshot())
        {
            emit Mainapp::getInstance()->sigDoMapshot(this);
        }
        else
        {
            m_lastMapPoint = QPoint(-1, -1);
        }
    }
}

void BaseGamemenu::keyUp(oxygine::KeyEvent event)
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

QPoint BaseGamemenu::getMousePos(qint32 x, qint32 y)
{    
    if (m_pMap.get() != nullptr)
    {
        qint32 mapX = m_pMap->getX() + m_mapSliding->getX() + m_mapSlidingActor->getX();
        qint32 mapY = m_pMap->getY() + m_mapSliding->getY() + m_mapSlidingActor->getY();
        qint32 mousePosX = x * (GameMap::getImageSize() * m_pMap->getZoom()) + mapX + (GameMap::getImageSize() * m_pMap->getZoom()) / 2;
        qint32 mousePosY = y * (GameMap::getImageSize() * m_pMap->getZoom()) + mapY + (GameMap::getImageSize() * m_pMap->getZoom()) / 2;
        return QPoint(mousePosX, mousePosY);
    }
    return QPoint(0, 0);
}

void BaseGamemenu::calcNewMousePosition(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    
    if (m_pMap.get() != nullptr &&
        m_pMap->onMap(x, y) &&
        pApp->hasCursor())
    {
        QPoint mousePos = getMousePos(x, y);
        qint32 mousePosX = mousePos.x();
        qint32 mousePosY = mousePos.y();
        if (mousePosX < m_autoScrollBorder.x())
        {
            qint32 moveX = GameMap::getImageSize() * m_pMap->getZoom();
            m_pMap->moveMap(moveX, 0);
            mousePosX += moveX;
        }
        if (mousePosX > Settings::getWidth() - m_autoScrollBorder.width())
        {
            qint32 moveX = -GameMap::getImageSize() * m_pMap->getZoom();
            m_pMap->moveMap(moveX, 0);
            mousePosX += moveX;
        }

        if (mousePosY < m_autoScrollBorder.y())
        {
            qint32 moveY = GameMap::getImageSize() * m_pMap->getZoom();
            m_pMap->moveMap(0, moveY);
            mousePosY += moveY;
        }
        if (mousePosY > Settings::getHeight() - m_autoScrollBorder.height())
        {
            qint32 moveY = -GameMap::getImageSize() * m_pMap->getZoom();
            m_pMap->moveMap(0, moveY);
            mousePosY += moveY;
        }
#ifdef GRAPHICSUPPORT
        if (Settings::getAutoMoveCursor())
        {
            QPoint curPos = pApp->mapPosToGlobal(QPoint(mousePosX, mousePosY));
            pApp->cursor().setPos(curPos);
        }
#endif
        m_Cursor->updatePosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
    }
}

void BaseGamemenu::centerMapOnCursor()
{    
    if (m_pMap.get() != nullptr)
    {
        m_pMap->centerMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
    }
}

void BaseGamemenu::initSlidingActor(qint32 x, qint32 y, qint32 width, qint32 height)
{
    CONSOLE_PRINT("InGameMenue::initSlidingActor() x " + QString::number(x) + " y " + QString::number(y) + " width " + QString::number(width) + " height "  + QString::number(height), GameConsole::eDEBUG);
    if (m_mapSliding.get() == nullptr)
    {
        m_mapSliding = oxygine::spSlidingActorNoClipRect::create();
        m_mapSlidingActor = oxygine::spActor::create();
        addChild(m_mapSliding);
        m_mapSliding->setContent(m_mapSlidingActor);
        m_mapSlidingActor->addEventListener(oxygine::Draggable::DragMoveEvent, [this](oxygine::Event*)
        {
            emit m_pMap->sigMovedMap();
        });
        m_mapSliding->addEventListener(oxygine::SlidingEvent::SLIDING, [this](oxygine::Event*)
        {
            emit m_pMap->sigMovedMap();
        });
    }

    m_mapSliding->setPosition(x, y);
    m_mapSliding->setSize(width, height);
    m_mapSliding->setPriority(static_cast<qint32>(Mainapp::ZOrder::Map));
    updateSlidingActorSize();
    setFocused(true);
}

void BaseGamemenu::updateSlidingActorSize()
{
    if (m_mapSlidingActor.get() != nullptr &&
        m_mapSliding.get() != nullptr)
    {
        
        qint32 mapWidth = m_pMap->getScaledWidth();
        qint32 mapHeight = m_pMap->getScaledHeight();
        CONSOLE_PRINT("InGameMenue::updateSlidingActorSize() width " + QString::number(mapWidth) + " height "  + QString::number(mapHeight), GameConsole::eDEBUG);
        if (mapWidth < m_mapSliding->getScaledWidth())
        {
            m_mapSlidingActor->setWidth(m_mapSliding->getScaledWidth());
            m_pMap->setX((m_mapSliding->getScaledWidth() -  mapWidth) / 2);
        }
        else
        {
            m_pMap->setX(0);
            m_mapSlidingActor->setWidth(mapWidth);
        }
        if (mapHeight < m_mapSliding->getScaledHeight())
        {
            m_pMap->setY((m_mapSliding->getScaledHeight() -  mapHeight) / 2);
            m_mapSlidingActor->setHeight(m_mapSliding->getScaledHeight());
        }
        else
        {
            m_pMap->setY(0);
            m_mapSlidingActor->setHeight(mapHeight);
        }
        m_mapSliding->updateDragBounds();
        qint32 x = m_mapSlidingActor->getX();
        qint32 y = m_mapSlidingActor->getY();
        m_pMap->limitPosition(this, x, y);
        m_mapSlidingActor->setPosition(x, y);
    }
}
