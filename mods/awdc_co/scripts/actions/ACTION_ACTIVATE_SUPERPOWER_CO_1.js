ACTION_ACTIVATE_SUPERPOWER_CO_1.canBePerformed = function(action, map)
{
    var co = map.getCurrentPlayer().getCO(1);
    if ((co !== null) &&
        co.getCOUnit() !== null &&
        co.canUseSuperpower() &&
        co.getCOUnit().getHasMoved() === false)
    {
        return true;
    }
    else
    {
        return false;
    }
};

ACTION_ACTIVATE_SUPERPOWER_CO_1.perform = function(action, map)
{
    var co = map.getCurrentPlayer().getCO(1);
    co.getCOUnit().setHasMoved(true);
    co.activateSuperpower(GameEnums.PowerMode_Superpower);
    map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                          GameEnums.GameRecord_SpecialEvents_SuperPower);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
