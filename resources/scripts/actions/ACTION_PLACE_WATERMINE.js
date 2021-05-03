var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();

        if ((unit.getHasMoved() === true) ||
            (unit.getAmmo2() === 0) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
                (action.getMovementTarget() === null))
        {
            if (ACTION_PLACE_WATERMINE.getMineFields(action).length > 0)
            {
                return true;
            }
        }
        return false;
    };
    this.getMineFields = function(action)
    {
        var targetField = action.getActionTarget();
        var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                            Qt.point(targetField.x - 1, targetField.y),
                            Qt.point(targetField.x,     targetField.y - 1),
                            Qt.point(targetField.x,     targetField.y + 1)];
        var unit = action.getTargetUnit();
        var targetTerrain = map.getTerrain(targetField.x, targetField.y);
        var ret = [];
        // check all neighbour terrains
        for (var i = 0; i < targetFields.length; i++)
        {
            if (map.onMap(targetFields[i].x, targetFields[i].y))
            {
                var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
                var defUnit = terrain.getUnit();
                // can the transported unit move over the terrain?
                var terrainId = terrain.getID();
                if (terrainId !== "BRIDGE" &&
                    terrainId !== "BEACH")
                {
                    if ((Global[unit.getMovementType()].getMovementpoints(terrain, unit, terrain, false) > 0) &&
                        (defUnit === null))
                    {
                        ret.push(targetFields[i]);
                    }
                }
            }
        }
        return ret;
    };

    this.getActionText = function()
    {
        return qsTr("Place Mine");
    };
    this.getIcon = function()
    {
        return "WATERMINE";
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

    this.getStepInputType = function(action)
    {        
        return "FIELD";
    };

    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        data.setColor("#C800FF00");
        var fields = ACTION_PLACE_WATERMINE.getMineFields(action);
        for (var i3 = 0; i3 < fields.length; i3++)
        {
            data.addPoint(Qt.point(fields[i3].x, fields[i3].y));
        }
    };

    this.postAnimationMinePosX = -1;
    this.postAnimationMinePosY = -1;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_PLACE_WATERMINE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        unit.reduceAmmo2(1);
        action.startReading();
        var x = action.readDataInt32();
        var y = action.readDataInt32();
        ACTION_PLACE_WATERMINE.postAnimationMinePosX = x;
        ACTION_PLACE_WATERMINE.postAnimationMinePosY = y;
    };
    this.performPostAnimation = function(postAnimation)
    {
        // unloading the units here :)
        var player = map.getCurrentPlayer();
        var unit = map.spawnUnit(ACTION_PLACE_WATERMINE.postAnimationMinePosX,
                                 ACTION_PLACE_WATERMINE.postAnimationMinePosY,
                                 "WATERMINE", player);
        if (unit !== null)
        {
            // pay for the unit
            map.getGameRecorder().buildUnit(player.getPlayerID(), unitID);
            unit.setHasMoved(true);
        }
        player.buildedUnit(unit);
        audio.playSound("unload.wav");
        ACTION_PLACE_WATERMINE.postAnimationMinePosX = -1;
        ACTION_PLACE_WATERMINE.postAnimationMinePosY = -1;
    };
    this.getDescription = function()
    {
        return qsTr("Places a stealthed Watermine at a given Position. The Mine automatically explodes at the start of a turn if an enemy is in explosion range.");
    };
}

Constructor.prototype = ACTION;
var ACTION_PLACE_WATERMINE = new Constructor();
