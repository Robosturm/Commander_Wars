var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon2ID("WEAPON_VULCAN_CANNON");
        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon1ID("FLARE_MG");
        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(6);
        unit.setCosts(8000);
        unit.setMinRange(1);
        unit.setMaxRange(5);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("flare", false);
        unit.loadSprite("flare+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
}

Constructor.prototype = UNIT;
var FLARE = new Constructor();
