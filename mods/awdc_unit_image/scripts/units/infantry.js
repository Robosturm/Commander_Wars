// called for loading the main sprite
INFANTRY.loadSprites = function(unit)
{
    // load sprites
    unit.loadSprite("infantry+os", false);
    unit.loadSprite("infantry+os+mask", true);
};
INFANTRY.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    animation.loadSprite("infantry+os+walk+mask", true, 1);
    animation.loadSprite("infantry+os+walk", false, 1);
    animation.setSound("movefeet.wav", -2);
    return animation;
};
