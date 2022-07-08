var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Delete unit");
    };
    this.getGroupName = function()
    {
        return qsTr("Units");
    };
    this.getDescription = function()
    {
        return qsTr("Deletes units from the map.");
    };
    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+delete");
        cursorData.setXOffset(0);
        cursorData.setYOffset(0);
        cursorData.setScale(1.0);
    };
    this.onFieldSelected = function(addIn, x, y, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        xVariable.writeDataInt32(x);
        yVariable.writeDataInt32(y);
    };
    this.execute = function(addIn, map, planner)
    {
        var variables = addIn.getVariables();
        var xVariable = variables.createVariable("x");
        var yVariable = variables.createVariable("y");
        var x = xVariable.readDataInt32();
        var y = yVariable.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit !== null)
        {
            unit.removeUnit(false);
        }
        return false;
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_DELETEUNIT = new Constructor();
