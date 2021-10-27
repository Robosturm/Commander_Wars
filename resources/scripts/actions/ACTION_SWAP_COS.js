var Constructor = function()
{
    this.canBePerformed = function(action)
    {
        // disabled in normal mode
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Swap CO's");
    };
    this.getIcon = function()
    {
        return "swap";
    };
    this.perform = function(action)
    {
        var player = map.getCurrentPlayer();
        var co0 = player.getCO(0);
        var co1 = player.getCO(1);
        if (co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
            co1.getPowerMode() === GameEnums.PowerMode_Off)
        {
            // start another movement turn by reenabling all units :)
            player.swapCOs();
            map.enableUnits(player);
        }
        else
        {
            player.swapCOs();
            map.nextTurn();
        }


    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}


Constructor.prototype = ACTION;
var ACTION_SWAP_COS = new Constructor();
