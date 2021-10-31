var Constructor = function()
{
    
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("oilrig", false);
            building.loadSpriteV2("oilrig+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("oilrig+neutral", false);
        }
    };
    this.baseTerrain = ["SEA", "PLAINS", "STREET", "SNOW", "DESERT", "DESERT_PATH"];
    this.getBaseTerrain = function(building)
    {
        return OILRIG.baseTerrain;
    };
	this.canBuildingBePlaced = function(terrain, building)
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
	this.onWeatherChanged = function(building, weather)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("oilrig+snow", false);
		};
	};
}

Constructor.prototype = BUILDING;
var OILRIG = new Constructor();
