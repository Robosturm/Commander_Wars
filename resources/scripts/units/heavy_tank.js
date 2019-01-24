var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
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
