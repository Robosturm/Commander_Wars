var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("cruiser", false);
        unit.loadSprite("cruiser+mask", true);
    };
    this.getMovementType = function()
    {
        return "SHIP";
    };
}

Constructor.prototype = UNIT;
var CRUISER = new Constructor();
