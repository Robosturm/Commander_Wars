#include "playerselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

PlayerSelectionDialog::PlayerSelectionDialog(GameMap* pMap)
{
    setObjectName("PlayerSelectionDialog");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() / 2, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event*)
    {
        emit sigPlayersChanged();
    });

    m_pPlayerSelection = spPlayerSelection::create(Settings::getWidth() - 60,
                                             Settings::getHeight() - 80 - m_OkButton->getHeight());
    m_pPlayerSelection->setMap(pMap);
    m_pPlayerSelection->setPosition(30, 30);
    m_pPlayerSelection->showPlayerSelection();
    pSpriteBox->addChild(m_pPlayerSelection);
    connect(this, &PlayerSelectionDialog::sigPlayersChanged, this, &PlayerSelectionDialog::remove, Qt::QueuedConnection);
}

void PlayerSelectionDialog::remove()
{
    detach();
}
