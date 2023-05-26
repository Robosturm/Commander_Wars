
#include "ingamescriptsupport/events/scripteventvolcanfire.h"

ScriptEventVolcanFire::ScriptEventVolcanFire(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::VolcanFire, ScriptEvent::EventVolcanFire, tr("Volcan fire"))
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 0;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("X Position of the volcan which should fire.");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].beforeCode = "ZVOLCAN.volcanFire(map.getTerrain(";
    m_Items[0].labelText = tr("X: ");
    m_Items[0].item = "0";
    m_Items[0].afterCode = ", ";

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("Y Position of the volcan which should fire.");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Y: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ").getBuilding(), ";

    m_Items[2].min = 0;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("List of targets for the volcan.");
    m_Items[2].types = ScriptEventGeneric::EditTypes::PointList;
    m_Items[2].beforeCode = "";
    m_Items[2].labelText = tr("Volcan targets (x, y) : ");
    m_Items[2].item = "[Qt.Point(0,0)]";
    m_Items[2].afterCode = ");";

}

