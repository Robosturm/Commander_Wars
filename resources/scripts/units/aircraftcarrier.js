var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_AIR_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(5);
		unit.setCosts(28000);
        unit.setMinRange(3);
        unit.setMaxRange(8);
		unit.setVision(4);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("aircraftcarrier", false);
        unit.loadSprite("aircraftcarrier+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
}

Constructor.prototype = UNIT;
var AIRCRAFTCARRIER = new Constructor();
