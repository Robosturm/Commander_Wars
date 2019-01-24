var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("artillery", false);
        unit.loadSprite("artillery+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
}

Constructor.prototype = UNIT;
var ARTILLERY = new Constructor();
