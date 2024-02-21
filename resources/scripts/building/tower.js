var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("tower+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("tower", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("tower+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.offensiveBonus = 10;
    this.defensiveBonus = 0;
    this.getOffensiveBonus = function(building)
    {
        return TOWER.offensiveBonus;
    };

    this.getDefensiveBonus = function(building)
    {
        return TOWER.defensiveBonus;
    };

    this.getName = function()
    {
        return qsTr("Tower");
    };

    this.getDescription = function()
    {
        var text = qsTr("<r>Once captured the Tower boosts your </r><div c='#00ff00'>attack by %0% and defence by %1%</div><r>.</r>");
        text = replaceTextArgs(text, [TOWER.offensiveBonus, TOWER.defensiveBonus]);
        return text;
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return "back_tower";
    };
	this.onWeatherChanged = function(building, weather, map)
	{	
		var weatherId = weather.getWeatherId();
		if (weatherId === "WEATHER_SNOW")
		{
			building.loadWeatherOverlaySpriteV2("tower+snow", false);
		};
	};
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        animation.addBuildingSprite("tower+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Matrix);
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var TOWER = new Constructor();
