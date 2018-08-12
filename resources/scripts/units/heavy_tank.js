var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("heavy_tank", false);
        unit.loadSprite("heavy_tank+mask", true);
    };
    this.getMovementType = function()
    {
        return "TANK";
    };
}

Constructor.prototype = UNIT;
var HEAVY_TANK = new Constructor();
