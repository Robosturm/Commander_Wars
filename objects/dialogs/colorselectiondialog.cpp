#include "colorselectiondialog.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"


ColorSelectionDialog::ColorSelectionDialog(QColor color)
    : QObject()
{
    setObjectName("ColorSelectionDialog");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(Settings::getWidth(), Settings::getHeight());
    pSpriteBox->setVerticalMode(oxygine::Box9Sprite::TILING_FULL);
    pSpriteBox->setHorizontalMode(oxygine::Box9Sprite::TILING_FULL);
    this->addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    this->setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() - m_OkButton->getWidth() - 30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit editFinished(m_pColorSelector->getCurrentColor());
        detach();
    });

    // cancel button
    m_ExitButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_ExitButton->setPosition(30, Settings::getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_ExitButton);
    m_ExitButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit canceled();
        detach();
    });
    qint32 pixelSize = (Settings::getHeight() - 220) / 256;
    // add spin boxes for red green and blue
    m_pColorSelector = spColorSelector::create(color, pixelSize);
    m_pColorSelector->setY(30);
    m_pColorSelector->setX(Settings::getWidth() / 2 - m_pColorSelector->getWidth() / 2);
    pSpriteBox->addChild(m_pColorSelector);
}
