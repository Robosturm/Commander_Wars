ACTION_ACTIVATE_TAGPOWER.canBePerformed = function(action, map)
{
    var player = map.getCurrentPlayer();
    var co0 = player.getCO(0);
    var co1 = player.getCO(1);
    if ((co0 !== null) && co0.canUseSuperpower() &&
        (co1 !== null) && co1.canUseSuperpower())
    {
        return true;
    }
    return false;
};

ACTION_ACTIVATE_TAGPOWER.perform = function(action, map)
{
    map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Tagpower);
    map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                          GameEnums.GameRecord_SpecialEvents_SuperPower);
};
