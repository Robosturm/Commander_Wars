var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("flak", false);
        unit.loadSprite("flak+mask", true);
    };
    this.getMovementType = function()
    {
        return "TANK";
    };
}

Constructor.prototype = UNIT;
var FLAK = new Constructor();
