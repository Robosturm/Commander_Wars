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
    this.actionList = ["ACTION_SUPPORTSINGLE_IRON_SHIELD", "ACTION_JOIN", "ACTION_LOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("iron_shield_generator+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_CRAWLER";
    };
    this.getBaseCost = function()
    {
        return 7000;
    };
    this.getName = function()
    {
        return qsTr("Iron Shield Gen.");
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        animation.loadSpriteV2("iron_shield_generator+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movewalker.wav", -2);
        return animation;
    };
    this.getDescription = function()
    {
        return qsTr("Unarmed unit that can give an allied unit a huge defense boost.");
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
var ZCOUNIT_IRON_SHIELD_GENERATOR = new Constructor();
