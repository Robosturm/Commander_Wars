var SelectDownloadRecord =
{
    getInitialY : function()
    {
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
}
