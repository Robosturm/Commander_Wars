var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Save Game");
    };
    this.getIcon = function(map)
    {
        return "savegame";
    };
    this.perform = function(action, map)
    {
        map.saveGame();
    };
    this.isFinalStep = function(action, map)
    {
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_SAVEGAME = new Constructor();
