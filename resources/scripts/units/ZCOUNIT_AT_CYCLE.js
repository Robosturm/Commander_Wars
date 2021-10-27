var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "MOTORBIKE";
    };

    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_BAZOOKA");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    this.getBaseCost = function()
    {
        return 3000;
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("at_cycle+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_B";
    };
    this.actionList = ["ACTION_FIRE", "ACTION_MISSILE", "ACTION_CAPTURE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.getName = function()
    {
        return qsTr("AT Cycle");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2("at_cycle+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movebike.wav", -2);
        return animation;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Infantry unit. High mobility. Able to </r><div c='#00ff00'>capture </div><r> bases.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Infantry;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_AT_CYCLE = new Constructor();
