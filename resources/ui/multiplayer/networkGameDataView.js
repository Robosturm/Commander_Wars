var NetworkGameDataView =
{
    getOk : function()
    {
        return qsTr("Ok");
    },
    getOkTooltip : function()
    {
        return qsTr("Closes the game data view");
    },
    getOnlineText : function(loopIdx)
    {
        if (currentMenu.getPlayerIsOnline(loopIdx))
        {
            return qsTr("Online");
        }
        else
        {
            return qsTr("Offline");
        }
    },
}
