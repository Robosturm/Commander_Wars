#include <QSize>
#include <QMutexLocker>
#include <QJsonDocument>

#include "objects/base/chat.h"

#include "menue/basegamemenu.h"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/interpreter.h"

#include "network/JsonKeys.h"

const char* const chatPlayerTarget = "@Player";
const char* const chatTeamTarget = "@Team";
const char* const chatNotTeamTarget = "@!Team";
const char* const chatAllyTarget = "@Ally";
const char* const chatEnemyTarget = "@Enemy";

Chat::Chat(spNetworkInterface pInterface, QSize size, NetworkInterface::NetworkSerives serviceMode, BaseGamemenu* pMenu)
    : m_pInterface(pInterface),
      m_serviceMode(serviceMode),
      m_pMenu(pMenu)
{
    setSize(size.width(), size.height());
#ifdef GRAPHICSUPPORT
    setObjectName("Chat");
#endif
    Interpreter::setCppOwnerShip(this);
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_Panel = spPanel::create(true, QSize(size.width(), size.height() - 50), QSize(size.width(), size.height() - 50));
    addChild(m_Panel);
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = true;
    m_Chat = oxygine::spTextField::create();
    m_Chat->setStyle(style);
    m_Chat->setPosition(10, 10);
    m_Chat->setWidth(size.width() - 10 - 60);

    m_Panel->addItem(m_Chat);

    m_Send = pObjectManager->createButton(tr("Send"), 150);
    m_Send->setPosition(size.width() - m_Send->getScaledWidth(), size.height() - m_Send->getScaledHeight());

    connect(this, &Chat::sigSendText, this, &Chat::sendData, Qt::QueuedConnection);
    addChild(m_Send);

    m_ChatInput = spTextbox::create(size.width() - m_Send->getScaledWidth() - 10);
    Chat::setVisible(true);

    m_ChatInput->setPosition(0, size.height() - m_Send->getScaledHeight());
    connect(m_ChatInput.get(), &Textbox::sigEnterPressed, this, &Chat::sendData, Qt::QueuedConnection);
    addChild(m_ChatInput);

    Textbox* pChatInput = m_ChatInput.get();
    m_Send->addEventListener(oxygine::TouchEvent::CLICK, [this, pChatInput](oxygine::Event * )->void
    {
        emit sigSendText(pChatInput->getCurrentText());
    });

    if (m_pInterface.get() != nullptr)
    {
        connect(m_pInterface.get(), &NetworkInterface::recieveData, this, &Chat::dataRecieved, NetworkCommands::UNIQUE_DATA_CONNECTION);
    }
}

void Chat::setVisible(bool vis)
{
    
    oxygine::Actor::setVisible(vis);
    if (vis)
    {
        if (m_pMenu != nullptr)
        {
            QString tooltip = tr("Message to send via chat. Start a message with one of the following items to send "
                                 "a message to specific targets. \n") +
                              chatAllyTarget+ tr(" send message to all your allies.\n") +
                              chatEnemyTarget + tr(" send message to all your enemies.\n") +
                              chatTeamTarget + "X" + tr(" send message to the given team X.\n") +
                              chatPlayerTarget + "X" + tr(" send message to the given player X.");
            m_ChatInput->setTooltipText(tooltip);
        }
        else
        {
            m_ChatInput->setTooltipText(tr("Message to send via chat."));
        }
    }
    
}

void Chat::dataRecieved(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == m_serviceMode)
    {
        CONSOLE_PRINT("Receiving chat message", GameConsole::eDEBUG);
        QJsonDocument message = QJsonDocument::fromJson(data);

        addMessage(message.object());
    }
}

void Chat::addMessage(QJsonObject data, bool local)
{
    bool show = true;

    QString textMessage = data.value(JsonKeys::JSONKEY_ChatMessage).toString();
    QString target = data.value(JsonKeys::JSONKEY_ChatTarget).toString();
    QString sender = data.value(JsonKeys::JSONKEY_ChatSender).toString();

    if (target.startsWith("@") && m_pMenu != nullptr &&
        m_pMenu->getCurrentViewPlayer() != nullptr)
    {
        if (target.startsWith(chatTeamTarget))
        {
            qint32 team = target.replace(chatTeamTarget, "").toInt();
            if (team - 1 != m_pMenu->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatAllyTarget))
        {
            qint32 team = target.replace(chatAllyTarget, "").toInt();
            if (team != m_pMenu->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatNotTeamTarget))
        {
            qint32 team = target.replace(chatNotTeamTarget, "").toInt();
            if (team == m_pMenu->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatPlayerTarget))
        {
            qint32 player = target.replace(chatPlayerTarget, "").toInt();
            if (player - 1 != m_pMenu->getCurrentViewPlayer()->getPlayerID())
            {
                show = false;
            }
        }
    }
    if (local)
    {
        show = true;
    }
    if (show)
    {
        m_messages.append(sender + ":" + textMessage);
        if (m_messages.size() > m_bufferSize)
        {
            m_messages.pop_front();
        }
        else
        {
            m_Panel->setContentHeigth(m_Chat->getTextRect().height() + 40);
        }
        if (m_Chat->getTextRect().height() > 100)
        {
            m_Panel->getH_Scrollbar()->changeScrollValue(1.0f);
        }
    }

    QString drawText;
    for(auto & message : m_messages)
    {
        drawText += "> " + message + "\n";
    }
    m_Chat->setHtmlText(drawText);
    m_Chat->setHeight(m_Chat->getTextRect().height() + 20);
}

void Chat::sendData(QString message)
{
    if (!message.isEmpty())
    {
        QJsonObject data;
        data.insert(JsonKeys::JSONKEY_ChatSender, Settings::getUsername());
        QString text;
        QString target;
        if (message.startsWith("@") &&
            m_pMenu != nullptr &&
            m_pMenu->getCurrentViewPlayer() != nullptr)
        {
            QStringList list = message.split(" ");
            for (qint32 i = 0; i < list.size(); i++)
            {
                if (i == 0)
                {
                    if (list[i] == chatEnemyTarget)
                    {
                        target = chatNotTeamTarget + QString::number(m_pMenu->getCurrentViewPlayer()->getTeam());
                    }
                    else if (list[i] == chatAllyTarget)
                    {
                        target = chatTeamTarget + QString::number(m_pMenu->getCurrentViewPlayer()->getTeam());
                    }
                    else
                    {
                        target = list[i];
                    }
                }
                else
                {
                    text += list[i] + " ";
                }
            }
            data.insert(JsonKeys::JSONKEY_ChatTarget, target);
            data.insert(JsonKeys::JSONKEY_ChatMessage, text);
        }
        else
        {
            data.insert(JsonKeys::JSONKEY_ChatTarget, "");
            data.insert(JsonKeys::JSONKEY_ChatMessage, message);
        }
        addMessage(data, true);
        if (m_messages.size() > m_bufferSize)
        {
            m_messages.pop_front();
        }
        if (m_pInterface.get() != nullptr)
        {
            CONSOLE_PRINT("Sending chat message", GameConsole::eDEBUG);
            QJsonDocument doc(data);
            emit m_pInterface->sig_sendData(0, doc.toJson(), m_serviceMode, true);
        }
        m_ChatInput->setCurrentText("");
        
    }
}
