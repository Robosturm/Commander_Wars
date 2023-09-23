#include <QJsonArray>

#include "multiplayer/dialogotherlobbyinfo.h"
#include "multiplayer/dialogcostatsinfo.h"
#include "multiplayer/lobbymenu.h"

#include "ui_reader/uifactory.h"

#include "network/JsonKeys.h"

DialogOtherLobbyInfo::DialogOtherLobbyInfo(LobbyMenu *pBaseMenu)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
      m_pLobbyMenu(pBaseMenu)
{
    connect(pBaseMenu, &LobbyMenu::sigSearchedPlayersReceived, this, &DialogOtherLobbyInfo::onSearchedPlayersReceived);
    connect(pBaseMenu, &LobbyMenu::sigReceivedPlayerStats, this, &DialogOtherLobbyInfo::receivedPlayerStats);
    connect(pBaseMenu, &LobbyMenu::sigRequestShowAutoMatches, this, &DialogOtherLobbyInfo::receivedShowAutoMatches);
    
    m_uiXml = "ui/multiplayer/lobbyOther.xml";
    loadXmlFile(m_uiXml);
}

QStringList DialogOtherLobbyInfo::getFoundPlayers()
{
    return m_foundPlayers;
}

qint32 DialogOtherLobbyInfo::getFoundPlayerSize()
{
    return m_foundPlayers.size();
}

void DialogOtherLobbyInfo::requestPlayersFromServer(const QString &searchFilter)
{
    m_pLobbyMenu->requestPlayersFromServer(searchFilter);
}

void DialogOtherLobbyInfo::onSearchedPlayersReceived(const QStringList &foundPlayers)
{
    m_foundPlayers = foundPlayers;
    refreshUi();
}

QString DialogOtherLobbyInfo::getUserName(qint32 idx)
{
    if (idx >= 0 && idx < m_foundPlayers.size())
    {
        return m_foundPlayers[idx];
    }
    return "";
}

void DialogOtherLobbyInfo::showPlayerStats(const QString &player)
{
    m_pLobbyMenu->requestPlayerStats(player);
}

void DialogOtherLobbyInfo::receivedPlayerStats(const QJsonObject &objData)
{
    auto stats = objData.value(JsonKeys::JSONKEY_COSTATS).toObject();
    spDialogCoStatsInfo pDialog = MemoryManagement::create<DialogCoStatsInfo>(m_pLobbyMenu, stats);
    addChild(pDialog);
}

void DialogOtherLobbyInfo::showAutoMatches()
{
    m_pLobbyMenu->requestShowAutoMatches();
}

void DialogOtherLobbyInfo::receivedShowAutoMatches(const QJsonObject & objData)
{

}