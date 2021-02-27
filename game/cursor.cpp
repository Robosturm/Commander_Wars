#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"

#include "game/gamemap.h"
#include "game/cursor.h"
#include "game/createoutline.h"

#include "resource_management/objectmanager.h"

Cursor::Cursor()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    changeCursor("cursor+default");
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Cursor));
    m_cursorRangeOutline = new oxygine::Actor();
    addChild(m_cursorRangeOutline);
    connect(this, &Cursor::sigUpdatePosition, this, &Cursor::updatePosition, Qt::QueuedConnection);
}

void Cursor::changeCursor(QString spriteID, qint32 xOffset, qint32 yOffset, float scale)
{
    
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (m_CurrentCursor.get() != nullptr)
    {
        m_CurrentCursor->detach();
        m_CurrentCursor = nullptr;
    }
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteID);
    m_CurrentCursor = new oxygine::Sprite();
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
    this->addChild(m_CurrentCursor);
    
}

void Cursor::updatePosition(qint32 mousePosX, qint32 mousePosY)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() != nullptr)
    {
        qint32 x = (mousePosX - pMap->getPosition().x) / (GameMap::getImageSize() * pMap->getZoom());
        qint32 y = (mousePosY - pMap->getPosition().y) / (GameMap::getImageSize() * pMap->getZoom());
        onMap = pMap->onMap(x, y);
        if (onMap)
        {
            // play tick sound when changing the field
            if ((x != m_MapPointX) ||
                (y != m_MapPointY))
            {
                Mainapp::getInstance()->getAudioThread()->playSound("switchfield.wav");
            }

            m_MapPointX = x;
            m_MapPointY = y;
            this->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
            // provide cursor move signal
            emit sigCursorMoved(m_MapPointX, m_MapPointY);
            
        }
    }
}

void Cursor::addCursorRangeOutline(qint32 range, QColor color)
{
    CreateOutline::addCursorRangeOutline(m_cursorRangeOutline, "cursor+border", range, color);
}

void Cursor::resetCursorRangeOutline()
{
    
    m_cursorRangeOutline->removeChildren();
    
}
