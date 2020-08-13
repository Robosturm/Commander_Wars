#include "scripteventplaysound.h"

ScriptEventPlaySound::ScriptEventPlaySound()
    : ScriptEventGeneric(EventType::PlayGameSound, ScriptEvent::EventPlaySound)
{

    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());

    m_Items[0].tooltip = tr("Sound which should be played.");
    m_Items[0].types = ScriptEventGeneric::EditTypes::File;
    m_Items[0].beforeCode = "audio.playSound(\"";
    m_Items[0].labelText = tr("Sound File: ");
    m_Items[0].item = "";
    m_Items[0].afterCode = "\", ";
    m_Items[0].filter = "*.wav";
    m_Items[0].startFolder = "/resources/sounds";

    m_Items[1].min = 1;
    m_Items[1].infinite = -1;
    m_Items[1].tooltip = tr("How often the sound is getting played");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Loops: ");
    m_Items[1].item = "1";
    m_Items[1].afterCode = ", \"\",";

    m_Items[2].min = 0;
    m_Items[2].infinite = -1;
    m_Items[2].tooltip = tr("Time in ms before the sound is getting played");
    m_Items[2].types = ScriptEventGeneric::EditTypes::Integer;
    m_Items[2].beforeCode = "";
    m_Items[2].labelText = tr("Delay: ");
    m_Items[2].item = "0";
    m_Items[2].afterCode = ");";
}
