var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null))
        {
            var x = actionTargetField.x + 1;
            var y = actionTargetField.y;
            if (ACTION_RATION.checkUnit(unit, x, y))
            {
                return true;
            }
            x = actionTargetField.x - 1;
            if (ACTION_RATION.checkUnit(unit, x, y))
            {
                return true;
            }
            x = actionTargetField.x;
            y = actionTargetField.y + 1;
            if (ACTION_RATION.checkUnit(unit, x, y))
            {
                return true;
            }
            y = actionTargetField.y - 1;
            if (ACTION_RATION.checkUnit(unit, x, y))
            {
                return true;
            }
        }
        return false;
    };
    this.checkUnit = function(unit, x, y)
    {
        if (map.onMap(x, y))
        {
            if (map.getTerrain(x, y).getUnit() !== null)
            {
                if (map.getTerrain(x, y).getUnit().getOwner() === unit.getOwner())
                {
                    return true;
                }
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Ration");
    };
    this.getIcon = function()
    {
        return "ration";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
	this.postAnimationUnit = null;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        ACTION_RATION.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_RATION.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_RATION", "performPostAnimation");
        // move unit to target position
        ACTION_RATION.postAnimationUnit.moveUnitAction(action);
        ACTION_RATION.postAnimationUnit.setHasMoved(true);
    };
	this.performPostAnimation = function()
    {
        ACTION_RATION.giveRation(ACTION_RATION.postAnimationUnit);
        ACTION_RATION.postAnimationUnit = null;
    };

    this.giveRation = function(unit)
    {
        var x = unit.getX() + 1;
        var y = unit.getY();
        var animation = null;
        if (ACTION_RATION.checkUnit(unit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", map.getImageSize() / 2, map.getImageSize() / 3, 400);
            animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);
        }
        x = unit.getX() - 1;
        if (ACTION_RATION.checkUnit(unit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", map.getImageSize() / 2, map.getImageSize() / 3, 400);
            animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);
        }
        x = unit.getX();
        y = unit.getY() + 1;
        if (ACTION_RATION.checkUnit(unit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", map.getImageSize() / 2, map.getImageSize() / 3, 400);
            animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);
        }
        y = unit.getY() - 1;
        if (ACTION_RATION.checkUnit(unit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", map.getImageSize() / 2, map.getImageSize() / 3, 400);
            animation.addText(qsTr("RATION"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);
        }
	};
}

Constructor.prototype = ACTION;
var ACTION_RATION = new Constructor();
