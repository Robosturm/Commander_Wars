var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setAmmo2(9);
        unit.setMaxAmmo2(9);
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
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
