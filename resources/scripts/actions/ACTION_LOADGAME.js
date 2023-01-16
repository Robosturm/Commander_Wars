var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Load game");
    };
    this.getIcon = function(map)
    {
        return "savegame";
    };
    this.perform = function(action, map)
    {
        map.loadGame();
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
    this.getName = function()
    {
        return qsTr("Load game");
    };
    this.getDescription = function()
    {
        return qsTr("Allows you to load a save game.");
    };
}


Constructor.prototype = ACTION;
var ACTION_LOADGAME = new Constructor();
