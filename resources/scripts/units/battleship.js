var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_BATTLESHIP_CANNON");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(5);
		unit.setCosts(25000);
        unit.setMinRange(3);
        unit.setMaxRange(5);
		unit.setVision(3);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("battleship", false);
        unit.loadSprite("battleship+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };
}

Constructor.prototype = UNIT;
var BATTLESHIP = new Constructor();
