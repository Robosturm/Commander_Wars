#include "fieldinfo.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "objects/panel.h"

#include "wiki/terraininfo.h"

#include "wiki/unitinfo.h"

FieldInfo::FieldInfo(Terrain* pTerrain)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(pApp->getSettings()->getWidth(), pApp->getSettings()->getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<short>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() / 2 - m_OkButton->getWidth() / 2, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigFinished();
        this->getParent()->removeChild(this);
    });

    // no the fun begins create checkboxes and stuff and a panel down here
    spPanel pPanel = new Panel(true, QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110),
                                     QSize(pApp->getSettings()->getWidth() - 60, pApp->getSettings()->getHeight() - 110));
    pPanel->setPosition(30, 30);
    pSpriteBox->addChild(pPanel);
    spTerrainInfo pTerrainInfo = new TerrainInfo(pTerrain, pPanel->getWidth() - 40);
    pTerrainInfo->setPosition(20, 10);
    pPanel->addItem(pTerrainInfo);
    pPanel->setContentHeigth(pTerrainInfo->getY() + pTerrainInfo->getHeight());

    Unit* pUnit = pTerrain->getUnit();
    if (pUnit != nullptr)
    {
        spUnitInfo pUnitInfo = new UnitInfo(pUnit, pPanel->getWidth() - 40);
        pUnitInfo->setPosition(20, pTerrainInfo->getY() + pTerrainInfo->getHeight() + 10);
        pPanel->addItem(pUnitInfo);
        pPanel->setContentHeigth(pUnitInfo->getY() + pUnitInfo->getHeight());
    }

    connect(pApp, &Mainapp::sigKeyDown, this, &FieldInfo::keyInput, Qt::QueuedConnection);
}

void FieldInfo::keyInput(SDL_Event event)
{
    // for debugging
    SDL_Keycode cur = event.key.keysym.sym;
    if (cur == Settings::getKey_information())
    {
        emit sigFinished();
        this->getParent()->removeChild(this);
    }
}
