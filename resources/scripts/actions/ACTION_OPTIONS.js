var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Options");
    };
    this.getIcon = function()
    {
        return "options";
    };
    this.perform = function(action)
    {
        map.options();
    };
    this.isFinalStep = function(action)
    {
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_OPTIONS = new Constructor();
