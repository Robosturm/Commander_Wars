var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 6;
        unit.maxAmmo1 = 6;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 60;
        unit.maxFuel = 60;
        unit.baseMovementPoints = 6;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("flak", false);
        unit.loadSprite("flak+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
}

Constructor.prototype = UNIT;
var FLAK = new Constructor();
