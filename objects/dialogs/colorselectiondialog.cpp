#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/colorselectiondialog.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

ColorSelectionDialog::ColorSelectionDialog(QColor color, bool showUnitPreview)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ColorSelectionDialog");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() - m_OkButton->getScaledWidth() - 30,
                            oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, oxygine::Stage::getStage()->getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit canceled();
    });
    qint32 pixelSize = (oxygine::Stage::getStage()->getHeight() - 220) / 256;
    // add spin boxes for red green and blue
    m_pColorSelector = spColorSelector::create(color, pixelSize);
    m_pColorSelector->setY(30);
    m_pColorSelector->setX(oxygine::Stage::getStage()->getWidth() / 2 - m_pColorSelector->getScaledWidth() / 2);
    pSpriteBox->addChild(m_pColorSelector);
    if (showUnitPreview)
    {
        constexpr float scale = 2.0f;
        connect(m_pColorSelector.get(), &ColorSelector::sigSelecetedColorChanged, this, &ColorSelectionDialog::selecetedColorChanged, Qt::QueuedConnection);
        m_pPlayer = spPlayer::create(nullptr);
        m_pPlayer->setColor(color);
        m_pBuilding = spBuilding::create("TOWN", nullptr);
        m_pBuilding->setOwner(m_pPlayer.get());
        m_pBuilding->setScale(scale);
        m_pBuilding->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - GameMap::getImageSize() * scale - 5, oxygine::Stage::getStage()->getHeight() -  GameMap::getImageSize() * scale - 5);
        m_pUnit = spUnit::create("INFANTRY", m_pPlayer.get(), false, nullptr);
        m_pUnit->setScale(scale);
        m_pUnit->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 5, oxygine::Stage::getStage()->getHeight() -  GameMap::getImageSize() * scale - 5);
        pSpriteBox->addChild(m_pUnit);
        pSpriteBox->addChild(m_pBuilding);
    }
    connect(this, &ColorSelectionDialog::sigFinished, this, &ColorSelectionDialog::remove, Qt::QueuedConnection);
    connect(this, &ColorSelectionDialog::canceled, this, &ColorSelectionDialog::remove, Qt::QueuedConnection);
    auto* pColorSelector = m_pColorSelector.get();
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this, pColorSelector](oxygine::Event*)
    {
        emit editFinished(pColorSelector->getCurrentColor());
        emit sigFinished();
    });
}

void ColorSelectionDialog::remove()
{
    detach();
}

void ColorSelectionDialog::selecetedColorChanged(QColor color)
{
    m_pPlayer->setColor(color);
    m_pBuilding->setOwner(m_pPlayer.get());
    m_pUnit->setOwner(m_pPlayer.get());
}
