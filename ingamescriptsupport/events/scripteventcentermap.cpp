#include "ingamescriptsupport/events/scripteventcentermap.h"

ScriptEventCenterMap::ScriptEventCenterMap(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::CenterMap, ScriptEvent::EventCenterMap)
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 0;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("X Position on which the map gets centered");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].beforeCode = "map.centerMap(";
    m_Items[0].labelText = tr("X: ");
    m_Items[0].item = "0";
    m_Items[0].afterCode = ", ";

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("Y Position on which the map gets centered");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Y: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ");";
}
