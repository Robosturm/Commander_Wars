var Constructor = function()
{
    
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("temporary_airport", false);
            building.loadSpriteV2("temporary_airport+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("temporary_airport+neutral", false);
        }
        building.loadSprite("temporary_airport+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
    };

    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("airport+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Table);
        animation.addBuildingSprite("airport", startPlayer , capturedPlayer, GameEnums.Recoloring_None);
    };
    this.getName = function()
    {
        return qsTr("Temporary Airport");
    };
    this.actionList = [];
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.constructionList = ["T_HELI", "K_HELI", "DUSTER", "TRANSPORTPLANE", "BOMBER", "FIGHTER", "STEALTHBOMBER", "BLACK_BOMB"];
    this.getConstructionList = function(building)
    {
        return TEMPORARY_AIRPORT.constructionList;
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (weatherModifier === "")
        {
            weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
        }
        return "back_" + weatherModifier + "airport";
    };
    this.getDefense = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Airport made by APC. </r><div c='#00ff00'>Resupplying </div><r>of </r><div c='#00ff00'>air </div><r>units is possible.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Air];
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("temporary_airport+snow", false);
		};
	};
}

Constructor.prototype = BUILDING;
var TEMPORARY_AIRPORT = new Constructor();
