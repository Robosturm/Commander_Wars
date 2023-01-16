#include <QDateTime>

#include "gameinput/mapmover.h"

#include "game/gamemap.h"

#include "menue/basegamemenu.h"

MapMover::MapMover(BaseGamemenu* pOwner)
    : m_pOwner(pOwner),
      m_scrollTimer(this)
{
#ifdef GRAPHICSUPPORT
    setObjectName("MapMover");
#endif
    Interpreter::setCppOwnerShip(this);
    connect(&m_scrollTimer, &QTimer::timeout, this, &MapMover::autoScroll, Qt::QueuedConnection);
    m_scrollTimer.setSingleShot(false);
    m_scrollTimer.start(100);
    Mainapp* pApp = Mainapp::getInstance();
    connect(pApp, &Mainapp::sigKeyDown, this, &MapMover::keyInput, Qt::QueuedConnection);
}

void MapMover::mouseWheel(float direction)
{    
    if (m_pOwner->getMap())
    {
        m_pOwner->getMap()->setZoom(direction);
    }
    m_pOwner->centerMapOnCursor();
}

void MapMover::autoScroll()
{
    Mainapp* pApp = Mainapp::getInstance();
    QPoint curPos;
    bool posValid = false;
    if (pApp->hasCursor())
    {
#ifdef GRAPHICSUPPORT
        curPos = pApp->mapPosFromGlobal(pApp->cursor().pos());
        posValid = true;
#endif
    }
    else
    {
        auto sliding = m_pOwner->getMapSliding();
        auto slidingActor = m_pOwner->getMapSlidingActor();
        GameMap* pMap = m_pOwner->getMap();
        if (sliding.get() != nullptr &&
            slidingActor.get() != nullptr &&
            pMap != nullptr)
        {
            auto position = sliding->getPosition() + slidingActor->getPosition() + pMap->getPosition();
            auto* pCursor = m_pOwner->getCursor();
            curPos.setX(position.x + pCursor->getMapPointX() * pMap->getImageSize() * pMap->getZoom() + pMap->getImageSize() * pMap->getZoom() / 2);
            curPos.setY(position.y + pCursor->getMapPointY() * pMap->getImageSize() * pMap->getZoom() + pMap->getImageSize() * pMap->getZoom() / 2);
            posValid = true;
        }
    }
    if (posValid)
    {
        m_pOwner->autoScroll(curPos);
    }
}

void MapMover::keyInput(oxygine::KeyEvent event)
{
    if (m_pOwner->getFocused())
    {        
        GameMap* pMap = m_pOwner->getMap();
        if (pMap != nullptr)
        {
            constexpr qint64 scrollspeed = 75;
            qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
            // for debugging
            Qt::Key cur = event.getKey();
            Cursor* pCursor = m_pOwner->getCursor();
            if ((cur == Settings::getKey_up() ||
                 cur == Settings::getKey_up2()) &&
                currentTimestamp - m_lastUpdateTimestamp[Keys::Up] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::Up] = currentTimestamp;
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
            }
            else if ((cur == Settings::getKey_down() ||
                      cur == Settings::getKey_down2()) &&
                      currentTimestamp - m_lastUpdateTimestamp[Keys::Down] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::Down] = currentTimestamp;
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
            }
            else if ((cur == Settings::getKey_left() ||
                     cur == Settings::getKey_left2()) &&
                     currentTimestamp - m_lastUpdateTimestamp[Keys::Left] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::Left] = currentTimestamp;
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
            }
            else if ((cur == Settings::getKey_right() ||
                      cur == Settings::getKey_right2()) &&
                      currentTimestamp - m_lastUpdateTimestamp[Keys::Right] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::Right] = currentTimestamp;
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
            }
            else if ((cur == Settings::getKey_moveMapUp() ||
                     cur == Settings::getKey_moveMapUp2()) &&
                currentTimestamp - m_lastUpdateTimestamp[Keys::MoveMapUp] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::MoveMapUp] = currentTimestamp;
                pMap->moveMap(0, -GameMap::getImageSize());
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
            }
            else if ((cur == Settings::getKey_moveMapDown() ||
                     cur == Settings::getKey_moveMapDown2()) &&
                    currentTimestamp - m_lastUpdateTimestamp[Keys::MoveMapDown] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::MoveMapDown] = currentTimestamp;
                pMap->moveMap(0, GameMap::getImageSize());
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
            }
            else if ((cur == Settings::getKey_moveMapRight() ||
                      cur == Settings::getKey_moveMapRight2()) &&
                     currentTimestamp - m_lastUpdateTimestamp[Keys::MoveMapRight] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::MoveMapRight] = currentTimestamp;
                pMap->moveMap(GameMap::getImageSize(), 0);
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
            }
            else if ((cur == Settings::getKey_moveMapLeft() ||
                     cur == Settings::getKey_moveMapLeft2()) &&
                     currentTimestamp - m_lastUpdateTimestamp[Keys::MoveMapLeft] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::MoveMapLeft] = currentTimestamp;
                pMap->moveMap(-GameMap::getImageSize(), 0);
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
            }
            else if ((cur == Settings::getKey_MapZoomIn() ||
                     cur == Settings::getKey_MapZoomIn2()) &&
                     currentTimestamp - m_lastUpdateTimestamp[Keys::ZoomIn] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::ZoomIn] = currentTimestamp;
                pMap->setZoom(1);
                m_pOwner->centerMapOnCursor();
            }
            else if ((cur == Settings::getKey_MapZoomOut() ||
                     cur == Settings::getKey_MapZoomOut2()) &&
                     currentTimestamp - m_lastUpdateTimestamp[Keys::ZoomOut] >= scrollspeed)
            {
                m_lastUpdateTimestamp[Keys::ZoomOut] = currentTimestamp;
                pMap->setZoom(-1);
                m_pOwner->centerMapOnCursor();
            }
        }
    }
}
