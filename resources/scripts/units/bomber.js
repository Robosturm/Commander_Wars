var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
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
