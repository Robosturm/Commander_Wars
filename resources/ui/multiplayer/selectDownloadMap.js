var SelectDownloadMap =
{
    curHeightPos : 10,

    updateY : function()
    {
        SelectDownloadMap.curHeightPos = lastY + lastHeight + 10
    },

    updateMapY : function()
    {
        var y = currentMenu.getMapImageHeight(loopIdx);
        if (y > SelectDownloadMap.curHeightPos)
        {
            SelectDownloadMap.curHeightPos = y;
        }
    },
    getAndUpdateMapY : function()
    {
        SelectDownloadMap.updateY();
        return lastY + lastHeight + 10;
    },

    getMapY : function()
    {
        SelectDownloadMap.updateMapY();
        return 10;
    },

    getInitialY : function()
    {
        if (loopIdx === 0)
        {
            return 10;
        }
        else
        {
            return SelectDownloadMap.curHeightPos + 10;
        }
    },
    getPageText : function()
    {
        var pageCount = currentMenu.getPageCount();
        if (pageCount > 0)
        {
            var curPage = currentMenu.getCurrentPage();
            return curPage.toString() + " / " + pageCount.toString();
        }
        else
        {
            return "-- / --";
        }
    },
}
