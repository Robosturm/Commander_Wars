// called for loading the main sprite
INFANTRY.loadSprites = function(unit)
{
    // load sprites
    unit.loadSprite("infantry+os", false);
    unit.loadSpriteV2("infantry+os+mask", GameEnums.Recoloring_Table);
};
INFANTRY.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    animation.loadSpriteV2("infantry+os+walk+mask", GameEnums.Recoloring_Table, 1.33);
    animation.loadSprite("infantry+os+walk", false, 1.33);
    animation.setSound("movewalk.wav", -2);
    return animation;
};
