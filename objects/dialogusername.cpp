#include "dialogusername.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

DialogUsername::DialogUsername()
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

    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    oxygine::spTextField pText = new oxygine::TextField();
    pText->setText(tr("Select your Username").toStdString().c_str());
    pText->setStyle(style);
    pText->setPosition(pApp->getSettings()->getWidth() / 2 - pText->getTextRect().getWidth() / 2, pApp->getSettings()->getHeight() / 2 - 40);
    pSpriteBox->addChild(pText);

    m_pTextbox = new Textbox(300);
    m_pTextbox->setPosition(pApp->getSettings()->getWidth() / 2 - m_pTextbox->getWidth() / 2, pApp->getSettings()->getHeight() / 2);
    m_pTextbox->setCurrentText(Settings::getUsername());
    pSpriteBox->addChild(m_pTextbox);

    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(pApp->getSettings()->getWidth() - m_OkButton->getWidth() - 30, pApp->getSettings()->getHeight() - 30 - m_OkButton->getHeight());
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        QString username = m_pTextbox->getCurrentText();
        if (!username.isEmpty())
        {
            Settings::setUsername(username);
            this->getParent()->removeChild(this);
        }
    });
}
