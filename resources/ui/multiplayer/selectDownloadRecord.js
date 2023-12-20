var SelectDownloadRecord =
{
    recordHeader : "",
    jsonMapPlayerData : "mapPlayerData",
    jsonUsername : "username",
    jsonCos : "cos",
    getInitialY : function()
    {
        SelectDownloadRecord.recordHeader = JSON.parse(currentMenu.getRecordHeader(loopIdx));
        if (loopIdx === 0)
        {
            return 10;
        }
        else
        {
            return lastY + lastHeight + 10;
        }
    },
    getPageText : function()
    {
        var pageCount = currentMenu.getPageCount();
        if (pageCount > 0)
        {
            var curPage = currentMenu.getCurrentPage() + 1;
            return curPage.toString() + " / " + pageCount.toString();
        }
        else
        {
            return "-- / --";
        }
    },
    getPlayerCount : function(loopIdx)
    {
        return SelectDownloadRecord.recordHeader[jsonMapPlayerData].length;
    },
    getPlayerName : function(loopIdx)
    {
        return SelectDownloadRecord.recordHeader[jsonMapPlayerData][loopIdx][jsonUsername];
    },
    getCoId : function(loopIdx, co)
    {
        var coid = SelectDownloadRecord.recordHeader[jsonMapPlayerData][loopIdx][jsonCos][co];
        if (coid === "")
        {
            coid = "no_co";
        }
        return coid;
    },
}
