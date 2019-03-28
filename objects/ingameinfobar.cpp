#include "ingameinfobar.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

IngameInfoBar::IngameInfoBar()
    : QObject()
{
    qint32 width = 300;
    Mainapp* pApp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pMiniMapBox = new oxygine::Box9Sprite();
    pMiniMapBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pMiniMapBox->setResAnim(pAnim);
    pMiniMapBox->setPosition(pApp->getSettings()->getWidth() - width, 0);
    pMiniMapBox->setSize(width, 300);
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
}

void IngameInfoBar::updateMinimap()
{
    m_pMinimap->updateMinimap(GameMap::getInstance(), true);
}
