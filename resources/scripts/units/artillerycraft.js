var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_CANNON");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(2);
        unit.setMaxRange(3);
        unit.setVision(1);

    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("artillerycraft+mask", GameEnums.Recoloring_Matrix);
    };
    this.getMovementType = function()
    {
        return "MOVE_HOVERCRAFT";
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("movehovercraft.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 6000;
    };
    this.getName = function()
    {
        return qsTr("Artillerycraft");
    };

    this.getDescription = function()
    {
        return qsTr("Indirect attack unit. Attack cannot be launched after unit has moved. Can move over most terrains.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Hovercraft;
    };
    this.getTypeOfWeapon1 = function(unit)
    {
        return GameEnums.WeaponType_Indirect;
    };
}

Constructor.prototype = UNIT;
var ARTILLERYCRAFT = new Constructor();
