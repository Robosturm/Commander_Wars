var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Surrender Game");
    };
    this.getIcon = function()
    {
        return "surrender";
    };
    this.perform = function(action)
    {
        map.surrenderGame();
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_SURRENDER = new Constructor();
