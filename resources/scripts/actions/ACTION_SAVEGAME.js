var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Save Game");
    };
    this.getIcon = function()
    {
        return "savegame";
    };
    this.perform = function(action)
    {
        map.saveGame();
    };
    this.isFinalStep = function(action)
    {
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_SAVEGAME = new Constructor();
