var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_AIR_TO_GROUND_MISSILE");
        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_K_HELI_MG");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setCosts(9000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("k_heli", false);
        unit.loadSprite("k_heli+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
}

Constructor.prototype = UNIT;
var K_HELI = new Constructor();
