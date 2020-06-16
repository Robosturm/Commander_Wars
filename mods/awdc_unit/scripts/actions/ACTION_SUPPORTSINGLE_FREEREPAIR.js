ACTION_SUPPORTSINGLE_FREEREPAIR.performPostAnimation = function(postAnimation)
{
    var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
    var repairUnit = terrain.getUnit();
    var animation = GameAnimationFactory.createAnimation(ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
    var width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, 2, 1);
    animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
    animation.addSprite("repair", map.getImageSize() / 2 + 8, 1, 400, 1.7);

    repairUnit.refill(true);
    repairUnit.setHp(repairUnit.getHp() + 1);
    ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit.setHasMoved(true);
    ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit = null;
    ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX = -1;
    ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY = -1;
};
