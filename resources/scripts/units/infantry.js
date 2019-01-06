var Constructor = function()
{
    this.init = function(unit)
    {
        unit.ammo1 = 10;
        unit.maxAmmo1 = 10;
        unit.ammo2 = 0;
        unit.maxAmmo2 = 0;
        unit.fuel = 100;
        unit.maxFuel = 100;
        unit.baseMovementPoints = 3;
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // none neutral player
        var player = unit.getOwner();
        // get army name
        var armyName = player.getArmy();
        // bh and bg have the same sprites
        if (armyName === "BG")
        {
            armyName = "BH"
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
        armyName = "os";
        animation.loadSprite("infantry+" + armyName + "+walk+mask", true, 1);
        animation.loadSprite("infantry+" + armyName + "+walk", false, 1);
        return animation;
    };
}

Constructor.prototype = UNIT;
var INFANTRY = new Constructor();
