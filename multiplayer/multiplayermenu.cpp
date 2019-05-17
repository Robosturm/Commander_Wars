#include "multiplayermenu.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"
#include "coreengine/settings.h"

#include "multiplayer/lobbymenu.h"

#include "network/tcpclient.h"
#include "network/tcpserver.h"

#include "resource_management/backgroundmanager.h"
#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

Multiplayermenu::Multiplayermenu(QString adress, bool host)
    : MapSelectionMapsMenue(),
      m_Host(host)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Console::print("Entering Multiplayer Menue", Console::eDEBUG);

    if (!host)
    {
        m_NetworkInterface = new TCPClient();
        emit m_NetworkInterface->sig_connect(adress, Settings::getGamePort());
        hideRuleSelection();
        showPlayerSelection();
        m_MapSelectionStep = MapSelectionStep::selectPlayer;
        // quit on host connection lost
        connect(m_NetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &Multiplayermenu::disconnected, Qt::QueuedConnection);
    }
    else
    {
        oxygine::TextStyle style = FontManager::getMainFont();
        style.color = oxygine::Color(255, 255, 255, 255);
        style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
        style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
        style.multiline = true;

        m_pHostAdresse = new oxygine::TextField();
        addChild(m_pHostAdresse);
        m_pHostAdresse->setStyle(style);
        m_pHostAdresse->setText((tr("Host Adress: ") + NetworkInterface::getIPAdresse()).toStdString().c_str());
        m_pHostAdresse->setPosition(pApp->getSettings()->getWidth() / 2 - m_pHostAdresse->getTextRect().getWidth() / 2,
                                 pApp->getSettings()->getHeight() - m_pHostAdresse->getTextRect().getHeight() - 10);

        m_pHostAdresse->setVisible(false);
    }

}

void Multiplayermenu::playerJoined(std::shared_ptr<QTcpSocket> pSocket)
{
    if (m_NetworkInterface->getIsServer())
    {

    }
}

void Multiplayermenu::disconnected(std::shared_ptr<QTcpSocket> pSocket)
{
    if (m_Host)
    {

    }
    else
    {
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        oxygine::Actor::detach();
    }
}

void Multiplayermenu::slotButtonBack()
{
    if (!m_Host || m_MapSelectionStep == MapSelectionStep::selectMap)
    {
        Console::print("Leaving Map Selection Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new LobbyMenu());
        oxygine::Actor::detach();
    }
    else if (m_Host)
    {
        MapSelectionMapsMenue::slotButtonBack();
        m_pHostAdresse->setVisible(false);
        if (m_NetworkInterface.get() != nullptr)
        {
            m_NetworkInterface->quit();
            m_NetworkInterface->wait();
            m_NetworkInterface = nullptr;
        }
    }
}

void Multiplayermenu::slotButtonNext()
{
    MapSelectionMapsMenue::slotButtonNext();
    if (m_Host && m_MapSelectionStep == MapSelectionStep::selectPlayer)
    {
        m_pHostAdresse->setVisible(true);
        m_NetworkInterface = new TCPServer();
        emit m_NetworkInterface->sig_connect("", Settings::getGamePort());
        connect(m_NetworkInterface.get(), &NetworkInterface::sigConnected, this, &Multiplayermenu::playerJoined, Qt::QueuedConnection);
    }
}

void Multiplayermenu::startGame()
{

}
