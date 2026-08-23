var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Toggle AI pause");
    };
    this.getIcon = function(map)
    {
        return "wait";
    };
    this.perform = function(action, map)
    {
        map.toggleAiPause()
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
    this.getName = function(map)
    {
        return qsTr("Toggle AI pause");
    };
    this.getDescription = function()
    {
        return qsTr("Toggles between an pausing all ai's and let them perform actions.");
    };
}


Constructor.prototype = ACTION;
var ACTION_TOGGLE_AI_PAUSE = new Constructor();

