ACTION_ACTIVATE_SUPERPOWER_CO_0.perform = function(action)
{
    map.getCurrentPlayer().getCO(0).activateSuperpower(GameEnums.PowerMode_Superpower);
    map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                          GameEnums.GameRecord_SpecialEvents_SuperPower);
	map.getCurrentPlayer().getCO(0).setPowerMode(GameEnums.PowerMode_Power);
};