var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Planner");
    };
    this.getIcon = function(map)
    {
        return "help";
    };
    this.perform = function(action, map)
    {
        map.showMovementPlanner();
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
    this.getStepInputType = function(action, map)
    {
        return "MENU";
    };
}


Constructor.prototype = ACTION;
var ACTION_MOVEMENTPLANNER = new Constructor();
