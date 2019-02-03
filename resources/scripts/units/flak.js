var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon2ID("WEAPON_VULCAN_CANNON");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon1ID("");
        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(6);
        unit.setCosts(8000);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("flak", false);
        unit.loadSprite("flak+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
}

Constructor.prototype = UNIT;
var FLAK = new Constructor();
