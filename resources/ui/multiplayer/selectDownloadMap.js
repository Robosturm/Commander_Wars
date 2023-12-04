var SelectDownloadMap =
{
    curHeightPos : 10,
    nextHeightPos : 10,
    updateMapY : function()
    {
        var y = currentMenu.getMapImageHeight(loopIdx) + SelectDownloadMap.curHeightPos + 10;
        if (y > lastY + lastHeight + 10)
        {
            SelectDownloadMap.nextHeightPos = y;
        }
        else
        {
            SelectDownloadMap.nextHeightPos = lastY + lastHeight + 10;
        }
    },
    getMapY : function()
    {
        SelectDownloadMap.updateMapY();
        if (loopIdx === 0)
        {
            return 10;
        }
        else
        {
            return SelectDownloadMap.curHeightPos + 10;
        }
    },
    getInitialY : function()
    {
        if (loopIdx === 0)
        {
            SelectDownloadMap.curHeightPos = 10;
            SelectDownloadMap.nextHeightPos = 10;
            return 10;
        }
        else
        {
            SelectDownloadMap.curHeightPos = SelectDownloadMap.nextHeightPos;
            return SelectDownloadMap.curHeightPos + 10;
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
}
