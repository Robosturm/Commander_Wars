#include "ingamescriptsupport/events/scripteventchangeunitowner.h"

ScriptEventChangeUnitOwner::ScriptEventChangeUnitOwner(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::ChangeUnitOwner, ScriptEvent::EventChangeUnitOwner, tr("Change unit owner"))
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].min = 0;
    m_Items[0].infinite = -1;
    m_Items[0].tooltip = tr("X Position of the Unit");
    m_Items[0].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[0].beforeCode = "map.getTerrain(";
    m_Items[0].labelText = tr("X: ");
    m_Items[0].item = "0";
    m_Items[0].afterCode = ", ";

    m_Items[1].min = 0;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("Y Position of the Unit");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Y: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ").getUnit().setOwner(map.getPlayer(";

    m_Items[2].min = 1;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("New Owner of the Unit");
    m_Items[2].labelText = tr("Owner: ");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[2].item = "1";
    m_Items[2].beforeCode = "";
    m_Items[2].afterCode = " - 1));}";
}

void ScriptEventChangeUnitOwner::removeCustomStart(QString& text)
{
    text = text.remove(0, text.indexOf("{") + 1);
}

void ScriptEventChangeUnitOwner::writeCustomStart(QTextStream& stream)
{
    stream << "if(map.getTerrain("
           << m_Items[0].item << ", "
           << m_Items[1].item << ").getUnit() !== null){";
}
