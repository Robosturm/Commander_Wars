var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 0;
    };
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("factory", false, 200);
            building.loadSpriteV2("factory+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSprite("factory+neutral", false);
        }
        building.loadSprite("factory+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
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
}

Constructor.prototype = BUILDING;
var FACTORY = new Constructor();
