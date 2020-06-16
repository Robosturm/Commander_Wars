ACTION_SUPPORTSINGLE_REPAIR.performPostAnimation = function(postAnimation)
{
    var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetY);
    var repairUnit = terrain.getUnit();
    var animation = GameAnimationFactory.createAnimation(ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetY);
    var width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, 2, 1);
    animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
    animation.addSprite("repair", map.getImageSize() / 2 + 8, 1, 400, 1.7);

    repairUnit.refill(true);
    UNIT.repairUnit(repairUnit, 1);
    ACTION_SUPPORTSINGLE_REPAIR.postAnimationUnit.setHasMoved(true);
    ACTION_SUPPORTSINGLE_REPAIR.postAnimationUnit = null;
    ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetX = -1;
    ACTION_SUPPORTSINGLE_REPAIR.postAnimationTargetY = -1;
};
