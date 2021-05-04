#include "multiplayer/dialogpasswordandadress.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogPasswordAndAdress::DialogPasswordAndAdress(QString text)
    : QObject()
{
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

    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText = oxygine::spTextField::create();
    pText->setHtmlText(text);
    pText->setStyle(style);
    pText->setPosition(Settings::getWidth() / 2 - pText->getTextRect().getWidth() / 2, Settings::getHeight() / 2 - 80);
    pSpriteBox->addChild(pText);

    m_pTextbox = spTextbox::create(300);
    m_pTextbox->setPosition(Settings::getWidth() / 2 - m_pTextbox->getWidth() / 2, Settings::getHeight() / 2 - 40);
    m_pTextbox->setCurrentText("");
    pSpriteBox->addChild(m_pTextbox);

    pText = oxygine::spTextField::create();
    pText->setHtmlText(tr("Password"));
    pText->setStyle(style);
    pText->setPosition(Settings::getWidth() / 2 - pText->getTextRect().getWidth() / 2, Settings::getHeight() / 2);
    pSpriteBox->addChild(pText);

    m_pPasswordbox = spPasswordbox::create(300);
    m_pPasswordbox->setPosition(Settings::getWidth() / 2 - m_pTextbox->getWidth() / 2, Settings::getHeight() / 2 + 40);
    m_pPasswordbox->setCurrentText("");
    pSpriteBox->addChild(m_pPasswordbox);


    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(Settings::getWidth() / 2 + 10,
                            Settings::getHeight() / 2 + 90);
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QString currentText = m_pTextbox->getCurrentText();
        QString password = m_pPasswordbox->getCurrentText();
        if (!currentText.isEmpty())
        {
            emit sigTextChanged(currentText, password);
            detach();
        }
    });
    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(Settings::getWidth() / 2 - m_OkButton->getWidth() - 10,
                                Settings::getHeight() / 2 + 90);
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigCancel();
        detach();
    });
}
