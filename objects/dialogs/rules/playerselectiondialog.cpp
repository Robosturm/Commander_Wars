#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/rules/playerselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

PlayerSelectionDialog::PlayerSelectionDialog(GameMap* pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("PlayerSelectionDialog");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigPlayersChanged();
    });

    m_pPlayerSelection = spPlayerSelection::create(oxygine::Stage::getStage()->getWidth() - 60,
                                             oxygine::Stage::getStage()->getHeight() - 80 - m_OkButton->getScaledHeight());
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
