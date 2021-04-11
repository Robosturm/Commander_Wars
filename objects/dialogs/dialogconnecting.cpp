#include "dialogconnecting.h"

#include "coreengine/mainapp.h"

#include "resource_management/objectmanager.h"

#include "resource_management/fontmanager.h"

DialogConnecting::DialogConnecting(QString text, qint32 timeoutMs)
    : QObject(),
      m_Message(text)
{
    setObjectName("DialogConnecting");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pSpriteBox = new oxygine::Box9Sprite();
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

    m_Text = new oxygine::TextField();
    m_Text->setHtmlText((text + ".  "));
    m_Text->setStyle(style);
    m_Text->setPosition(Settings::getWidth() / 2 - m_Text->getTextRect().getWidth() / 2, Settings::getHeight() / 2 - 40);
    pSpriteBox->addChild(m_Text);

    m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
    m_CancelButton->setPosition(Settings::getWidth() / 2 - m_CancelButton->getWidth() / 2,
                                Settings::getHeight() / 2 + 10);
    pSpriteBox->addChild(m_CancelButton);
    m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [ = ](oxygine::Event*)
    {
        emit sigCancel();
    });
    connect(this, &DialogConnecting::sigCancel, this, &DialogConnecting::cancel, Qt::QueuedConnection);
    m_Timer.setInterval(250);
    m_Timer.setSingleShot(false);
    m_Timer.start();
    m_TimerConnectionTimeout.setInterval(timeoutMs);
    m_TimerConnectionTimeout.setSingleShot(true);
    m_TimerConnectionTimeout.start();
    connect(&m_Timer, &QTimer::timeout, this, &DialogConnecting::timeout, Qt::QueuedConnection);
    connect(&m_TimerConnectionTimeout, &QTimer::timeout, this, &DialogConnecting::connectionTimeout, Qt::QueuedConnection);
}

void DialogConnecting::cancel()
{
    detach();
}

void DialogConnecting::connected()
{
    detach();
    emit sigConnected();
}

void DialogConnecting::connectionTimeout()
{
    emit sigCancel();
}

void DialogConnecting::timeout()
{
    
    counter++;
    if (counter % 3 == 0)
    {
        m_Text->setHtmlText((m_Message + ".  "));
    }
    else if (counter % 3 == 1)
    {
        m_Text->setHtmlText((m_Message + " . "));
    }
    else if (counter % 3 == 2)
    {
        m_Text->setHtmlText((m_Message + "  ."));
    }
    
}
