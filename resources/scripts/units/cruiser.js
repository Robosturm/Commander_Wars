var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 9;
        unit.maxAmmo1 = 9;
        unit.ammo2 = 9;
        unit.maxAmmo2 = 9;
        unit.fuel = 100;
        unit.maxFuel = 100;
        unit.baseMovementPoints = 6;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("cruiser", false);
        unit.loadSprite("cruiser+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
}

Constructor.prototype = UNIT;
var CRUISER = new Constructor();
