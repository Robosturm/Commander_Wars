var Constructor = function()
{
    
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Next Player");
    };
    this.getIcon = function()
    {
        return "next_player";
    };
    this.perform = function(action)
    {
        map.nextTurn();
    };
    this.isFinalStep = function(action)
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
