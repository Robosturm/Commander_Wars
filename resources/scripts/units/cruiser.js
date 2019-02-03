var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_MISSILE");
        unit.setAmmo2(9);
        unit.setMaxAmmo2(9);
        unit.setWeapon2ID("WEAPON_A_AIR_GUN");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
        unit.setCosts(16000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(5);
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
