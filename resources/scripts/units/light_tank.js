var Constructor = function()
{
    this.init = function(unit)
    {
        unit.setAmmo1(6);
        unit.setMaxAmmo1(6);
        unit.setAmmo2(10);
        unit.setMaxAmmo2(10);
        unit.setFuel(70);
        unit.setMaxFuel(70);
        unit.setBaseMovementPoints(6);
    };
    // called for loading the main sprite
    this.loadSprites = function(unit)
    {
        // load sprites
        unit.loadSprite("light_tank", false);
        unit.loadSprite("light_tank+mask", true);
    };
    this.getMovementType = function()
    {
        return "MOVE_TANK";
    };
    this.doWalkingAnimation = function(action)
    {
        var unit = action.getTargetUnit();
        var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
        animation.loadSprite("light_tank+walk+mask", true, 1);
        animation.loadSprite("light_tank+walk", false, 1);
        return animation;
    };
}

Constructor.prototype = UNIT;
var LIGHT_TANK = new Constructor();
