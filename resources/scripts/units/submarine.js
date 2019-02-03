var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_TORPEDO");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(6);
		unit.setCosts(20000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
		unit.setVision(5);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("submarine", false);
        unit.loadSprite("submarine+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
}

Constructor.prototype = UNIT;
var SUBMARINE = new Constructor();
