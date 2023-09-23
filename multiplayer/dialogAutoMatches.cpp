#include "multiplayer/dialogAutoMatches.h"
#include "multiplayer/lobbymenu.h"

#include "network/JsonKeys.h"

#include "resource_management/cospritemanager.h"

DialogAutoMatches::DialogAutoMatches(LobbyMenu *pBaseMenu, const QJsonObject &objData)
    : CustomDialog("", "", pBaseMenu, tr("Close"))
{
    
    m_uiXml = "ui/multiplayer/dialogAutoMatches.xml";
    loadXmlFile(m_uiXml);
}