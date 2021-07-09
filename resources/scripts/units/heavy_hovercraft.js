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
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("heavy_hovercraft", false);
        unit.loadSpriteV2("heavy_hovercraft+mask", GameEnums.Recoloring_Table);
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 2);
        animation.loadSprite(unitID + "+walk", false, 2);
        animation.setSound("movehovercraft.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_HOVERCRAFT";
    };
    this.getBaseCost = function()
    {
        return 13500;
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
}

Constructor.prototype = UNIT;
var HEAVY_HOVERCRAFT = new Constructor();
