#include "scripteventchangeunitai.h"

ScriptEventChangeUnitAI::ScriptEventChangeUnitAI()
    : ScriptEventGeneric(EventType::ChangeUnitAI, ScriptEvent::EventChangeUnitAI)
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
    m_Items[1].afterCode = ").getUnit().setAiMode(";

    m_Items[2].min = 1;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("New AI Mode for the Unit");
    m_Items[2].labelText = tr("AI Mode: ");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Selection;
    m_Items[2].item = "GameEnums.GameAi_Normal";
    m_Items[2].beforeCode = "";
    m_Items[2].afterCode = ");}";

    m_Items[2].data.append("GameEnums.GameAi_Normal");
    m_Items[2].data.append("GameEnums.GameAi_Offensive");
    m_Items[2].data.append("GameEnums.GameAi_Defensive");
    m_Items[2].data.append("GameEnums.GameAi_Hold");

    m_Items[2].items.append(tr("Normal"));
    m_Items[2].items.append(tr("Offensive"));
    m_Items[2].items.append(tr("Defensive"));
    m_Items[2].items.append(tr("Hold"));
}

void ScriptEventChangeUnitAI::removeCustomStart(QString& text)
{
    text = text.remove(0, text.indexOf("{") + 1);
}

void ScriptEventChangeUnitAI::writeCustomStart(QTextStream& stream)
{
    stream << "if(map.getTerrain("
           << m_Items[0].item << ", "
           << m_Items[1].item << ").getUnit() !== null){";
}
