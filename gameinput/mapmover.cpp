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

MapMover::~MapMover()
{
}

void MapMover::mouseWheel(float direction)
{
    GameMap::getInstance()->zoom(direction);
    m_pOwner->centerMapOnCursor();
}

void MapMover::autoScroll()
{
    m_pOwner->autoScroll();
}

void MapMover::MoveMap(qint32 x, qint32 y)
{
    m_pOwner->MoveMap(x, y);
}

void MapMover::keyInput(oxygine::KeyEvent event)
{
    if (m_pOwner->getFocused())
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
                GameMap::getInstance()->moveMap(0, -GameMap::getImageSize());
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
            }
            else if (cur == Settings::getKey_moveMapDown() ||
                     cur == Settings::getKey_moveMapDown2())
            {
                GameMap::getInstance()->moveMap(0, GameMap::getImageSize());
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
            }
            else if (cur == Settings::getKey_moveMapRight() ||
                     cur == Settings::getKey_moveMapRight2())
            {
                GameMap::getInstance()->moveMap(GameMap::getImageSize(), 0);
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
            }
            else if (cur == Settings::getKey_moveMapLeft() ||
                     cur == Settings::getKey_moveMapLeft2())
            {
                GameMap::getInstance()->moveMap(-GameMap::getImageSize(), 0);
                m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
            }
            else if (cur == Settings::getKey_MapZoomIn() ||
                     cur == Settings::getKey_MapZoomIn2())
            {
                GameMap::getInstance()->zoom(1);
                m_pOwner->centerMapOnCursor();
            }
            else if (cur == Settings::getKey_MapZoomOut() ||
                     cur == Settings::getKey_MapZoomOut2())
            {
                GameMap::getInstance()->zoom(-1);
                m_pOwner->centerMapOnCursor();
            }
        }
    }
}
