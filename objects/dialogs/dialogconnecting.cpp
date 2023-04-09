#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

#include "objects/dialogs/dialogconnecting.h"

#include "coreengine/interpreter.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

DialogConnecting::DialogConnecting(QString text, qint32 timeoutMs, bool showCancel)
    : QObject(),
      m_Message(text),
      m_Timer(this),
      m_TimerConnectionTimeout(this)
{
    CONSOLE_PRINT("Creating DialogConnecting", GameConsole::eDEBUG);
#ifdef GRAPHICSUPPORT
    setObjectName("DialogConnecting");
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
    style.multiline = false;

    m_Text = oxygine::spTextField::create();
    m_Text->setHtmlText((text + ".  "));
    m_Text->setStyle(style);
    m_Text->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_Text->getTextRect().width() / 2, oxygine::Stage::getStage()->getHeight() / 2 - 40);
    pSpriteBox->addChild(m_Text);

    if (showCancel)
    {
        m_CancelButton = pObjectManager->createButton(tr("Cancel"), 150);
        m_CancelButton->setPosition(oxygine::Stage::getStage()->getWidth() / 2 - m_CancelButton->getScaledWidth() / 2,
                                    oxygine::Stage::getStage()->getHeight() / 2 + 10);
        pSpriteBox->addChild(m_CancelButton);
        m_CancelButton->addEventListener(oxygine::TouchEvent::CLICK, [this](oxygine::Event*)
        {
            emit sigCancel();
        });
        connect(this, &DialogConnecting::sigCancel, this, &DialogConnecting::cancel, Qt::QueuedConnection);
    }
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
    CONSOLE_PRINT("Canceling DialogConnecting", GameConsole::eDEBUG);
    detach();
}

void DialogConnecting::connected()
{
    CONSOLE_PRINT("Connected in DialogConnecting", GameConsole::eDEBUG);
    emit sigConnected();
    detach();
}

void DialogConnecting::connectionTimeout()
{
    emit sigCancel();
}

void DialogConnecting::timeout()
{
    
    m_counter++;
    if (m_counter % 3 == 0)
    {
        m_Text->setHtmlText((m_Message + ".  "));
    }
    else if (m_counter % 3 == 1)
    {
        m_Text->setHtmlText((m_Message + " . "));
    }
    else if (m_counter % 3 == 2)
    {
        m_Text->setHtmlText((m_Message + "  ."));
    }
    
}
