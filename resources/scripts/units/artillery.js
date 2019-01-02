var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 6;
        unit.maxAmmo1 = 6;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 50;
        unit.maxFuel = 50;
        unit.baseMovementPoints = 5;
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
