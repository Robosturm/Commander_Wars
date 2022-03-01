var Constructor = function()
{
    
    this.loadSprites = function(building, neutral, map)
    {
        // neutral player
        building.loadSprite("silo", false);
        building.loadSprite("silo+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
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
        return "minimap_silo";
    };

    this.getName = function()
    {
        return qsTr("Silo");
    };

    this.getDescription = function()
    {
        return qsTr("Empty missile silo which provides some defence.");
    };

    this.getTerrainAnimationBackground = function(unit, terrain, dfender, map)
    {
        return "back_missile+1";
    };
}

Constructor.prototype = BUILDING;
var SILO = new Constructor();
