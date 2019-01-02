var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 5;
        unit.maxAmmo1 = 5;
        unit.ammo2 = 10;
        unit.maxAmmo2 = 10;
        unit.fuel = 50;
        unit.maxFuel = 50;
        unit.baseMovementPoints = 5;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("heavy_tank", false);
        unit.loadSprite("heavy_tank+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
}

Constructor.prototype = UNIT;
var HEAVY_TANK = new Constructor();
