var Constructor = function()
{
    // called for loading the main sprite
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
}


Constructor.prototype = ACTION;
var ACTION_NEXT_PLAYER = new Constructor();
