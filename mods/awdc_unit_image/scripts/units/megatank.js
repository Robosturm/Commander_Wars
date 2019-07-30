MEGATANK.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    var unitID = unit.getUnitID().toLowerCase();
    animation.loadSprite(unitID + "+walk+mask", true, 1);
    animation.loadSprite(unitID + "+walk", false, 1);
    animation.setSound("moveheavytank.wav", -2);
    return animation;
};
