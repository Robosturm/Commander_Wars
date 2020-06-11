// called for loading the main sprite
MECH.loadSprites = function(unit)
{
    // load sprites
    unit.loadSprite("mech+os", false);
    unit.loadSpriteV2("mech+os+mask", GameEnums.Recoloring_Table);
};
MECH.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    animation.loadSpriteV2("mech+os+walk+mask", GameEnums.Recoloring_Table, 1);
    animation.loadSprite("mech+os+walk", false, 1);
    animation.setSound("movefeet.wav", -2);
    return animation;
};
