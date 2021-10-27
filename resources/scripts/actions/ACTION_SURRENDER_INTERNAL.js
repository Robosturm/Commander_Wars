var Constructor = function()
{
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return "unknown";
    };
    this.getIcon = function()
    {
        return "";
    };
    this.perform = function(action)
    {
        map.getCurrentPlayer().defeatPlayer(null);
		map.nextTurn();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}

Constructor.prototype = ACTION;
var ACTION_SURRENDER_INTERNAL = new Constructor();
