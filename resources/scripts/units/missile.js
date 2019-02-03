var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon2ID("WEAPON_ANTI_AIR_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(4);
        unit.setCosts(12000);
        unit.setMinRange(2);
        unit.setMaxRange(6);
        unit.setVision(5);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("missile", false);
        unit.loadSprite("missile+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_A";
    };
}

Constructor.prototype = UNIT;
var MISSILE = new Constructor();
