#include "cursor.h"

#include "SDL.h"

#include "resource_management/objectmanager.h"
#include "coreengine/mainapp.h"
#include "game/gamemap.h"

Cursor::Cursor()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    changeCursor("cursor+default");
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Cursor));
    connect(this, &Cursor::sigUpdatePosition, this, &Cursor::updatePosition, Qt::QueuedConnection);
}

void Cursor::changeCursor(const QString& spriteID, qint32 xOffset, qint32 yOffset, float scale)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    if (m_CurrentCursor.get() != nullptr)
    {
        this->removeChild(m_CurrentCursor);
        m_CurrentCursor = nullptr;
    }
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim(spriteID.toStdString());
    m_CurrentCursor = new oxygine::Sprite();
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * 200, -1);
        m_CurrentCursor->addTween(tween);
    }
    else
    {
        m_CurrentCursor->setResAnim(pAnim);
    }
    m_CurrentCursor->setScale(scale);
    m_CurrentCursor->setPosition(xOffset, yOffset);
    this->addChild(m_CurrentCursor);
    pApp->continueThread();
}

void Cursor::updatePosition(qint32 mousePosX, qint32 mousePosY)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pGameMap = GameMap::getInstance();
    qint32 x = (mousePosX - pGameMap->getPosition().x) / (GameMap::Imagesize * pGameMap->getZoom());
    qint32 y = (mousePosY - pGameMap->getPosition().y) / (GameMap::Imagesize * pGameMap->getZoom());
    onMap = true;
    // check if the mouse is still on the map
    if (x < 0)
    {
        x = 0;
        onMap = false;
    }
    if (y < 0)
    {
        y = 0;
        onMap = false;
    }
    if (x >= pGameMap->getMapWidth())
    {
        x = pGameMap->getMapWidth() - 1;
        onMap = false;
    }
    if (y >= pGameMap->getMapHeight())
    {
        y = pGameMap->getMapHeight() - 1;
        onMap = false;
    }
    // play tick sound when changing the field
    if ((x != m_MapPointX) ||
        (y != m_MapPointY))
    {
        Mainapp::getInstance()->getAudioThread()->playSound("switchfield.wav");
    }

    m_MapPointX = x;
    m_MapPointY = y;
    this->setPosition(x * GameMap::Imagesize, y * GameMap::Imagesize);
    // provide cursor move signal
    emit sigCursorMoved(m_MapPointX, m_MapPointY);
    pApp->continueThread();
}
