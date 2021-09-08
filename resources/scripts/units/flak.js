var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_VULCAN_CANNON");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(60);
        unit.setMaxFuel(60);
        unit.setBaseMovementPoints(6);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("flak+mask", GameEnums.Recoloring_Matrix);
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
        return 8000;
    };
    this.getName = function()
    {
        return qsTr("Anti Air");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Specialised anti-air and anti-infantry unit. Can also attack ground forces.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var FLAK = new Constructor();
