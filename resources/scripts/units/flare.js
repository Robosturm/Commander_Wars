var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(3);
        unit.setMaxAmmo1(3);
        unit.setWeapon1ID("WEAPON_FLARE");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_FLARE_MG");

        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("flare+mask", GameEnums.Recoloring_Matrix);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getBaseCost = function()
    {
        return 5000;
    };
    this.getName = function()
    {
        return qsTr("Flare");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.actionList = ["ACTION_FLARE", "ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War, can fire </r><div c='#00ff00'>flare rocket</div><r>. Illuminates </r><div c='#00ff00'>2 square area</div><r> where it lands.</r>");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var FLARE = new Constructor();
