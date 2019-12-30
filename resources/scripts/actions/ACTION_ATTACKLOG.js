var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Attack Log");
    };
    this.getIcon = function()
    {
        return "help";
    };
    this.perform = function(action)
    {
        map.showAttackLog();
    };
    this.isFinalStep = function(action)
    {
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_ATTACKLOG = new Constructor();
