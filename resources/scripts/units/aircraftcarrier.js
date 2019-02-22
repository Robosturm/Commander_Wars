var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_AIR_MISSILE");
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(3);
        unit.setMaxRange(8);
		unit.setVision(4);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        unit.loadSprite("aircraftcarrier", false);
        unit.loadSprite("aircraftcarrier+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_SHIP";
    };

    this.getBaseCost = function()
    {
        return 28000;
    };

    this.getName = function()
    {
        return qsTr("Aircraft Carrier");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        unit.setFuel(unit.getFuel() - 1);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", 0, -map.getImageSize(), 0, 1.5);
        return animation;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.getTransportUnits = function()
    {
        return ["T_HELI", "K_HELI", "BOMBER", "DUSTER", "FIGHTER", "WATERPLANE", "STEALTHBOMBER"];
    };
}

Constructor.prototype = UNIT;
var AIRCRAFTCARRIER = new Constructor();
