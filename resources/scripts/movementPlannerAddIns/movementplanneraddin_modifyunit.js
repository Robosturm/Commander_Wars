var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Modify unit");
    };
    this.getGroupName = function()
    {
        return qsTr("Units");
    };
    this.getDescription = function()
    {
        return qsTr("Allows to modify a unit, e.g. it's health, fuel, ammo etc.");
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
        if (unit !== null)
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
        if (stepInput === 0 || unit === null)
        {
            return GameEnums.AddinStepType_Field;
        }
        else
        {
            var hpVariable = variables.createVariable("hp");
            if (!unit.getHpHidden(map.getCurrentViewPlayer()))
            {
                hpVariable.writeDataInt32(unit.getHpRounded());
            }
            else
            {
                hpVariable.writeDataInt32(10);
            }
            var ammo1Variable = variables.createVariable("ammo1");
            ammo1Variable.writeDataInt32(unit.getAmmo1());
            var ammo2Variable = variables.createVariable("ammo2");
            ammo2Variable.writeDataInt32(unit.getAmmo2());
            var fuelVariable = variables.createVariable("fuel");
            fuelVariable.writeDataInt32(unit.getFuel());
            var hasMovedVariable = variables.createVariable("hasMoved");
            hasMovedVariable.writeDataBool(unit.getHasMoved());
            return GameEnums.AddinStepType_Menu;
        }
    };
    this.getUiXml = function(addIn, map, planner)
    {
        return "scripts/movementPlannerAddIns/movementplanneraddin_modifyunit.xml";
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
        if (stepInput === 2 && unit !== null)
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
        var ammo1Variable = variables.createVariable("ammo1");
        var ammo2Variable = variables.createVariable("ammo2");
        var fuelVariable = variables.createVariable("fuel");
        var hasMovedVariable = variables.createVariable("hasMoved");
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit !== null)
        {
            var hp  = hpVariable.readDataInt32();
            unit.setHp(hp);
            var ammo1  = ammo1Variable.readDataInt32();
            unit.setAmmo1(ammo1);
            var ammo2  = ammo2Variable.readDataInt32();
            unit.setAmmo2(ammo2);
            var fuel  = fuelVariable.readDataInt32();
            unit.setFuel(fuel);
            var hasMoved  = hasMovedVariable.readDataBool();
            unit.setHasMoved(hasMoved);
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
    this.changeHp = function(addIn, hp)
    {
        var variables = addIn.getVariables();
        var hpVariable = variables.createVariable("hp");
        hpVariable.writeDataInt32(hp)
    };
    this.changeAmmo1 = function(addIn, ammo1)
    {
        var variables = addIn.getVariables();
        var ammo1Variable = variables.createVariable("ammo1");
        ammo1Variable.writeDataInt32(ammo1)
    };
    this.changeAmmo2 = function(addIn, ammo2)
    {
        var variables = addIn.getVariables();
        var ammo2Variable = variables.createVariable("ammo2");
        ammo2Variable.writeDataInt32(ammo2)
    };
    this.changeFuel = function(addIn, fuel)
    {
        var variables = addIn.getVariables();
        var fuelVariable = variables.createVariable("fuel");
        fuelVariable.writeDataInt32(fuel)
    };
    this.changeHasMoved = function(addIn, value)
    {
        var variables = addIn.getVariables();
        var hasMovedVariable = variables.createVariable("hasMoved");
        hasMovedVariable.writeDataBool(value)
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_MODIFYUNIT = new Constructor();
