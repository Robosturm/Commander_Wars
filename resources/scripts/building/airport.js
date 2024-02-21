var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 0;
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("airport+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("airport", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("airport+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };

    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("airport+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
    this.getName = function()
    {
        return qsTr("Airport");
    };
    this.actionList = ["ACTION_BUILD_UNITS"];

    this.constructionList = ["T_HELI", "K_HELI", "DUSTER", "TRANSPORTPLANE", "BOMBER", "FIGHTER", "STEALTHBOMBER", "BLACK_BOMB"];
    this.getConstructionList = function(building)
    {
        return AIRPORT.constructionList;
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

    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for </r><div c='#00ff00'>production and resupplying</div><r> of </r><div c='#00ff00'>air</div><r> units.</r>");
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
			building.loadWeatherOverlaySpriteV2("airport+snow", false);
		};
	};
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var AIRPORT = new Constructor();
