var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Exit Game");
    };
    this.getIcon = function(map)
    {
        return "exitgame";
    };
    this.perform = function(action, map)
    {
        map.exitGame();
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_EXITGAME = new Constructor();
