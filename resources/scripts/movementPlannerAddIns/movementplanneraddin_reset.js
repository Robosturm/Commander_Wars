var Constructor = function()
{
    this.getName = function()
    {
        return qsTr("Reset planner");
    };
    this.getGroupName = function()
    {
        return qsTr("Menu");
    };
    this.getDescription = function()
    {
        return qsTr("Resets the planner to the current map state.");
    };
    this.execute = function(addIn, map, planner)
    {
        planner.reloadMap();
        return false;
    };
};

Constructor.prototype = MOVEMENTPLANNERADDIN;
var MOVEMENTPLANNERADDIN_RESET = new Constructor();
