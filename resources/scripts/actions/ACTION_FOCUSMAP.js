var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Focus map");
    };
    this.getIcon = function(map)
    {
        return "options";
    };
    this.perform = function(action, map)
    {
        map.autoFocus();
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getStepInputType = function(action, map)
    {
        return "MENU";
    };

    this.getStepData = function(action, data, map)
    {

    };
    this.getName = function(map)
    {
        return qsTr("Focus map");
    };
    this.getDescription = function()
    {
        return qsTr("Centers the map and changes the map to the optimal size to fit on screen.");
    };
}


Constructor.prototype = ACTION;
var ACTION_FOCUSMAP = new Constructor();
