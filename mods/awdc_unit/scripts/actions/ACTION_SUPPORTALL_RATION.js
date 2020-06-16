ACTION_SUPPORTALL_RATION.giveRation = function(unit)
{
    var x = unit.getX() + 1;
    var y = unit.getY();
    var animation = null;
    var refillUnit= null;
    var width = 0;
    if (ACTION_SUPPORTALL_RATION.checkUnit(unit, x, y))
    {
        refillUnit = map.getTerrain(x, y).getUnit();
        refillUnit.refill(true);
        if (!refillUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            animation = GameAnimationFactory.createAnimation(x, y);
            width = animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 25, 2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
            animation.addSprite("ration", map.getImageSize() / 2 + 8, 1, 400, 1.7);
        }
    }
    x = unit.getX() - 1;
    if (ACTION_SUPPORTALL_RATION.checkUnit(unit, x, y))
    {
        refillUnit = map.getTerrain(x, y).getUnit();
        refillUnit.refill(true);
        if (!refillUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            animation = GameAnimationFactory.createAnimation(x, y);
            width = animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 25, 2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
            animation.addSprite("ration", map.getImageSize() / 2 + 8, 1, 400, 1.7);
        }
    }
    x = unit.getX();
    y = unit.getY() + 1;
    if (ACTION_SUPPORTALL_RATION.checkUnit(unit, x, y))
    {
        refillUnit = map.getTerrain(x, y).getUnit();
        refillUnit.refill(true);
        if (!refillUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            animation = GameAnimationFactory.createAnimation(x, y);
            width = animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 25, 2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
            animation.addSprite("ration", map.getImageSize() / 2 + 8, 1, 400, 1.7);
        }
    }
    y = unit.getY() - 1;
    if (ACTION_SUPPORTALL_RATION.checkUnit(unit, x, y))
    {
        refillUnit = map.getTerrain(x, y).getUnit();
        refillUnit.refill(true);
        if (!refillUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            animation = GameAnimationFactory.createAnimation(x, y);
            width = animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 25, 2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
            animation.addSprite("ration", map.getImageSize() / 2 + 8, 1, 400, 1.7);
        }
    }
};
