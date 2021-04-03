ACTION_EXPLODE.getStepCursor = function(action, cursorData)
{
    cursorData.setCursor("cursor+radius_3");
    cursorData.setXOffset(- map.getImageSize() * 3);
    cursorData.setYOffset(- map.getImageSize() * 3);
    cursorData.setScale(1.5);
};

ACTION_EXPLODE.performPostAnimation = function(postAnimation)
{
    var owner = ACTION_EXPLODE.postAnimationUnit.getOwner();
    var x = ACTION_EXPLODE.postAnimationTargetX;
    var y = ACTION_EXPLODE.postAnimationTargetY;
    var fields = globals.getCircle(1, 3);
    for (var i = 0; i < fields.size(); i++)
    {
        var point = fields.at(i);
        if (map.onMap(x + point.x, y + point.y))
        {
            var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
            if (unit !== null)
            {
                unit.setHp(unit.getHpRounded() - 5);
                if (unit.getHp() <= 0)
                {
                    // we destroyed a unit
                    map.getGameRecorder().destroyedUnit(unit.getOwner().getPlayerID(), unit.getUnitID());
                    unit.killUnit();
                }
            }
            var animation = GameAnimationFactory.createAnimation(x + point.x, y + point.y);
            animation.addSprite("explosion+land", -map.getImageSize() / 2, -map.getImageSize(), 0, 1.5);
            audio.playSound("explosion+land.wav");
        }
    }
    fields.remove();
    // we destroyed a unit
    map.getGameRecorder().destroyedUnit(owner.getPlayerID(), ACTION_EXPLODE.postAnimationUnit.getUnitID());
    ACTION_EXPLODE.postAnimationUnit.killUnit();
    ACTION_EXPLODE.postAnimationUnit = null;
    ACTION_EXPLODE.postAnimationTargetX = -1;
    ACTION_EXPLODE.postAnimationTargetY = -1;
};
