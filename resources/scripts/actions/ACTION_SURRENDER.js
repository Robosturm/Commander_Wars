var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Surrender Game");
    };
    this.getIcon = function(map)
    {
        return "surrender";
    };
    this.perform = function(action, map)
    {
        map.surrenderGame();
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
    this.getName = function(map)
    {
        return qsTr("Surrender game");
    };
    this.getDescription = function()
    {
        return qsTr("You lose the game.");
    };
}


Constructor.prototype = ACTION;
var ACTION_SURRENDER = new Constructor();
