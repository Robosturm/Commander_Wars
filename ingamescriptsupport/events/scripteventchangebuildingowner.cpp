#include "ingamescriptsupport/events/scripteventchangebuildingowner.h"

ScriptEventChangeBuildingOwner::ScriptEventChangeBuildingOwner()
    : ScriptEventGeneric(EventType::ChangeBuildingOwner, ScriptEvent::EventChangeBuildingOwner)
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 0;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("X Position of the Building");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].beforeCode = "map.getTerrain(";
    m_Items[0].labelText = tr("X: ");
    m_Items[0].item = "0";
    m_Items[0].afterCode = ", ";

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("Y Position of the Building");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Y: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ").getBuilding().setOwner(map.getPlayer(";

    m_Items[2].min = 1;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("New Owner of the Building");
    m_Items[2].labelText = tr("Owner: ");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[2].item = "1";
    m_Items[2].beforeCode = "";
    m_Items[2].afterCode = " - 1));}";
}

void ScriptEventChangeBuildingOwner::removeCustomStart(QString& text)
{
    text = text.remove(0, text.indexOf("{") + 1);
}

void ScriptEventChangeBuildingOwner::writeCustomStart(QTextStream& stream)
{
    stream << "if(map.getTerrain("
           << m_Items[0].item << ", "
           << m_Items[1].item << ").getBuilding() !== null){";
}
