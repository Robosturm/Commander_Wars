var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(10);
        unit.setMaxAmmo1(10);
        unit.setAmmo2(0);
        unit.setMaxAmmo2(0);
        unit.setFuel(100);
        unit.setMaxFuel(100);
        unit.setBaseMovementPoints(3);
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
        return "ACTION_CAPTURE,ACTION_WAIT";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy();
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "os") &&
            (armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh") &&
            (armyName !== "ma"))
        {
            armyName = "os";
        }
        animation.loadSprite("infantry+" + armyName + "+walk+mask", true, 1);
        animation.loadSprite("infantry+" + armyName + "+walk", false, 1);
        return animation;
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
