var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setWeapon1ID("WEAPON_STEALTHROCKET");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(7);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("stealthbomber", false);
        unit.loadSprite("stealthbomber+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_AIR";
    };
    this.getBaseCost = function()
    {
        return 22000;
    };
    this.getName = function()
    {
        return qsTr("Stealthbomber");
    };
    this.startOfTurn = function(unit)
    {
        // pay unit upkeep
        var fuelCosts = 5;
        if (unit.getHidden())
        {
            fuelCosts += 3;
        }
        fuelCosts = fuelCosts + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), fuelCosts);
        if (fuelCosts < 0)
        {
            fuelCosts = 0;
        }
        unit.setFuel(unit.getFuel() - fuelCosts);
    };
    this.createExplosionAnimation = function(x, y)
    {
        var animation = GameAnimationFactory.createAnimation(x, y);
        animation.addSprite("explosion+air", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
        audio.playSound("explosion+air.wav");
        return animation;
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        var unitID = unit.getUnitID().toLowerCase();
        animation.loadSprite(unitID + "+walk+mask", true, 1.5);
        animation.loadSprite(unitID + "+walk", false, 1.5);
        animation.setSound("moveair.wav", -2);
        return animation;
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_FIRE,ACTION_STEALTH,ACTION_UNSTEALTH,ACTION_LOAD,ACTION_JOIN,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.canMoveAndFire = function()
    {
        return true;
    };
    this.useTerrainDefense = function()
    {
        return false;
    };
}

Constructor.prototype = UNIT;
var STEALTHBOMBER = new Constructor();
