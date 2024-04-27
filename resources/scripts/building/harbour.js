var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 0;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("harbour+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("harbour", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("harbour+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("harbour+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
    this.getName = function()
    {
        return qsTr("Harbour");
    };

    this.actionList = ["ACTION_BUILD_UNITS"];
    this.constructionList = ["GUNBOAT", "CANNONBOAT", "BLACK_BOAT", "LANDER", "TORPEDOBOAT", "FRIGATE", "DESTROYER", "CRUISER", "SUBMARINE", "BATTLECRUISER", "BATTLESHIP", "AIRCRAFTCARRIER"];
    this.getConstructionList = function(building)
    {
        return HARBOUR.constructionList;
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

    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for the </r><div c='#00ff00'>production, repairing and resupplying </div><r>of </r><div c='#00ff00'>naval </div><r>units.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Naval];
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("harbour+snow", false);
		};
	};
    this.baseTerrain = ["LAKE", "SEA", "PLAINS", "SNOW", "WASTE", "DESERT"];
    this.getBaseTerrain = function(building)
    {
        return HARBOUR.baseTerrain;
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var HARBOUR = new Constructor();
