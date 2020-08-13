#include "scripteventplaysound.h"

ScriptEventPlaySound::ScriptEventPlaySound()
    : ScriptEventGeneric(EventType::PlayGameSound, ScriptEvent::EventPlaySound)
{

    m_Items.append(ScriptEventGeneric::Data());
    m_Items[0].tooltip = tr("Sound which should be played.");
    m_Items[0].types = ScriptEventGeneric::EditTypes::File;
    m_Items[0].beforeCode = "audio.playSound(\"";
    m_Items[0].labelText = tr("Sound File: ");
    m_Items[0].item = "";
    m_Items[0].afterCode = "\", \"\");";
    m_Items[0].filter = "*.wav";
    m_Items[0].startFolder = "/resources/sounds";
}
