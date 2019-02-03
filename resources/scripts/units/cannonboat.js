var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(1);
        unit.setMaxAmmo1(1);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
		unit.setCosts(6000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("cannonboat", false);
        unit.loadSprite("cannonboat+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_BOAT";
    };
}

Constructor.prototype = UNIT;
var CANNONBOAT = new Constructor();
