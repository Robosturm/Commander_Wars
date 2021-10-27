var Constructor = function()
{
    
    this.loadSprites = function(building)
    {
        // neutral player
        building.loadSprite("silo", false);
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

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_missile+1";
    };
}

Constructor.prototype = BUILDING;
var SILO = new Constructor();
