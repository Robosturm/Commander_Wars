var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 0;
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("factory+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("factory", GameEnums.Recoloring_Matrix, 200, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("factory+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getName = function()
    {
        return qsTr("Base");
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("factory+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };

    this.actionList = ["ACTION_BUILD_UNITS"];

    this.constructionList = ["INFANTRY", "MOTORBIKE", "SNIPER", "MECH", "RECON", "APC", "FLARE",
                             "ARTILLERY", "LIGHT_TANK", "FLAK", "ANTITANKCANNON", "HEAVY_TANK",
                             "NEOTANK", "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return FACTORY.constructionList;
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
        return "back_" + weatherModifier + "factory";
    };

    this.getDescription = function()
    {
        return qsTr("<r>Once captured can be used for </r><div c='#00ff00'>production and resupplying </div><r>of </r><div c='#00ff00'>ground </div><r>units.</r>");
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
			building.loadWeatherOverlaySpriteV2("factory+snow", false);
		};
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var FACTORY = new Constructor();
