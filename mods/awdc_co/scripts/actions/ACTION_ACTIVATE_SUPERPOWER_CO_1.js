ACTION_ACTIVATE_SUPERPOWER_CO_1.perform = function(action)
{
    map.getCurrentPlayer().getCO(1).activateSuperpower(GameEnums.PowerMode_Superpower);
    map.getGameRecorder().addSpecialEvent(map.getCurrentPlayer().getPlayerID(),
                                          GameEnums.GameRecord_SpecialEvents_SuperPower);
	map.getCurrentPlayer().getCO(1).setPowerMode(GameEnums.PowerMode_Power);
};