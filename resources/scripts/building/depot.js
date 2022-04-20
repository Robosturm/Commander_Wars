var Constructor = function()
{
    
    this.loadSprites = function(building, neutral, map)
    {
        // neutral player
        building.loadSprite("depot", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 3;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_depot";
    };

    this.getName = function()
    {
        return qsTr("Depot");
    };

    this.getDescription = function()
    {
        return qsTr("Depot which provides some defence.");
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
        return "back_" + weatherModifier + "depot";
    };
}

Constructor.prototype = BUILDING;
var DEPOT = new Constructor();
