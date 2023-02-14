ACTION_SWAP_COS.canBePerformed = function(action, map)
{
    var player = map.getCurrentPlayer();
    var co0 = player.getCO(0);
    var co1 = player.getCO(1);
    if (co0 !== null && co1 !== null)
    {
        if   (co0.getPowerMode() === GameEnums.PowerMode_Tagpower &&
              co1.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return false;
        }
        return true;
    }
    return false;
};
ACTION_SWAP_COS.perform = function(action, map)
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
        map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Tagpower);
    }
    else
    {
        player.swapCOs();
        map.nextTurn();
    }
};
