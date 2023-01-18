#include "ingamescriptsupport/events/scripteventchangeplayerteam.h"

ScriptEventChangePlayerTeam::ScriptEventChangePlayerTeam(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::ChangePlayerTeam, ScriptEvent::EventChangePlayerTeam, tr("Change player team"))
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 1;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("Player to change the team.");
    m_Items[0].labelText = tr("Player: ");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].item = "1";
    m_Items[0].beforeCode = "map.getPlayer(";
    m_Items[0].afterCode = " - 1).setTeam(";

    m_Items[1].min = 1;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("New Team of the Player");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Team: ");
    m_Items[1].item = "1";
    m_Items[1].afterCode = " - 1);";
}
