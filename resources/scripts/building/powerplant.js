var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("powerplant", false);
            building.loadSpriteV2("powerplant+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("powerplant+neutral", false);
        }
        building.loadSprite("powerplant+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
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
}

Constructor.prototype = BUILDING;
var POWERPLANT = new Constructor();
