var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 6;
        unit.maxAmmo1 = 6;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 100;
        unit.maxFuel = 100;
        unit.baseMovementPoints = 7;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("bomber", false);
        unit.loadSprite("bomber+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
}

Constructor.prototype = UNIT;
var BOMBER = new Constructor();
