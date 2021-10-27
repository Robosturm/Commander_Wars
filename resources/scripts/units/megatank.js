var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_MEGATANK_GUN");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_MEGATANK_MG");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    this.getBaseCost = function()
    {
        return 16000;
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("megatank+mask", GameEnums.Recoloring_Matrix);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveheavytank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getName = function()
    {
        return qsTr("Megatank");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Most powerful Tank. Boast superlative attack and defence capabilities.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var MEGATANK = new Constructor();
