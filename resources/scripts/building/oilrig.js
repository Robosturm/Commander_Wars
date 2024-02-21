var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 1;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("oilrig+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("oilrig", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("oilrig+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.baseTerrain = ["LAKE", "SEA", "PLAINS", "SNOW", "WASTE", "DESERT"];
    this.getBaseTerrain = function(building)
    {
        return OILRIG.baseTerrain;
    };
	this.canBuildingBePlaced = function(terrain, building, map)
    {
        var id = terrain.getTerrainID();
        return OILRIG.baseTerrain.includes(id);
	};
    this.getBaseIncome = function()
    {
        return 3000;
    };
    this.getName = function()
    {
        return qsTr("Oil Rig");
    };

    this.getDescription = function()
    {
        return qsTr("Oil rig. Once captured produces a lot of funds, but can't repair units.");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("oilrig+snow", false);
		};
	};
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("oilrig+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var OILRIG = new Constructor();
