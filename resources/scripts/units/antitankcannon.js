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
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveheavytire.wav", -2);
        return animation;
    };

    this.getBaseCost = function()
    {
        return 11000;
    };

    this.getName = function()
    {
        return qsTr("Anti Tank Cannon");
    };

    this.getDescription = function()
    {
        return qsTr("Indirect attack unit. When under direct enemy attack it can <div c='00ff00'>counter-attack<div c='ffffff'>.");
    };
}

Constructor.prototype = UNIT;
var ANTITANKCANNON = new Constructor();
