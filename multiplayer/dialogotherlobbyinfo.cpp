#include <QJsonArray>

#include "multiplayer/dialogotherlobbyinfo.h"
#include "multiplayer/dialogcostatsinfo.h"
#include "multiplayer/lobbymenu.h"

#include "ui_reader/uifactory.h"

#include "network/JsonKeys.h"

DialogOtherLobbyInfo::DialogOtherLobbyInfo(LobbyMenu *pBaseMenu, const QJsonObject &objData)
    : CustomDialog("", "", pBaseMenu, tr("Close")),
      m_otherData(objData),
      m_pLobbyMenu(pBaseMenu)
{
    connect(pBaseMenu, &LobbyMenu::sigSearchedPlayersReceived, this, &DialogOtherLobbyInfo::onSearchedPlayersReceived);
    connect(pBaseMenu, &LobbyMenu::sigReceivedPlayerStats, this, &DialogOtherLobbyInfo::receivedPlayerStats);
    m_uiXml = "ui/multiplayer/lobbyOther.xml";
    loadXmlFile(m_uiXml);
}

qint32 DialogOtherLobbyInfo::getPreparingAutomatedMatchCount() const
{
    return m_otherData[JsonKeys::JSONKEY_PREPARINGAUTOMATCHES].toArray().size();
}

qint32 DialogOtherLobbyInfo::getRunningAutomatedMatchCount() const
{
    return m_otherData[JsonKeys::JSONKEY_RUNNINGAUTOMATCHES].toArray().size();
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
