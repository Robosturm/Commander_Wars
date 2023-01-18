var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_ROCKET_MISSILE");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(3);
        unit.setMaxRange(5);
        unit.setVision(3);
    };
    this.getBaseCost = function()
    {
        return 15000;
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("rocketthrower+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_TIRE_A";
    };
    this.getName = function()
    {
        return qsTr("Rockets");
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveheavytire.wav", -2);
        return animation;
    };
    this.canMoveAndFire = function()
    {
        return false;
    };

    this.getDescription = function()
    {
        return qsTr("Indirect attack unit. Powerful with a wide attack range.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveheavytire.wav";
    };
}

Constructor.prototype = UNIT;
var ROCKETTHROWER = new Constructor();
