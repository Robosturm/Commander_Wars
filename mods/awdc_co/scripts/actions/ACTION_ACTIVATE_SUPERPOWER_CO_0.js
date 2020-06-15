// called for loading the main sprite
ACTION_ACTIVATE_SUPERPOWER_CO_0.canBePerformed = function(action)
{
    var co = map.getCurrentPlayer().getCO(0);
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

ACTION_ACTIVATE_SUPERPOWER_CO_0.perform = function(action)
{
    var co = map.getCurrentPlayer().getCO(0);
    co.getCOUnit().setHasMoved(true);
    co.activateSuperpower(GameEnums.PowerMode_Superpower);
    map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                          GameEnums.GameRecord_SpecialEvents_SuperPower);
    co.setPowerMode(GameEnums.PowerMode_Power);
};
