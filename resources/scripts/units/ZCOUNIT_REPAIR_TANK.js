var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "APC";
    };
	
    this.init = function(unit)
    {
        unit.setAmmo1(0);
        unit.setMaxAmmo1(0);
        unit.setWeapon1ID("");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(1);
    };
	this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_SUPPORTSINGLE_FREEREPAIR,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("repair_tank", false);
        unit.loadSpriteV2("repair_tank+mask", GameEnums.Recoloring_Table);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getBaseCost = function()
    {
        return 7000;
    };
    this.getName = function()
    {
        return qsTr("Repair Tank");
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSpriteV2("repair_tank+walk+mask", GameEnums.Recoloring_Table, 1.5);
        animation.loadSprite("repair_tank+walk", false, 1.5);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getDescription = function()
    {
        return "Unarmed unit that can repair units for 1Hp for free each turn.";
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_REPAIR_TANK = new Constructor();
