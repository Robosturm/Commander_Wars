var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("temporary_airport+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("temporary_airport", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("temporary_airport+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
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
        return qsTr("<r>An airport made by an APC. </r><div c='#00ff00'>Repairing and resupplying </div><r>of </r><div c='#00ff00'>air </div><r>units is possible here. It does not produce any funds or units.</r>");
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
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var TEMPORARY_AIRPORT = new Constructor();
