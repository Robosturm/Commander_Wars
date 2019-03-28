#include "ingameinfobar.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

IngameInfoBar::IngameInfoBar()
    : QObject()
{
    qint32 width = 300;
    qint32 cursorInfoHeigth = 300;
    qint32 gameInfoHeigth = 200;
    Mainapp* pApp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pMiniMapBox = new oxygine::Box9Sprite();
    pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setResAnim(pAnim);
    pMiniMapBox->setPosition(pApp->getSettings()->getWidth() - width, 0);
    pMiniMapBox->setSize(width, pApp->getSettings()->getHeight() - cursorInfoHeigth - gameInfoHeigth);
    pMiniMapBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    m_pMinimap = new Minimap();
    m_pMinimap->setPosition(0, 0);
    m_pMinimap->updateMinimap(GameMap::getInstance());
    m_pMinimap->setScale(2.0f);
    m_pMinimapSlider = new oxygine::SlidingActor();
    m_pMinimapSlider->setPosition(10, 10);
    m_pMinimapSlider->setSize(pMiniMapBox->getWidth() - 20,
                            pMiniMapBox->getHeight() - 20);
    m_pMinimapSlider->setContent(m_pMinimap);
    pMiniMapBox->addChild(m_pMinimapSlider);
    addChild(pMiniMapBox);

    oxygine::spBox9Sprite pCursorInfoBox = new oxygine::Box9Sprite();
    pCursorInfoBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pCursorInfoBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pCursorInfoBox->setResAnim(pAnim);
    pCursorInfoBox->setPosition(pApp->getSettings()->getWidth() - width, pApp->getSettings()->getHeight() - cursorInfoHeigth);
    pCursorInfoBox->setSize(width, 300);
    pCursorInfoBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));

    addChild(pCursorInfoBox);
}

void IngameInfoBar::updateMinimap()
{
    m_pMinimap->updateMinimap(GameMap::getInstance(), true);
}
