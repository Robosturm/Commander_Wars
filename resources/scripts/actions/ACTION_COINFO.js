var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("CO Info");
    };
    this.getIcon = function()
    {
        return "co_info";
    };
    this.perform = function(action)
    {
        map.showCOInfo();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_COINFO = new Constructor();
