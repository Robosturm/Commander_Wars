#include "chat.h"

#include "QSize"

#include "QMutexLocker"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

Chat::Chat(spNetworkInterface pInterface, QSize size)
    : QObject(),
      m_pInterface(pInterface)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_Panel = new Panel(true, QSize(size.width(), size.height() - 50), QSize(size.width(), size.height() - 50));
    addChild(m_Panel);
    oxygine::TextStyle style = FontManager::getTimesFont10();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_Chat = new oxygine::TextField();
    m_Chat->setStyle(style);
    m_Chat->setPosition(10, 10);
    m_Chat->setWidth(size.width() - 10 - 60);
    m_Panel->addItem(m_Chat);

    m_Send = pObjectManager->createButton(tr("Send"), 150);
    m_Send->setPosition(size.width() - m_Send->getWidth(), size.height() - m_Send->getHeight());
    m_Send->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigSendText(m_ChatInput->getCurrentText());
    });
    connect(this, &Chat::sigSendText, this, &Chat::sendData, Qt::QueuedConnection);
    addChild(m_Send);

    m_ChatInput = new Textbox(size.width() - m_Send->getWidth() - 10);
    m_ChatInput->setPosition(0, size.height() - m_Send->getHeight());
    connect(m_ChatInput.get(), &Textbox::sigEnterPressed, this, &Chat::sendData, Qt::QueuedConnection);
    addChild(m_ChatInput);


    if (m_pInterface.get() != nullptr)
    {
        connect(m_pInterface.get(), &NetworkInterface::recieveData, this, &Chat::dataRecieved, Qt::QueuedConnection);
    }
}

void Chat::update(const oxygine::UpdateState& us)
{
    QString drawText;
    for(qint32 i = 0; i < messages.size();i++)
    {
        drawText += "> " + messages[i] + "\n";
    }
    m_Chat->setHtmlText(drawText.toStdString().c_str());
    oxygine::Actor::update(us);
}

void Chat::dataRecieved(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::Chat)
    {
        QString message(data);
        addMessage(message);
    }
}

void Chat::addMessage(QString message)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    messages.append(message);
    if (messages.size() > bufferSize)
    {
        messages.pop_front();
    }
    else
    {
        m_Panel->setContentHeigth(m_Chat->getTextRect().getHeight() + 40);
    }
    if (m_Chat->getTextRect().getHeight() > 100)
    {
        m_Panel->getH_Scrollbar()->changeScrollValue(1.0f);
    }
    pApp->continueThread();
}

void Chat::sendData(QString message)
{
    if (!message.isEmpty())
    {
        QString text = Settings::getUsername() + ": " + message;
        addMessage(text);
        if (messages.size() > bufferSize)
        {
            messages.pop_front();
        }
        if (m_pInterface.get() != nullptr)
        {
            m_pInterface->sig_sendData(0, text.toStdString().c_str(), NetworkInterface::NetworkSerives::Chat, true);
        }
        m_ChatInput->setCurrentText("");
    }
}
