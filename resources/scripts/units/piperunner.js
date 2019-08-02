var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(9);
        unit.setMaxAmmo1(9);
        unit.setWeapon1ID("WEAPON_PIPERUNNER_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(99);
        unit.setMaxFuel(99);
        unit.setBaseMovementPoints(9);
        unit.setMinRange(2);
        unit.setMaxRange(5);
        unit.setVision(4);
    };
    this.getBaseCost = function()
    {
        return 20000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("piperunner", false);
        unit.loadSprite("piperunner+mask", true);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_PIPERUNNER";
    };
    this.getName = function()
    {
        return qsTr("Piperunner");
    };
    this.canMoveAndFire = function()
    {
        return false;
    };

    this.getDescription = function()
    {
        return qsTr("Indirect attack unit. Can attack all units. Can only move over pipelines. ");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var PIPERUNNER = new Constructor();
