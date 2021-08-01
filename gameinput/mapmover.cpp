#include "qdatetime.h"

#include "gameinput/mapmover.h"

#include "game/gamemap.h"

#include "menue/ingamemenue.h"

MapMover::MapMover(InGameMenue* pOwner)
    : m_pOwner(pOwner),
      m_scrollTimer(this)
{
    setObjectName("MapMover");
    connect(&m_scrollTimer, &QTimer::timeout, this, &MapMover::autoScroll, Qt::QueuedConnection);
    m_scrollTimer.setSingleShot(false);
    m_scrollTimer.start(100);
}

void MapMover::mouseWheel(float direction)
{
    GameMap::getInstance()->setZoom(direction);
    m_pOwner->centerMapOnCursor();
}

void MapMover::autoScroll()
{
    Mainapp* pApp = Mainapp::getInstance();
    QPoint curPos;
    bool posValid = false;
    if (pApp->hasCursor())
    {
        curPos = pApp->mapFromGlobal(pApp->cursor().pos());
        posValid = true;
    }
    else
    {
        auto sliding = m_pOwner->getMapSliding();
        auto slidingActor = m_pOwner->getMapSlidingActor();
        spGameMap pMap = GameMap::getInstance();
        if (sliding.get() != nullptr &&
            slidingActor.get() != nullptr &&
            pMap.get() != nullptr)
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
        spGameMap pMap = GameMap::getInstance();
        if (pMap.get() != nullptr)
        {
            constexpr qint64 scrollspeed = 75;
            qint64 currentTimestamp = QDateTime::currentMSecsSinceEpoch();
            if (currentTimestamp - m_lastUpdateTimestamp >= scrollspeed)
            {
                m_lastUpdateTimestamp = currentTimestamp;
                // for debugging
                Qt::Key cur = event.getKey();
                Cursor* pCursor = m_pOwner->getCursor();
                if (cur == Settings::getKey_up() ||
                    cur == Settings::getKey_up2())
                {
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
                }
                else if (cur == Settings::getKey_down() ||
                         cur == Settings::getKey_down2())
                {
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
                }
                else if (cur == Settings::getKey_left() ||
                         cur == Settings::getKey_left2())
                {
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
                }
                else if (cur == Settings::getKey_right() ||
                         cur == Settings::getKey_right2())
                {
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
                }
                else if (cur == Settings::getKey_moveMapUp() ||
                         cur == Settings::getKey_moveMapUp2())
                {
                    pMap->moveMap(0, -GameMap::getImageSize());
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
                }
                else if (cur == Settings::getKey_moveMapDown() ||
                         cur == Settings::getKey_moveMapDown2())
                {
                    pMap->moveMap(0, GameMap::getImageSize());
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
                }
                else if (cur == Settings::getKey_moveMapRight() ||
                         cur == Settings::getKey_moveMapRight2())
                {
                    pMap->moveMap(GameMap::getImageSize(), 0);
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
                }
                else if (cur == Settings::getKey_moveMapLeft() ||
                         cur == Settings::getKey_moveMapLeft2())
                {
                    pMap->moveMap(-GameMap::getImageSize(), 0);
                    m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
                }
                else if (cur == Settings::getKey_MapZoomIn() ||
                         cur == Settings::getKey_MapZoomIn2())
                {
                    pMap->setZoom(1);
                    m_pOwner->centerMapOnCursor();
                }
                else if (cur == Settings::getKey_MapZoomOut() ||
                         cur == Settings::getKey_MapZoomOut2())
                {
                    pMap->setZoom(-1);
                    m_pOwner->centerMapOnCursor();
                }
            }
        }
    }
}
