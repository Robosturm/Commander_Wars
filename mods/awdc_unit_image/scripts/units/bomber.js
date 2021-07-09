BOMBER.doWalkingAnimation = function(action)
{
    var unit = action.getTargetUnit();
    var animation = GameAnimationFactory.createWalkingAnimation(unit, action);
    var unitID = unit.getUnitID().toLowerCase();
    animation.loadSpriteV2(unitID + "+walk+mask", GameEnums.Recoloring_Table, 1.33);
    animation.loadSprite(unitID + "+walk", false, 1.33);
    animation.setSound("moveair.wav", -2);
    return animation;
};
