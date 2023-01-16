var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Spawn unit");
    };

    this.getGroupName = function()
    {
        return qsTr("Units");
    };

    this.getDescription = function()
    {
        return qsTr("Allows to modify a unit, e.g. it's health, fuel, ammo etc.");
    };
    this.getUnitLabel = function()
    {
        return qsTr("Unit:");
    };
    this.getUnitTooltip = function()
    {
        return qsTr("Unit that will be spawned.");
    };
    this.getOwnerLabel = function()
    {
        return qsTr("Owner:");
    };
    this.getOwnerTooltip = function()
    {
        return qsTr("Owner of the unit.");
    };
    this.getHpLabel = function()
    {
        return qsTr("HP:");
    };
    this.getHpTooltip = function()
    {
        return qsTr("HP of the unit.");
    };
    this.getHasMovedLabel = function()
    {
        return qsTr("Has moved:");
    };
    this.getHasMovedTooltip = function()
    {
        return qsTr("If the unit has moved this turn or not.");
    };
    this.getApplyLabel = function()
    {
        return qsTr("Apply");
    };
    this.getApplyTooltip = function()
    {
        return qsTr("Spawns the unit.");
    };
    this.getCancelLabel = function()
    {
        return qsTr("Cancel");
    };
    this.getCancelTooltip = function()
    {
        return qsTr("Cancels the dialog.");
    };

    this.startAddIn = function(addIn)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(0);
    };
    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+edit");
        cursorData.setXOffset(0);
        cursorData.setYOffset(0);
        cursorData.setScale(1.0);
    };

    this.onFieldSelected = function(addIn, x, y, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var stepInputVariable = variables.createVariable("stepInput");
        xVariable.writeDataInt32(x);
        yVariable.writeDataInt32(y);
        var unit = map.getTerrain(x, y).getUnit();
        if (unit === null)
        {
            stepInputVariable.writeDataInt32(1);
        }
    };

    this.nextInputType = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var stepInputVariable = variables.createVariable("stepInput");
        var stepInput = stepInputVariable.readDataInt32();
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (stepInput === 0 || unit !== null)
        {
            return GameEnums.AddinStepType_Field;
        }
        else
        {
            var ownerVariable = variables.createVariable("owner");
            ownerVariable.writeDataInt32(1);
            var hpVariable = variables.createVariable("hp");
            hpVariable.writeDataInt32(10);
            var unitIdVariable = variables.createVariable("unitId");
            unitIdVariable.writeDataString(unitSpriteManager.getUnitsSorted()[0]);
            var hasMovedVariable = variables.createVariable("hasMoved");
            hasMovedVariable.writeDataBool(true);
            return GameEnums.AddinStepType_Menu;
        }
    };

    this.getUiXml = function(addIn, map, planner)
    {
        return "scripts/movementPlannerAddIns/movementplanneraddin_spawnunit.xml";
    };

    this.readyToExecute = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var stepInputVariable = variables.createVariable("stepInput");
        var stepInput = stepInputVariable.readDataInt32();
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (stepInput === 2 && unit === null)
        {
            return true;
        }
        else
        {
            return false;
        }
    };

    this.onMenuInputDone = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(2);
    };

    this.execute = function(addIn, map, planner)
    {
        addIn.hide();
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var stepInputVariable = variables.createVariable("stepInput");
        var hpVariable = variables.createVariable("hp");
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit === null)
        {
            var ownerVariable = variables.createVariable("owner");
            var owner = ownerVariable.readDataInt32() - 1;
            var unitIdVariable = variables.createVariable("unitId");
            var hasMovedVariable = variables.createVariable("hasMoved");
            var unitId = unitIdVariable.readDataString(unitSpriteManager.getUnitsSorted()[0]);
            map.spawnUnit(x, y, unitId, map.getPlayer(owner));
            unit = map.getTerrain(x, y).getUnit();
            if (unit !== null)
            {
                var hp  = hpVariable.readDataInt32();
                unit.setHp(hp);
                var hasMoved  = hasMovedVariable.readDataBool();
                unit.setHasMoved(hasMoved);
            }
        }
        stepInputVariable.writeDataInt32(0);
        return false;
    };
    this.getSelectedUnit = function(addIn)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        return addIn.getMap().getTerrain(x, y).getUnit();
    };

    this.changeUnitId = function(addIn, unitId)
    {
        var variables = addIn.getVariables();
        var unitIdVariable = variables.createVariable("unitId");
        unitIdVariable.writeDataString(unitId)
    };

    this.changeHp = function(addIn, hp)
    {
        var variables = addIn.getVariables();
        var hpVariable = variables.createVariable("hp");
        hpVariable.writeDataInt32(hp)
    };

    this.changeOwner = function(addIn, owner)
    {
        var variables = addIn.getVariables();
        var ownerVariable = variables.createVariable("owner");
        ownerVariable.writeDataInt32(owner)
    };

    this.changeHasMoved = function(addIn, value)
    {
        var variables = addIn.getVariables();
        var hasMovedVariable = variables.createVariable("hasMoved");
        hasMovedVariable.writeDataBool(value)
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_SPAWNUNIT = new Constructor();
