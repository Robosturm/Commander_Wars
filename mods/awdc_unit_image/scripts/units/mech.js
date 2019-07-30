// called for loading the main sprite
MECH.loadSprites = function(unit)
{
    // load sprites
    unit.loadSprite("mech+os", false);
    unit.loadSprite("mech+os+mask", true);
};
MECH.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    animation.loadSprite("mech+os+walk+mask", true, 1);
    animation.loadSprite("mech+os+walk", false, 1);
    animation.setSound("movefeet.wav", -2);
    return animation;
};
