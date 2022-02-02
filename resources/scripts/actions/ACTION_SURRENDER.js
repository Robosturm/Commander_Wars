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
}


Constructor.prototype = ACTION;
var ACTION_SURRENDER = new Constructor();
