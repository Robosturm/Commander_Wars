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
            building.loadSprite("temporary_harbour", false);
            building.loadSpriteV2("temporary_harbour+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("temporary_harbour+neutral", false);
        }
        building.loadSprite("temporary_harbour+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("harbour+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Table);
        animation.addBuildingSprite("harbour", startPlayer , capturedPlayer, GameEnums.Recoloring_None);
    };
    this.getName = function()
    {
        return qsTr("Temporary Harbour");
    };
    this.actionList = [];
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.constructionList = ["CANNONBOAT", "BLACK_BOAT", "LANDER", "CRUISER", "SUBMARINE", "DESTROYER", "BATTLESHIP", "AIRCRAFTCARRIER"];
    this.getConstructionList = function(building)
    {
        return TEMPORARY_HARBOUR.constructionList;
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
        return "back_" + weatherModifier + "harbour";
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.getDescription = function()
    {
        return qsTr("<r>Harbour made by APC. </r><div c='#00ff00'>Resupplying </div><r>of </r><div c='#00ff00'>naval </div><r>units is possible.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Hovercraft, GameEnums.UnitType_Naval];
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("temporary_harbour+snow", false);
		};
	};
    this.baseTerrain = ["LAKE", "SEA", "PLAINS", "SNOW", "WASTE", "DESERT"];
    this.getBaseTerrain = function(building)
    {
        return TEMPORARY_HARBOUR.baseTerrain;
    };
}

Constructor.prototype = BUILDING;
var TEMPORARY_HARBOUR = new Constructor();
