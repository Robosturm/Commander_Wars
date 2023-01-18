var Constructor = function()
{
    this.getUnitDamageID = function(unit)
    {
        return "HEAVY_TANK";
    };

    this.init = function(unit)
    {
        unit.setAmmo1(5);
        unit.setMaxAmmo1(5);
        unit.setWeapon1ID("WEAPON_TANKHUNTER_GUN");

        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setWeapon2ID("WEAPON_HEAVY_TANK_MG");

        unit.setFuel(50);
        unit.setMaxFuel(50);
        unit.setBaseMovementPoints(5);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    
    this.loadSprites = function(unit)
    {
        unit.loadSpriteV2("tank_hunter+mask", GameEnums.Recoloring_Matrix);
    };
    this.doWalkingAnimation = function(action, map)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(map, unit, action);
        animation.loadSpriteV2("tank_hunter+walk+mask", GameEnums.Recoloring_Matrix, 2);
        animation.setSound("moveheavytank.wav", -2);
        return animation;
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.getBaseCost = function()
    {
        return 14000;
    };
    this.getName = function()
    {
        return qsTr("Tank Hunter");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Reinforced Tank his armor piercing cannon destroys fuel in addition to dealing damage.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.postBattleActions = function(unit, damage, otherUnit, gotAttacked, weapon, action)
    {
        if (weapon === 0 && gotAttacked === false && unit.getUnitID() === "ZCOUNIT_TANK_HUNTER" && damage >= 0)
        {
            var fuel = otherUnit.getFuel();
            otherUnit.setFuel(fuel - 35);
        }
    };
    this.getCOSpecificUnit = function(building)
    {
        return true;
    };
    this.getEditorPlacementSound = function()
    {
        return "moveheavytank.wav";
    };
}

Constructor.prototype = UNIT;
var ZCOUNIT_TANK_HUNTER = new Constructor();
