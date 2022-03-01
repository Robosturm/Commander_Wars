var Constructor = function()
{
    
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("radar", false);
            building.loadSpriteV2("radar+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("radar+neutral", false);
        }
        building.loadSprite("radar+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
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
        return qsTr("<r>In Fog of War, Radars make a </r><div c='#00ff00'>5 square </div><r>area around them visible.</r>");
    };

    this.getVision = function(building, map)
    {
        return 5;
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getTerrainAnimationBackground = function(unit, terrain, dfender, map)
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
}

Constructor.prototype = BUILDING;
var RADAR = new Constructor();
