var CoStatsInfo =
{    
    curYPos : 10,
    getNextX : function()
    {
        
        var x = lastX + lastWidth + 10;
        if (x + 320 > parentWidth - 60)
        {
            x = 10;
            CoStatsInfo.curYPos = lastY + lastHeight + 10;
        }
        return x;
    },

    getNextY : function()
    {
        if (loopIdx === 0)
        {
            CoStatsInfo.curYPos = 10;
        }
        return CoStatsInfo.curYPos;
    },
}