var Constructor = function()
{
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
        action.setIsLocal(true);
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_SURRENDER = new Constructor();
