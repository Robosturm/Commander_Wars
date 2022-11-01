#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "game/gamemap.h"
#include "game/cursor.h"
#include "game/createoutline.h"

#include "menue/basegamemenu.h"

#include "resource_management/objectmanager.h"

Cursor::Cursor(GameMap* pMap)
    : m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("Cursor");
#endif
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    changeCursor("cursor+default");
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Cursor));
    m_cursorRangeOutline = oxygine::spActor::create();
    addChild(m_cursorRangeOutline);
}

void Cursor::changeCursor(const QString & spriteID, qint32 xOffset, qint32 yOffset, float scale)
{    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (m_CurrentCursor.get() != nullptr)
    {
        m_CurrentCursor->detach();
        m_CurrentCursor = nullptr;
    }
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteID);
    m_CurrentCursor = oxygine::spSprite::create();
    if (pAnim != nullptr)
    {
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * 200), -1);
            m_CurrentCursor->addTween(tween);
        }
        else
        {
            m_CurrentCursor->setResAnim(pAnim);
        }
    }
    m_CurrentCursor->setScale(scale * static_cast<float>(GameMap::getImageSize()) / static_cast<float>(GameMap::defaultImageSize));
    m_CurrentCursor->setPosition(xOffset, yOffset);
    addChild(m_CurrentCursor);    
}

void Cursor::setMapPoint(qint32 x, qint32 y)
{    
    if (m_pMap != nullptr)
    {
        m_onMap = m_pMap->onMap(x, y);
        if (m_onMap)
        {
            // play tick sound when changing the field
            if ((x != m_MapPointX) ||
                (y != m_MapPointY))
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchfield.wav");
            }
            m_MapPointX = x;
            m_MapPointY = y;
            setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            // provide cursor move signal
            emit sigCursorMoved(m_MapPointX, m_MapPointY);
        }
    }
}

void Cursor::updatePosition(qint32 mousePosX, qint32 mousePosY)
{    
    if (m_pMap != nullptr)
    {
        auto* pMenu = m_pMap->getMenu();
        if (pMenu != nullptr)
        {
            // Mainapp* pApp = Mainapp::getInstance();
            // auto position = pMenu->getMapSlidingActor()->getPosition() + pMenu->getMapSliding()->getPosition() + m_pMap->getPosition();
            // - position.x) * pApp->getActiveDpiFactor()
            // - position.y) * pApp->getActiveDpiFactor()
            // * m_pMap->getZoom())
            // * m_pMap->getZoom())
            qint32 x = mousePosX / GameMap::getImageSize();
            qint32 y = mousePosY / GameMap::getImageSize();
            setMapPoint(x, y);
        }
    }
}

GameMap *Cursor::getMap() const
{
    return m_pMap;
}

void Cursor::addCursorRangeOutline(qint32 range, QColor color)
{
    CreateOutline::addCursorRangeOutline(m_cursorRangeOutline, "cursor+border", range, color);
}

void Cursor::resetCursorRangeOutline()
{    
    m_cursorRangeOutline->removeChildren();    
}
