var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 1;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("town+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("town", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("town+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.constructionList = ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                            "ARTILLERY", "LIGHT_TANK", "FLAK",
                            "ANTITANKCANNON", "HEAVY_TANK", "NEOTANK",
                            "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return TOWN.constructionList;
    };

    this.getName = function()
    {
        return qsTr("Town");
    };

    this.getDescription = function()
    {
        return qsTr("<r>" + "Populated city. Once captured </r><div c='#00ff00'>ground </div><r>units can </r><div c='#00ff00'>resupply</div><r>.</r>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Ground, GameEnums.UnitType_Infantry];
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("town+snow", false);
		};
	};
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var TOWN = new Constructor();
