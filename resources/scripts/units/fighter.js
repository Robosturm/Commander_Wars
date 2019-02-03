var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_AIR_TO_AIR_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(9);
        unit.setCosts(20000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("fighter", false);
        unit.loadSprite("fighter+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
}

Constructor.prototype = UNIT;
var FIGHTER = new Constructor();
