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
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(1);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("intel_truck+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.actionList = ["ACTION_SUPPORTALL_CLOAK", "ACTION_JOIN", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
    this.getBaseCost = function()
    {
        return 5000;
    };
    this.getName = function()
    {
        return qsTr("Intel Truck");
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2("intel_truck+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getDescription = function()
    {
        return qsTr("The intel truck is a defenseless unit that can create a cloak field that cloaks all owned units around it. Cloaked units are stealthed but can still be attack by other units.");
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
var ZCOUNIT_INTEL_TRUCK = new Constructor();
