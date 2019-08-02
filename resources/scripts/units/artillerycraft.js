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
        unit.setBaseMovementPoints(5);
        unit.setMinRange(2);
        unit.setMaxRange(3);
        unit.setVision(1);

    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("artillerycraft", false);
        unit.loadSprite("artillerycraft+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_HOVERCRAFT";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("movetank.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 8000;
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
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var ARTILLERYCRAFT = new Constructor();
