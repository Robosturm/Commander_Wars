var Constructor = function()
{
    
    this.loadSprites = function(terrain, map)
    {
        building.loadSprite("depot", false);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 2;
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
        return qsTr("A civilian cargo depot that provides adequate cover.");
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
