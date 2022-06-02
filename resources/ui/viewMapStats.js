var ViewMapStats =
{
    playerIdx : 0,
    lastId : "",
    lastPlayer : null,
    initialX : false,
    initialY : false,
    getNeutralText : function()
    {
        return qsTr("Neutral");
    },
    getTerrainText : function()
    {
        return qsTr("Terrains");
    },
    getPlayerText : function()
    {
        return qsTr("Player") + " " + (loopIdx + 1).toString();
    },
    getPlayerCount : function(viewMapStats)
    {
        return viewMapStats.getMap().getPlayerCount();
    },
    getBuildingCount : function(viewMapStats, loopIdx)
    {
        ViewMapStats.initialX = true;
        ViewMapStats.initialY = true;
        var map = viewMapStats.getMap();
        if (loopIdx >= 0)
        {
            ViewMapStats.lastPlayer = map.getPlayer(loopIdx);
        }
        else
        {
            ViewMapStats.lastPlayer = null;
        }
        return buildingSpriteManager.getBuildingCount();
    },
    getUnitCount : function()
    {
        ViewMapStats.initialX = true;
        ViewMapStats.initialY = true;
        var map = viewMapStats.getMap();
        ViewMapStats.lastPlayer = map.getPlayer(loopIdx);
        return unitSpriteManager.getUnitCount();
    },
    getTerrainCount : function()
    {
        ViewMapStats.initialX = true;
        ViewMapStats.initialY = true;
        return terrainSpriteManager.getTerrainCount();
    },
    hasBuilding : function(loopIdx)
    {
        var buildings = buildingSpriteManager.getLoadedBuildings();
        ViewMapStats.lastId = buildings[loopIdx];
        return ViewMapStats.getCountOfBuilding() > 0;
    },
    hasUnit : function(loopIdx)
    {
        var units = unitSpriteManager.getUnitsSorted();
        ViewMapStats.lastId = units[loopIdx];
        return ViewMapStats.lastPlayer.getUnitCount(ViewMapStats.lastId) > 0;
    },
    hasTerrain : function(loopIdx)
    {
        var terrains = terrainSpriteManager.getTerrainsSorted();
        ViewMapStats.lastId = terrains[loopIdx];
        return ViewMapStats.getCountOfTerrain() > 0;
    },
    getCountOfBuilding : function()
    {
        var buildingCount = 0;
        var map = viewMapStats.getMap();
        buildingCount = map.getPlayerBuildingCount(ViewMapStats.lastId, ViewMapStats.lastPlayer);
        return buildingCount;
    },
    getCountOfUnit : function()
    {
        return ViewMapStats.lastPlayer.getUnitCount(ViewMapStats.lastId);
    },
    getCountOfTerrain : function()
    {
        //
        var map = viewMapStats.getMap();
        return map.getTerrainCount(ViewMapStats.lastId);
    },
    getLastPlayer : function()
    {
        return ViewMapStats.lastPlayer;
    },
    getNextIconX : function()
    {
        var x = 10;
        if (ViewMapStats.initialX === false)
        {
            if (lastX + lastWidth + 150 > settings.getWidth() - 20)
            {
                x = 10;
            }
            else
            {
                x = lastX + lastWidth + 10;
            }
        }
        ViewMapStats.initialX = false;
        return x;
    },
    getNextIconY : function()
    {
        var y = lastY;
        if (ViewMapStats.initialY === true)
        {
            y = y + lastHeight + 30;
        }
        else
        {
            if (lastX + lastWidth + 150 > settings.getWidth() - 20)
            {
                y = lastY + lastHeight + 40;
            }
        }
        ViewMapStats.initialY = false;
        return y;
    },
    getNextLabelX : function()
    {
        return lastX + lastWidth + 5;
    },
};
