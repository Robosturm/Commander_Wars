#include "mapmover.h"
#include "game/gamemap.h"
#include "menue/ingamemenue.h"

MapMover::MapMover(InGameMenue* pOwner)
    : m_pOwner(pOwner),
      scrollTimer(this)
{
    connect(&scrollTimer, &QTimer::timeout, this, &MapMover::autoScroll, Qt::QueuedConnection);
    scrollTimer.setSingleShot(false);
    scrollTimer.start(100);
}

MapMover::~MapMover()
{
}

void MapMover::mouseWheel(qint32 direction)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap::getInstance()->zoom(direction);
    pApp->continueThread();
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
        // for debugging
        Qt::Key cur = event.getKey();
        Cursor* pCursor = m_pOwner->getCursor();
        if (cur == Settings::getKey_up())
        {
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
        }
        else if (cur == Settings::getKey_down())
        {
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
        }
        else if (cur == Settings::getKey_left())
        {
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
        }
        else if (cur == Settings::getKey_right())
        {
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapUp())
        {
            GameMap::getInstance()->moveMap(0, -GameMap::Imagesize);
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() + 1);
        }
        else if (cur == Settings::getKey_moveMapDown())
        {
            GameMap::getInstance()->moveMap(0, GameMap::Imagesize);
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX(), pCursor->getMapPointY() - 1);
        }
        else if (cur == Settings::getKey_moveMapRight())
        {
            GameMap::getInstance()->moveMap(GameMap::Imagesize, 0);
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX() - 1, pCursor->getMapPointY());
        }
        else if (cur == Settings::getKey_moveMapLeft())
        {
            GameMap::getInstance()->moveMap(-GameMap::Imagesize, 0);
            m_pOwner->calcNewMousePosition(pCursor->getMapPointX() + 1, pCursor->getMapPointY());
        }
        else if (cur == Settings::getKey_MapZoomIn())
        {
            GameMap::getInstance()->zoom(1);
        }
        else if (cur == Settings::getKey_MapZoomOut())
        {
            GameMap::getInstance()->zoom(-1);
        }
    }
}
