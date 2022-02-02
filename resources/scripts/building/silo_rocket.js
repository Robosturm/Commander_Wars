var Constructor = function()
{
    
    this.loadSprites = function(building)
    {
        // neutral player
        building.loadSprite("silo+rocket", false);
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
        return qsTr("<r>Missile can be launched once, by Infantry unit. Damage across </r><div c='#00ff00'>13 square </div><r>area.</r>");
    };

    this.getTerrainAnimationBackground = function(unit, terrain, dfender, map)
    {
        return "back_missile+0";
    };
}

Constructor.prototype = BUILDING;
var SILO_ROCKET = new Constructor();
