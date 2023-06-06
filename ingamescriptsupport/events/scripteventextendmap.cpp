#include "ingamescriptsupport/events/scripteventextendmap.h"

#include "game/GameEnums.h"

#include "coreengine/settings.h"

ScriptEventExtendMap::ScriptEventExtendMap(GameMap* pMap)
    : ScriptEventGeneric(pMap, EventType::ExtendMap, ScriptEvent::EventExtendMap, tr("Extend map"))
{
    m_Items.append(ScriptEventGeneric::Data());
    m_Items.append(ScriptEventGeneric::Data());

    m_Items[0].filter = "*.map";
    m_Items[0].startFolder = Settings::getInstance()->getUserPath() + "maps";
    m_Items[0].tooltip = tr("Map file with which the current map will be extended");
    m_Items[0].types = ScriptEventGeneric::EditTypes::File;
    m_Items[0].beforeCode = "map.extendMap(\"";
    m_Items[0].labelText = tr("Map file: ");
    m_Items[0].item = "";
    m_Items[0].afterCode = "\", ";

    m_Items[1].items = {                                 tr("North"),                                  tr("East"),                                  tr("South"),                                  tr("West")};
    m_Items[1].data = {QString::number(GameEnums::Directions_North), QString::number(GameEnums::Directions_East), QString::number(GameEnums::Directions_South), QString::number(GameEnums::Directions_West)};
    m_Items[1].tooltip = tr("Direction in which the map will be extended.");
    m_Items[1].types = ScriptEventGeneric::EditTypes::Selection;
    m_Items[1].beforeCode = "";
    m_Items[1].labelText = tr("Direction: ");
    m_Items[1].item = "0";
    m_Items[1].afterCode = ");";
}
