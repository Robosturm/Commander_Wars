#include <QJsonArray>

#include "multiplayer/dialogotherlobbyinfo.h"
#include "multiplayer/lobbymenu.h"

#include "network/JsonKeys.h"

DialogOtherLobbyInfo::DialogOtherLobbyInfo(LobbyMenu* pBaseMenu, const QJsonObject & objData)
    : CustomDialog("", "resources/ui/multiplayer/lobbyOther.xml", pBaseMenu, tr("Close")),
      m_otherData(objData)
{
}

qint32 DialogOtherLobbyInfo::getPreparingAutomatedMatchCount() const
{
    return m_otherData[JsonKeys::JSONKEY_PREPARINGAUTOMATCHES].toArray().size();
}

qint32 DialogOtherLobbyInfo::getRunningAutomatedMatchCount() const
{
    return m_otherData[JsonKeys::JSONKEY_RUNNINGAUTOMATCHES].toArray().size();
}
