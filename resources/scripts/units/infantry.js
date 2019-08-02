var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setWeapon1ID("WEAPON_INFANTRY_MG");

        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setWeapon2ID("");

        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
        unit.setMinRange(1);
        unit.setMaxRange(1);
        unit.setVision(2);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        // bh and bg have the same sprites
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        // load sprites
        unit.loadSprite("infantry+" + armyName, false);
        unit.loadSprite("infantry+" + armyName +"+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_FEET";
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this unit can perform
        return "ACTION_MISSILE,ACTION_CAPTURE,ACTION_FIRE,ACTION_JOIN,ACTION_LOAD,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy().toLowerCase();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        animation.loadSprite("infantry+" + armyName + "+walk+mask", true, 1.5);
        animation.loadSprite("infantry+" + armyName + "+walk", false, 1.5);
        animation.setSound("movefeet.wav", -2);
        return animation;
    };
    this.getBaseCost = function()
    {
        return 1000;
    };
    this.getName = function()
    {
        return qsTr("Infantry");
    };
    this.canMoveAndFire = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("Cheapest unit. Can <div c='00ff00'>capture <div c='ffffff'> bases. <div c='00ff00'>Vision +3 <div c='ffffff'> when on mountains.");
    };
    this.getUnitType = function()
    {
        return GameEnums.UnitType_Ground;
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
