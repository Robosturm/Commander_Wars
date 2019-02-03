var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_AIRPLANE_MG");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(8);
        unit.setCosts(13000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(4);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("duster", false);
        unit.loadSprite("duster+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
}

Constructor.prototype = UNIT;
var DUSTER = new Constructor();
