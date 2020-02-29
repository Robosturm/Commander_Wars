var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if (((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
            (action.getMovementTarget() === null)) &&
            (ACTION_SUPPORTSINGLE_FREEREPAIR.getRepairFields(action).length > 0))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Repair");
    };
    this.getIcon = function()
    {
        return "repair";
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
    };
    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        data.setColor("#C800FF00");
        var fields = ACTION_SUPPORTSINGLE_FREEREPAIR.getRepairFields(action);
        for (var i3 = 0; i3 < fields.length; i3++)
        {
            data.addPoint(Qt.point(fields[i3].x, fields[i3].y));
        }
    };
    this.getRepairFields = function(action)
    {
        var targetField = action.getActionTarget();
        var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                            Qt.point(targetField.x - 1, targetField.y),
                            Qt.point(targetField.x,     targetField.y - 1),
                            Qt.point(targetField.x,     targetField.y + 1)];
        // check all neighbour terrains
        var unit = action.getTargetUnit();
        var ret = [];
        for (var i = 0; i < targetFields.length; i++)
        {
            if (map.onMap(targetFields[i].x, targetFields[i].y))
            {
                var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
                var repairUnit = terrain.getUnit();
                // can we repair the unit?
                if (repairUnit !== null &&
                    repairUnit.getOwner() === unit.getOwner() &&
                    repairUnit !== unit)
                {
                    ret.push(targetFields[i]);
                }
            }
        }
        return ret;
    };

    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.postAnimationUnit = null;
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_SUPPORTSINGLE_FREEREPAIR", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        action.startReading();
        // read action data
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit = unit;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX = action.readDataInt32();
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY = action.readDataInt32();
    };
    this.performPostAnimation = function(postAnimation)
    {
        var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
        var repairUnit = terrain.getUnit();
        var animation = GameAnimationFactory.createAnimation(ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
        animation.addSprite("repair", map.getImageSize() / 2, map.getImageSize() / 3, 400);
        animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);

        repairUnit.refill();
        repairUnit.setHp(repairUnit.getHp() + 1);
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit.setHasMoved(true);
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit = null;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX = -1;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY = -1;
    };
}

Constructor.prototype = ACTION;
var ACTION_SUPPORTSINGLE_FREEREPAIR = new Constructor();
