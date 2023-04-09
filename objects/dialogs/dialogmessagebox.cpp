#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/dialogmessagebox.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogMessageBox::DialogMessageBox(QString text, bool withCancel, QString confirmText, QString cancelText)
    : QObject(),
      m_Message(text)
{
#ifdef GRAPHICSUPPORT
    setObjectName("DialogMessageBox");
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

    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;

    m_Text = oxygine::spTextField::create();
    m_Text->setSize(oxygine::Stage::getStage()->getWidth() / 2, oxygine::Stage::getStage()->getHeight());
    m_Text->setHtmlText(text);
    m_Text->setStyle(style);
    m_Text->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Text->getTextRect().width() / 2, oxygine::Stage::getStage()->getHeight() / 2 - m_Text->getTextRect().height());
    pSpriteBox->addChild(m_Text);

    m_OkButton = pObjectManager->createButton(confirmText, 150);
    m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() / 2,
                            m_Text->getY() + m_Text->getTextRect().height() + 20);
    pSpriteBox->addChild(m_OkButton);
    m_OkButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
    {
        emit sigOk();
    });

    if (withCancel)
    {
        m_CancelButton = pObjectManager->createButton(cancelText, 150);
        m_CancelButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 + 10,
                                m_Text->getY() + m_Text->getTextRect().height() + 20);
        pSpriteBox->addChild(m_CancelButton);
        m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            emit sigCancel();
        });
        m_OkButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_OkButton->getScaledWidth() - 10,
                                m_Text->getY() + m_Text->getTextRect().height() + 20);
    }

    connect(this, &DialogMessageBox::sigOk, this, &DialogMessageBox::remove, Qt::QueuedConnection);
    connect(this, &DialogMessageBox::sigCancel, this, &DialogMessageBox::remove, Qt::QueuedConnection);
}

void DialogMessageBox::remove()
{
    detach();
}

