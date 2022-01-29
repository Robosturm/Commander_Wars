var Constructor = function()
{
    
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Next Player");
    };
    this.getIcon = function(map)
    {
        return "next_player";
    };
    this.perform = function(action, map)
    {
        map.nextTurn();
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getDescription = function()
    {
        return qsTr("Ends your turn and gives control to the next player.");
    };
}


Constructor.prototype = ACTION;
var ACTION_NEXT_PLAYER = new Constructor();
