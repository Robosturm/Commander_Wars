var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("artillery", false);
        unit.loadSprite("artillery+mask", true);
    };
    this.getMovementType = function()
    {
        return "TANK";
    };
}

Constructor.prototype = UNIT;
var ARTILLERY = new Constructor();
