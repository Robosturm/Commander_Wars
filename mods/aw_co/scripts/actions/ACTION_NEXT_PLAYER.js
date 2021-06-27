var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if (co0 !== null && co1 !== null &&
            co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
            co1.getPowerMode() === GameEnums.PowerMode_Off)
        {
            return false;
        }
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
