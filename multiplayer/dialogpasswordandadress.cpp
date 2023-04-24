#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "multiplayer/dialogpasswordandadress.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogPasswordAndAdress::DialogPasswordAndAdress(QString text)
{
    Mainapp* pApp = Mainapp::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("codialog");
    pSpriteBox->setResAnim(pAnim);
    pSpriteBox->setSize(oxygine::Stage::getStage()->getWidth(), oxygine::Stage::getStage()->getHeight());
    addChild(pSpriteBox);
    pSpriteBox->setPosition(0, 0);
    pSpriteBox->setPriority(static_cast<qint32>(Mainapp::ZOrder::Objects));
    setPriority(static_cast<qint32>(Mainapp::ZOrder::Dialogs));

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;

    oxygine::spTextField pText = oxygine::spTextField::create();
    pText->setHtmlText(text);
    pText->setStyle(style);
    pText->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pText->getTextRect().width() / 2, oxygine::Stage::getStage()->getHeight() / 2 - 80);
    pSpriteBox->addChild(pText);

    m_pTextbox = spTextbox::create(300);
    m_pTextbox->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pTextbox->getScaledWidth() / 2, oxygine::Stage::getStage()->getHeight() / 2 - 40);
    m_pTextbox->setCurrentText("");
    pSpriteBox->addChild(m_pTextbox);

    pText = oxygine::spTextField::create();
    pText->setHtmlText(tr("Password"));
    pText->setStyle(style);
    pText->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - pText->getTextRect().width() / 2, oxygine::Stage::getStage()->getHeight() / 2);
    pSpriteBox->addChild(pText);

    m_pPasswordbox = spPasswordbox::create(300);
    m_pPasswordbox->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_pTextbox->getScaledWidth() / 2, oxygine::Stage::getStage()->getHeight() / 2 + 40);
    m_pPasswordbox->setCurrentText("");
    pSpriteBox->addChild(m_pPasswordbox);


    // ok button
    m_OkButton = pObjectManager->createButton(tr("Ok"), 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                            oxygine::Stage::getStage()->getHeight() / 2 + 90);
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        QString currentText = m_pTextbox->getCurrentText();
        QString password = m_pPasswordbox->getCurrentText();
        if (!currentText.isEmpty())
        {
            emit sigTextChanged(currentText, password);
            emit sigFinished();
        }
    });
    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() - 10,
                                oxygine::Stage::getStage()->getHeight() / 2 + 90);
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigCancel();
    });
    connect(this, &DialogPasswordAndAdress::sigFinished, this, &DialogPasswordAndAdress::remove, Qt::QueuedConnection);
    connect(this, &DialogPasswordAndAdress::sigCancel, this, &DialogPasswordAndAdress::remove, Qt::QueuedConnection);
}

void DialogPasswordAndAdress::remove()
{
    detach();
}
