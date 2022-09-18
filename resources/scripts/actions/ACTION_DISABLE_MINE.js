var Constructor = function()
{
    this.mines = ["WATERMINE"];
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var transportTerrain = map.getTerrain(actionTargetField.x, actionTargetField.y);
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }

        if (ACTION_DISABLE_MINE.getDisableFields(action, map).length > 0)
        {
            return true;
        }
        return false;

    };

    this.getDisableFields = function(action, map)
    {
        var unit = action.getTargetUnit();
        var owner = unit.getOwner();
        var targetField = action.getActionTarget();
        var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                            Qt.point(targetField.x - 1, targetField.y),
                            Qt.point(targetField.x,     targetField.y - 1),
                            Qt.point(targetField.x,     targetField.y + 1)];        
        var ret = [];
        // check all neighbour terrains
        for (var i = 0; i < targetFields.length; i++)
        {
            if (map.onMap(targetFields[i].x, targetFields[i].y))
            {
                var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
                var mine = terrain.getUnit();
                // can the transported unit move over the terrain?
                if (mine !== null &&
                    !mine.isStealthed(owner) &&
                    owner.isEnemyUnit(mine) &&
                    ACTION_DISABLE_MINE.mines.includes(mine.getUnitID()))
                {
                    ret.push(targetFields[i]);
                }
            }
        }
        return ret;
    };

    this.getActionText = function(map)
    {
        return qsTr("Remove mine");
    };
    this.getIcon = function(map)
    {
        return "icon_fire";
    };
    this.isFinalStep = function(action, map)
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

    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };

    this.getStepData = function(action, data, map)
    {
        data.setColor("#FFFF0000");
        var fields = ACTION_DISABLE_MINE.getDisableFields(action, map);
        var size = fields.length;
        for (var i = 0; i < size; i++)
        {
            data.addPoint(fields[i]);
        }
    };
    this.getDescription = function()
    {
        return qsTr("Destroys a mine without it damaging another unit.");
    };
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.postAnimationPlayer = null;
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_DISABLE_MINE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        action.startReading();
        // read action data
        ACTION_DISABLE_MINE.postAnimationTargetX = action.readDataInt32();
        ACTION_DISABLE_MINE.postAnimationTargetY = action.readDataInt32();
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        var terrain = map.getTerrain(ACTION_DISABLE_MINE.postAnimationTargetX, ACTION_DISABLE_MINE.postAnimationTargetY);
        var targetUnit = terrain.getUnit();
        if (targetUnit !== null)
        {
            // we destroyed a unit
            map.getGameRecorder().destroyedUnit(targetUnit.getOwner().getPlayerID(), targetUnit.getUnitID(), targetUnit.getOwner().getPlayerID());
            targetUnit.killUnit();
        }
        ACTION_DISABLE_MINE.postAnimationTargetX = -1;
        ACTION_DISABLE_MINE.postAnimationTargetY = -1;
    };
}

Constructor.prototype = ACTION;
var ACTION_DISABLE_MINE = new Constructor();
