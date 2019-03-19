var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Exit Game");
    };
    this.getIcon = function()
    {
        return "exitgame";
    };
    this.perform = function(action)
    {
        map.exitGame();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_EXITGAME = new Constructor();
