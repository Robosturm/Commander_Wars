var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_HEAVY_HOVERCRAFT_GUN");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_HEAVY_HOVERCRAFT_MG");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(4);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("heavy_hovercraft+mask", GameEnums.Recoloring_Matrix);
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
    this.getMovementType = function()
    {
        return "MOVE_HOVERCRAFT";
    };
    this.getBaseCost = function()
    {
        return 12000;
    };
    this.getName = function()
    {
        return qsTr("Heavy Hovercraft");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Reinforced Tank. Attack and defence capabilities superior to standard hovercraft. Can move over most terrains.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Hovercraft;
    };
    this.getEditorPlacementSound = function()
    {
        return "movehovercraft.wav";
    };
}

Constructor.prototype = UNIT;
var HEAVY_HOVERCRAFT = new Constructor();
