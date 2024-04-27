var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Unknown");
    };
    this.getIcon = function(map)
    {
        return "surrender";
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
    this.getName = function(map)
    {
        return qsTr("Game: surrender game");
    };
    this.getDescription = function()
    {
        return qsTr("You lose the game.");
    };
}

Constructor.prototype = ACTION;
var ACTION_SURRENDER_INTERNAL = new Constructor();
