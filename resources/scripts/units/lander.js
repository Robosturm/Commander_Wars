var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
		unit.setCosts(10000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(1);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("lander", false);
        unit.loadSprite("lander+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_BOAT";
    };
}

Constructor.prototype = UNIT;
var LANDER = new Constructor();
