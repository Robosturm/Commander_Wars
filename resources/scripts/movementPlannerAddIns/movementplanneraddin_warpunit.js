var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Warp unit");
    };
    this.getGroupName = function()
    {
        return qsTr("Units");
    };
    this.getDescription = function()
    {
        return qsTr("Warp units around the map.");
    };
    this.startAddIn = function(addIn)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(0);
    };
    this.onFieldSelected = function(addIn, x, y, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var x2Variable = variables.createVariable("x2");
        var y2Variable = variables.createVariable("y2");
        var stepInputVariable = variables.createVariable("stepInput");
        var stepInput = stepInputVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (stepInput === 0 && unit !== null)
        {
            xVariable.writeDataInt32(x);
            yVariable.writeDataInt32(y);
            stepInput = stepInput + 1;
            stepInputVariable.writeDataInt32(stepInput);
        }
        else if (stepInput === 1 && unit === null)
        {
            x2Variable.writeDataInt32(x);
            y2Variable.writeDataInt32(y);
            stepInput = stepInput + 1;
            stepInputVariable.writeDataInt32(stepInput);
        }
    };
    this.readyToExecute = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var stepInputVariable = variables.createVariable("stepInput");
        var stepInput = stepInputVariable.readDataInt32();
        if (stepInput === 2)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.execute = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var x2Variable = variables.createVariable("x2");
        var y2Variable = variables.createVariable("y2");
        var stepInputVariable = variables.createVariable("stepInput");
        stepInputVariable.writeDataInt32(0);
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var x2 = x2Variable.readDataInt32();
        var y2 = y2Variable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit !== null)
        {
            unit.moveUnitToField(x2, y2);
        }
        return false;
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_WARPUNIT = new Constructor();
