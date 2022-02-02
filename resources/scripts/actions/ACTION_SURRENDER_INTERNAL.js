var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return "unknown";
    };
    this.getIcon = function(map)
    {
        return "";
    };
    this.perform = function(action, map)
    {
        map.getCurrentPlayer().defeatPlayer(null);
		map.nextTurn();
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
}

Constructor.prototype = ACTION;
var ACTION_SURRENDER_INTERNAL = new Constructor();
