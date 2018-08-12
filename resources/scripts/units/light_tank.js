var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("light_tank", false);
        unit.loadSprite("light_tank+mask", true);
    };
    this.getMovementType = function()
    {
        return "TANK";
    };
}

Constructor.prototype = UNIT;
var LIGHT_TANK = new Constructor();
