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
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("piperunner+mask", GameEnums.Recoloring_Matrix);
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 1);
        animation.setSound("movepipe.wav", -2);
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
        return qsTr("Indirect-attack unit. Can attack all units. Can only move over pipelines and factories. ");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };

    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_pipeline";
    };
    this.getEditorPlacementSound = function()
    {
        return "movepipe.wav";
    };
}

Constructor.prototype = UNIT;
var PIPERUNNER = new Constructor();
