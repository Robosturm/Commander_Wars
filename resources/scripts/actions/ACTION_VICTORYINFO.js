var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Victory Info");
    };
    this.getIcon = function()
    {
        return "help";
    };
    this.perform = function(action)
    {
        map.victoryInfo();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_VICTORYINFO = new Constructor();
