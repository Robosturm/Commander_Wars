var SelectDownloadMap =
{
    curHeightPos : 10,

    updateY : function(y)
    {
        SelectDownloadMap.curHeightPos = lastY + lastHeight * 2 + 10
    },

    updateMapY : function()
    {
        var y = currentMenu.getMapImageHeight();
        if (y > SelectDownloadMap.curHeightPos)
        {
            SelectDownloadMap.curHeightPos = y;
        }
    },

    getInitialY : function()
    {
        if (loopIdx === 0)
        {
            return lastY + lastHeight + 10;
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
