var Constructor = function()
{
    // called for loading the main sprite
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
        return qsTr("Missile can be launched once, by Infantry unit. Damage across <div c='00ff00'>13 square </div>area.");
    };
}

Constructor.prototype = BUILDING;
var SILO_ROCKET = new Constructor();
