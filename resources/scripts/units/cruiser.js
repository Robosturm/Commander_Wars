var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_ANTI_SHIP_CRUISER_MISSILE");

        unit.setAmmo2(9);
        unit.setMaxAmmo2(9);
        unit.setWeapon2ID("WEAPON_A_AIR_GUN");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(6);
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
    this.getBaseCost = function()
    {
        return 16000;
    };
    this.getName = function()
    {
        return qsTr("Cruiser");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        unit.setFuel(unit.getFuel() - 1);
        UNIT.transporterRefilling(unit);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+water", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+water.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.25);
        animation.loadSprite(unitID + "+walk", false, 1.25);
        animation.setSound("moveship.wav", -2);
        return animation;
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getLoadingPlace = function()
    {
        return 2;
    };
    this.getTransportUnits = function()
    {
        return ["T_HELI", "K_HELI"];
    };

}

Constructor.prototype = UNIT;
var CRUISER = new Constructor();
