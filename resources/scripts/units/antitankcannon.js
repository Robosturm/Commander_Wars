var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_A_TANK_CANNON");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(4);
        unit.setCosts(11000);
        unit.setMinRange(1);
        unit.setMaxRange(3);
        unit.setVision(2);

    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("antitankcannon", false);
        unit.loadSprite("antitankcannon+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_B";
    };

}

Constructor.prototype = UNIT;
var ANTITANKCANNON = new Constructor();
