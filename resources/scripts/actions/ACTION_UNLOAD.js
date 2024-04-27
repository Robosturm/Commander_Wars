var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var transportTerrain = map.getTerrain(actionTargetField.x, actionTargetField.y);
        if (ACTION_UNLOAD.isUnloadTerrain(unit, transportTerrain, map) &&
            ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
        {
            var size = unit.getLoadedUnitCount();
            for (var i = 0; i < size; i++)
            {
                if (ACTION_UNLOAD.getUnloadFields(action, i, map).length > 0)
                {
                    return true;
                }
            }
        }
        return false;

    };

    this.isUnloadTerrain = function(unit, transportTerrain, map)
    {
        var unitID = unit.getUnitID();
        var terrainId = transportTerrain.getID();
        if (unitID === "LANDER" ||
            unitID === "BLACK_BOAT" ||
            unitID === "CANNONBOAT")
        {
            if ((transportTerrain !== null &&
                 (terrainId === "BRIDGE" ||
                  terrainId === "BRIDGE1" ||
                  terrainId === "BRIDGE2")))
            {
                return false;
            }
        }
        return true;
    };

    this.getUnloadFields = function(action, transportUnitIdx, map)
    {
        var targetField = action.getActionTarget();
        var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                            Qt.point(targetField.x - 1, targetField.y),
                            Qt.point(targetField.x,     targetField.y - 1),
                            Qt.point(targetField.x,     targetField.y + 1)];
        var unit = action.getTargetUnit();
        var targetTerrain = map.getTerrain(targetField.x, targetField.y);
        var transportUnit = unit.getLoadedUnit(transportUnitIdx);
        var ret = [];
        // can both units move over the current terrain?
        var moveType = Global[transportUnit.getMovementType()];
        if (ACTION_UNLOAD.isUnloadTerrain(unit, targetTerrain) &&
            (moveType.getMovementpoints(targetTerrain, transportUnit, targetTerrain, false, map) > 0))
        {
            // check all neighbour terrains
            for (var i = 0; i < targetFields.length; i++)
            {
                if (map.onMap(targetFields[i].x, targetFields[i].y))
                {
                    var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
                    var defUnit = terrain.getUnit();
                    // can the transported unit move over the terrain?
                    if ((Global[transportUnit.getMovementType()].getMovementpoints(terrain, transportUnit, targetTerrain, false, map) > 0) &&
                        (defUnit === null ||
                         defUnit.isStealthed(unit.getOwner()) ||
                         (((defUnit !== null) && (defUnit === unit)))))
                    {
                        ret.push(targetFields[i]);
                    }
                }
            }
        }
        return ret;
    };

    this.getActionText = function(map)
    {
        return qsTr("Unload");
    };
    this.getIcon = function(map)
    {
        return "load";
    };
    this.isFinalStep = function(action, map)
    {
        // check if the final step was a wait command
        var step = action.getInputStep();
        if (!globals.isEven(step))
        {
            action.startReading();
            for (var i = 0; i < step; i += 2)
            {
                var dataString = action.readDataString();
                if (i === step - 1)
                {
                    if (dataString === "ACTION_WAIT")
                    {
                        return true;
                    }
                }
                else
                {
                    action.readDataInt32();
                    action.readDataInt32();
                }
            }
        }
        else
        {
            var unloadableUnits = ACTION_UNLOAD.getUnloadableUnits(action, map);
            if (unloadableUnits.length === 0)
            {
                action.writeDataString("ACTION_WAIT");
                action.setInputStep(action.getInputStep() + 1);
                return true;
            }
        }
        return false;
    };

    this.getStepInputType = function(action, map)
    {
        // supported types are MENU and FIELD
        if (globals.isEven(action.getInputStep()))
        {
            // only one unit can be unloaded?
            var unitIndexes = [];
            var blockedFields = [];
            ACTION_UNLOAD.getUsedUnitsAndFields(action, unitIndexes, blockedFields);
            var unloadableUnits = ACTION_UNLOAD.getUnloadableUnits(action, map);
            if (unitIndexes.length === 0 && unloadableUnits.length === 1)
            {
                // skip selecting a unit when only one can be unloaded
                action.writeDataString(unloadableUnits[0]);
                action.setInputStep(action.getInputStep() + 1);
                return "FIELD";
            }
            else
            {
                return "MENU";
            }
        }
        else
        {
            return "FIELD";
        }
    };

    this.getUsedUnitsAndFields = function(action, unitIndexes, blockedFields)
    {
        action.startReading();
        var step = action.getInputStep();
        for (var i2 = 0; i2 < step; i2 += 2)
        {
            unitIndexes.push(parseInt(action.readDataString()));
            if (i2 !== step - 1)
            {
                var x = action.readDataInt32();
                var y = action.readDataInt32();
                blockedFields.push(Qt.point(x, y))
            }
        }
    };

    this.getUnloadableUnits = function(action, map)
    {
        var ret = [];
        var unitIndexes = [];
        var blockedFields = [];
        ACTION_UNLOAD.getUsedUnitsAndFields(action, unitIndexes, blockedFields);
        var unit = action.getTargetUnit();
        var fields = null;
        var i3 = 0;
        var i4 = 0;
        var found = false;
        var size = unit.getLoadedUnitCount();
        for (var i = 0; i < size; i++)
        {
            if (unitIndexes.indexOf(i) < 0)
            {
                fields = ACTION_UNLOAD.getUnloadFields(action, i, map);
                for (i3 = 0; i3 < fields.length; i3++)
                {
                    found = false;
                    for (i4 = 0; i4 < blockedFields.length; i4++)
                    {
                        if (blockedFields[i4] === fields[i3])
                        {
                            found = true;
                            break;
                        }
                    }
                    if (found === false)
                    {
                        var transportUnit = unit.getLoadedUnit(i);
                        ret.push(i.toString());
                        break;
                    }
                }
            }
        }
        return ret;
    };

    this.getStepData = function(action, data, map)
    {
        var step = action.getInputStep();

        var unitIndexes = [];
        var blockedFields = [];
        ACTION_UNLOAD.getUsedUnitsAndFields(action, unitIndexes, blockedFields);

        var fields = null;
        var i3 = 0;
        var i4 = 0;
        var found = false;
        if (globals.isEven(action.getInputStep()))
        {
            var unit = action.getTargetUnit();
            var size = unit.getLoadedUnitCount();
            for (var i = 0; i < size; i++)
            {
                if (unitIndexes.indexOf(i) < 0)
                {
                    fields = ACTION_UNLOAD.getUnloadFields(action, i, map);
                    for (i3 = 0; i3 < fields.length; i3++)
                    {
                        found = false;
                        for (i4 = 0; i4 < blockedFields.length; i4++)
                        {
                            if (blockedFields[i4] === fields[i3])
                            {
                                found = true;
                                break;
                            }
                        }
                        if (found === false)
                        {
                            var transportUnit = unit.getLoadedUnit(i);
                            data.addUnitData(Global[transportUnit.getUnitID()].getName(), i.toString(), transportUnit);
                            break;
                        }
                    }
                }
            }
            data.addData(qsTr("Wait"), "ACTION_WAIT", "wait");
        }
        else
        {
            // find out which unit we want to unload
            fields = ACTION_UNLOAD.getUnloadFields(action, unitIndexes[unitIndexes.length - 1], map);
            for (i3 = 0; i3 < fields.length; i3++)
            {
                found = false;
                for (i4 = 0; i4 < blockedFields.length; i4++)
                {
                    if (blockedFields[i4] === fields[i3])
                    {
                        found = true;
                        break;
                    }
                }
                if (found === false)
                {
                    data.addPoint(fields[i3]);
                }
            }
            data.setColor("#0000FF");
        }
    };

    this.postAnimationUnit = null;
    this.postAnimationTransportUnits = [];
    this.postAnimationTransportUnitsPosX = [];
    this.postAnimationTransportUnitsPosY = [];
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        ACTION_UNLOAD.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_UNLOAD.postAnimationUnit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_UNLOAD", "performPostAnimation");
        // move unit to target position
        ACTION_UNLOAD.postAnimationUnit.moveUnitAction(action);
        // disable unit commandments for this turn
        ACTION_UNLOAD.postAnimationUnit.setHasMoved(true);
        action.startReading();
        var step = action.getInputStep();
        for (var i = 0; i < step; i += 2)
        {
            var dataString = action.readDataString();
            if (i === step - 1)
            {
            }
            else
            {
                var x = action.readDataInt32();
                var y = action.readDataInt32();
                ACTION_UNLOAD.postAnimationTransportUnits.push(ACTION_UNLOAD.postAnimationUnit.getLoadedUnit(parseInt(dataString)));
                ACTION_UNLOAD.postAnimationTransportUnitsPosX.push(x);
                ACTION_UNLOAD.postAnimationTransportUnitsPosY.push(y);
            }
        }
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        // unloading the units here :)
        for (var i = 0; i < ACTION_UNLOAD.postAnimationTransportUnits.length; i++)
        {
            // check if the field is empty before unloading
            if (map.getTerrain(ACTION_UNLOAD.postAnimationTransportUnitsPosX[i],
                           ACTION_UNLOAD.postAnimationTransportUnitsPosY[i]).getUnit() === null)
            {
                ACTION_UNLOAD.postAnimationUnit.unloadUnit(ACTION_UNLOAD.postAnimationTransportUnits[i],
                                                           Qt.point(ACTION_UNLOAD.postAnimationTransportUnitsPosX[i],
                                                                    ACTION_UNLOAD.postAnimationTransportUnitsPosY[i]));
            }
        }
        audio.playSound("unload.wav");
        ACTION_UNLOAD.postAnimationUnit = null;
        ACTION_UNLOAD.postAnimationTransportUnits = [];
        ACTION_UNLOAD.postAnimationTransportUnitsPosX = [];
        ACTION_UNLOAD.postAnimationTransportUnitsPosY = [];
    };
    this.getName = function()
    {
        return qsTr("Unload");
    };
    this.getDescription = function()
    {
        return qsTr("Unloads loaded units. Units that have been loaded this turn can't move after being unloaded. Units that were loaded on previous turns can immediately move after being unloaded.");
    };
}

Constructor.prototype = ACTION;
var ACTION_UNLOAD = new Constructor();
