#include "chat.h"

#include "QSize"

#include "QMutexLocker"

#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "menue/gamemenue.h"

static const QString chatPlayerTarget = "@Player";
static const QString chatTeamTarget = "@Team";
static const QString chatNotTeamTarget = "@!Team";
static const QString chatAllyTarget = "@Ally";
static const QString chatEnemyTarget = "@Enemy";

Chat::Chat(spNetworkInterface pInterface, QSize size)
    : QObject(),
      m_pInterface(pInterface)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    ObjectManager* pObjectManager = ObjectManager::getInstance();

    m_Panel = new Panel(true, QSize(size.width(), size.height() - 50), QSize(size.width(), size.height() - 50));
    addChild(m_Panel);
    oxygine::TextStyle style = FontManager::getMainFont16();
    style.color = FontManager::defaultColor;
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
    setVisible(true);

    m_ChatInput->setPosition(0, size.height() - m_Send->getHeight());
    connect(m_ChatInput.get(), &Textbox::sigEnterPressed, this, &Chat::sendData, Qt::QueuedConnection);
    addChild(m_ChatInput);


    if (m_pInterface.get() != nullptr)
    {
        connect(m_pInterface.get(), &NetworkInterface::recieveData, this, &Chat::dataRecieved, Qt::QueuedConnection);
    }
}

void Chat::setVisible(bool vis)
{
    oxygine::Actor::setVisible(vis);
    if (vis)
    {
        if (GameMenue::getInstance() != nullptr)
        {
            m_ChatInput->setTooltipText(tr("Message to send via chat. Start a message with one of the folling items to send ") +
                                        tr("a message to specific targets. \n") +
                                        chatAllyTarget.toStdString().c_str() + tr(" send message to all your allies.\n") +
                                        chatEnemyTarget.toStdString().c_str() + tr(" send message to all your enemies.\n") +
                                        chatTeamTarget.toStdString().c_str() + "X" + tr(" send message to the given team X.\n") +
                                        chatPlayerTarget.toStdString().c_str() + "X" + tr(" send message to the given player X."));
        }
        else
        {
            m_ChatInput->setTooltipText(tr("Message to send via chat."));
        }
    }
}

void Chat::update(const oxygine::UpdateState& us)
{
    QString drawText;
    for(qint32 i = 0; i < messages.size();i++)
    {
        drawText += "> " + messages[i] + "\n";
    }
    m_Chat->setHtmlText(drawText);
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

void Chat::addMessage(QString message, bool local)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    spGameMap pMap = GameMap::getInstance();
    bool show = true;
    if (message.startsWith("@") && pMap.get() != nullptr)
    {
        QString target = message.split(" ")[0];
        message = message.remove(0, message.indexOf(" "));
        if (target.startsWith(chatTeamTarget))
        {
            qint32 team = target.replace(chatTeamTarget, "").toInt();
            if (team - 1 != pMap->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatAllyTarget))
        {
            qint32 team = target.replace(chatAllyTarget, "").toInt();
            if (team != pMap->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatNotTeamTarget))
        {
            qint32 team = target.replace(chatNotTeamTarget, "").toInt();
            if (team == pMap->getCurrentViewPlayer()->getTeam())
            {
                show = false;
            }
        }
        else if (target.startsWith(chatPlayerTarget))
        {
            qint32 player = target.replace(chatPlayerTarget, "").toInt();
            if (player - 1 != pMap->getCurrentViewPlayer()->getPlayerID())
            {
                show = false;
            }
        }
        message = message.remove(0, message.indexOf(" "));
    }
    if (local)
    {
        show = true;
    }
    if (show)
    {
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
    }
    pApp->continueThread();
}

void Chat::sendData(QString message)
{
    if (!message.isEmpty())
    {
        QString text;
        spGameMap pMap = GameMap::getInstance();
        if (message.startsWith("@") && pMap.get() != nullptr)
        {
            QStringList list = message.split(" ");
            for (qint32 i = 0; i < list.size(); i++)
            {
                if (i == 0)
                {
                    if (list[i] == chatEnemyTarget)
                    {
                        list[i] = chatNotTeamTarget + QString::number(pMap->getCurrentViewPlayer()->getTeam());
                    }
                    else if (list[i] == chatAllyTarget)
                    {
                        list[i] = chatTeamTarget + QString::number(pMap->getCurrentViewPlayer()->getTeam());
                    }
                    text += list[i] + " " + Settings::getUsername() + ": ";
                }
                else
                {
                    text += list[i] + " ";
                }
            }
        }
        else
        {
            text = Settings::getUsername() + ": " + message;
        }

        addMessage(text, true);
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
