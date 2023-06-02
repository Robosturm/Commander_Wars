
#include "ingamescriptsupport/events/scripteventbuildingfirecounter.h"

ScriptEventBuildingFireCounter::ScriptEventBuildingFireCounter(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::BuildingFireCounter, ScriptEvent::EventBuildingFireCounter, tr("Building fire counter"))
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 0;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("X Position of the building which should change the fire counter");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].beforeCode = "map.getTerrain(";
    m_Items[0].labelText = tr("X: ");
    m_Items[0].item = "0";
    m_Items[0].afterCode = ", ";

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("Y Position of the building which should change the fire counter");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Y: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ").getBuilding().setFireCount(";

    m_Items[2].min = -1;
    m_Items[2].infinite = -2;
    m_Items[2].tooltip = tr("The new fire counter of the building. The fire counter affects what a certain special building can do each turn.");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[2].beforeCode = "";
    m_Items[2].labelText = tr("Fire counter: ");
    m_Items[2].item = "0";
    m_Items[2].afterCode = ");";
}

