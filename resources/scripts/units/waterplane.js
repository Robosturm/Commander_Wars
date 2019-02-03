var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon1ID("WEAPON_WATERPLANE_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(7);
        unit.setCosts(15000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(3);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("waterplane", false);
        unit.loadSprite("waterplane+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
}

Constructor.prototype = UNIT;
var WATERPLANE = new Constructor();
