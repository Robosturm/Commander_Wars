var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_ANTI_AIR_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(2);
        unit.setMaxRange(6);
        unit.setVision(5);
    };
    this.getBaseCost = function()
    {
        return 12000;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("missile", false);
        unit.loadSpriteV2("missile+mask", GameEnums.Recoloring_Table);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 2);
        animation.loadSprite(unitID + "+walk", false, 2);
        animation.setSound("moveheavytire.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_A";
    };
    this.getName = function()
    {
        return qsTr("Missile");
    };
    this.getDescription = function()
    {
        return qsTr("Indirect attack unit. Only capable of striking air targets.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var MISSILE = new Constructor();
