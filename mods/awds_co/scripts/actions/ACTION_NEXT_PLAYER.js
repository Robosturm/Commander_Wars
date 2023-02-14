ACTION_NEXT_PLAYER.canBePerformed = function(action, map)
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
