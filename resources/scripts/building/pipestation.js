var Constructor = function()
{
    
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("pipestation", false);
            building.loadSpriteV2("pipestation+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("pipestation+neutral", false);
        }
    };
    this.actionList = ["ACTION_BUILD_UNITS"];
    this.constructionList = ["PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return PIPESTATION.constructionList;
    };
    this.getBaseIncome = function()
    {
        return 2000;
    };
    this.getName = function()
    {
        return qsTr("Pipestation");
    };
    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for </r><div c='#00ff00'>production and resupplying </div><r>of </r><div c='#00ff00'>pipe </div><r>units. Produces also more funds.</r>");
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
			building.loadWeatherOverlaySpriteV2("pipestation+snow", false);
		};
	};
}

Constructor.prototype = BUILDING;
var PIPESTATION = new Constructor();
