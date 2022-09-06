#include "objects/dialogs/colorselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "game/gamemap.h"

ColorSelectionDialog::ColorSelectionDialog(QColor color, bool showUnitPreview)
{
#ifdef GRAPHICSUPPORT
    setObjectName("ColorSelectionDialog");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getScaledWidth() - 30,
                            Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_OkButton);

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_OkButton->getScaledHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit canceled();
    });
    qint32 pixelSize = (Settings::getHeight() - 220) / 256;
    // add spin boxes for red green and blue
    m_pColorSelector = spColorSelector::create(color, pixelSize);
    m_pColorSelector->setY(30);
    m_pColorSelector->setX(Settings::getWidth() / 2 - m_pColorSelector->getScaledWidth() / 2);
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
        m_pBuilding->setPosition(Settings::getWidth() / 2 - GameMap::getImageSize() * scale - 5, Settings::getHeight() -  GameMap::getImageSize() * scale - 5);
        m_pUnit = spUnit::create("INFANTRY", m_pPlayer.get(), false, nullptr);
        m_pUnit->setScale(scale);
        m_pUnit->setPosition(Settings::getWidth() / 2 + 5, Settings::getHeight() -  GameMap::getImageSize() * scale - 5);
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
