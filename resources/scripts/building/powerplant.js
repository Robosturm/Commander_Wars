var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("powerplant+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("powerplant", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("powerplant+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };

    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getDefense = function()
    {
        return 2;
    };

    this.getName = function()
    {
        return qsTr("Power plant");
    };
    this.powerChargeBonus = 20;
    this.getPowerChargeBonus = function(building, map)
    {
        return POWERPLANT.powerChargeBonus;
    };
    this.getDescription = function()
    {
        var text = qsTr("A power plant that when captured increases how fast the powermeter charges by %0%.");
        text = replaceTextArgs(text, [POWERPLANT.powerChargeBonus]);
        return text;
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
			building.loadWeatherOverlaySpriteV2("powerplant+snow", false);
		};
	};
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var POWERPLANT = new Constructor();
