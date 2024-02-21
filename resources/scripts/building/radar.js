var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("radar+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("radar", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("radar+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getVisionBonus = function()
    {
        return 0;
    };

    this.getName = function()
    {
        return qsTr("Radar");
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War, Radars make a </r><div c='#00ff00'>3 square </div><r>area around them visible.</r>");
    };

    this.getVision = function(building, map)
    {
        return 3 + map.getGameRules().getBuildingVision();
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_radar";
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("radar+snow", false);
		};
	};
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("radar+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
}

Constructor.prototype = BUILDING;
var RADAR = new Constructor();
