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
        var animation = this[ACTION_RATION.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_RATION", "performPostAnimation");
        // move unit to target position
        ACTION_RATION.postAnimationUnit.moveUnitAction(action);
        ACTION_RATION.postAnimationUnit.setHasMoved(true);
    };
	this.performPostAnimation = function()
    {
        var x = ACTION_RATION.postAnimationUnit.getX() + 1;
        var y = ACTION_RATION.postAnimationUnit.getY();
        var animation = null;
        if (ACTION_RATION.checkUnit(ACTION_RATION.postAnimationUnit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", 12, 8, 400);
        }
        x = ACTION_RATION.postAnimationUnit.getX() - 1;
        if (ACTION_RATION.checkUnit(ACTION_RATION.postAnimationUnit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", 12, 8, 400);
        }
        x = ACTION_RATION.postAnimationUnit.getX();
        y = ACTION_RATION.postAnimationUnit.getY() + 1;
        if (ACTION_RATION.checkUnit(ACTION_RATION.postAnimationUnit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", 12, 8, 400);
        }
        y = ACTION_RATION.postAnimationUnit.getY() - 1;
        if (ACTION_RATION.checkUnit(ACTION_RATION.postAnimationUnit, x, y))
        {
            map.getTerrain(x, y).getUnit().refill();
            animation = GameAnimationFactory.createAnimation(x, y);
            animation.addSprite("ration", 12, 8, 400);
        }
        ACTION_RATION.postAnimationUnit = null;
	};
}

Constructor.prototype = ACTION;
var ACTION_RATION = new Constructor();
